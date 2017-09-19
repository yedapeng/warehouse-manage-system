


#include "stm8s.h"
#include "dis.h"





uint8_t sys_mode = 0;

uint8_t own_addr;

extern void inf_write(uint8_t addr);



uint8_t dis_buff[8] = {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0};


unsigned char  LED_0F[] = 
{
  // 0123456789AbCdEF    -
  0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};

com_data_t      point_rx;
com_data_t      point_tx;


void LED_OUT(uint8_t X)
{
  uint8_t i;
  for(i=8;i>=1;i--)
  {
    if (X&0x80) DIO_H; else DIO_L;
    X<<=1;
    SCLK_L;
    SCLK_H;
  }
}

void LED4_Display (void)
{
  static unsigned char count = 0;
  //
  LED_OUT(0x01 << count);
  LED_OUT(dis_buff[count]);                        
                  

  RCLK_L;
  RCLK_H;
  
  if (count++ == 8)
  {
    count = 0;
  }
}

struct key_param_t
{
  uint8_t   state;
  uint8_t   sampletime;
  uint16_t   presstime;
} key;


#define KEY1_GPIO_Port  GPIOC
#define KEY2_GPIO_Port  GPIOC
#define KEY3_GPIO_Port  GPIOC

#define KEY1_Pin           GPIO_PIN_3
#define KEY2_Pin           GPIO_PIN_4
#define KEY3_Pin           GPIO_PIN_5


#define KEY_SAMPLE_TIME       50// 50ms
#define KEY_LONGPRESS_TIME    700// 50ms
#define KEY_SAMPLE_TICK       1// 5ms

#define KEY1_READ       GPIO_ReadInputPin(KEY1_GPIO_Port, KEY1_Pin)
#define KEY2_READ       GPIO_ReadInputPin(KEY2_GPIO_Port, KEY2_Pin)
#define KEY3_READ       GPIO_ReadInputPin(KEY3_GPIO_Port, KEY3_Pin)

#define KEY_LONG_COUNT          1000

/* I */
uint8_t key_read (void)
{
  uint8_t curkey;
  curkey = 0;

    if (! KEY1_READ)
    {
      curkey |= KEY_VALUE_KEY1;
    }
    

    if (! KEY2_READ)
    {
      curkey |= KEY_VALUE_KEY2;
    }

    
    if (! KEY3_READ)
    {
      curkey |= KEY_VALUE_KEY3;
    }

    

    if (curkey == 0)
    {
      key.state = 0;
      return 0;
    }

    if (curkey != key.state)
    {
      key.state = curkey;
      key.sampletime = (KEY_SAMPLE_TIME / KEY_SAMPLE_TICK);
      key.presstime = 0;
    }
    else
    {
      if (key.sampletime > 0)
      {
        key.sampletime --;
      }
      else
      {
        //key is down
        if (key.presstime)
        {
          if (key.presstime > (KEY_LONGPRESS_TIME / KEY_SAMPLE_TICK))
          {
            if (key.presstime == (KEY_LONGPRESS_TIME / KEY_SAMPLE_TICK) + 1)
            {
              key.presstime++;
              return key.state | KEY_VALUE_LONGPRESS;
            }
          }
          else
          {
            key.presstime++;
          }
        }
        else
        {
          //first press
          key.presstime++;
          return key.state;
        }
      }
    }
  

  return 0;
      
}


uint8_t dis_num(uint8_t num, uint8_t count, uint8_t dir)
{
  uint8_t bai, shi, ge;
  
  bai = num/100;
  shi = num%100/10;
  ge = num%10;
  /* 减 */
  if (dir == 0)
  {
    if (count == 1)
    {
      if (ge == 0)
      {
        if (bai == 2 && shi == 5)
        {
          ge = 5;
        }
        else 
        {
          ge = 9;
        }
        
      }
      else
      {
        ge -= 1;
      }
    }
    else if (count == 10)
    {
      if (shi == 0)
      {
        if (bai == 2)
        {
          shi = 5;
        }
        else
        {
          shi = 9;
        }
        
      }
      else
      {
        shi -= 1;
      }
    }
    else if (count == 100)
    {
      if (bai == 0)
      {
        bai = 2;
      }
      else
      {
        bai -= 1;
      }
    }
  }     
  else
  {     /* 加 */
    if (count == 1)
    {
      if (ge == 9)
      {
        ge = 0;
      }
      else
      {
        if (shi == 5&&bai ==2&&ge==5)
        {
          ge = 0;
        }
        else
        {
          ge += 1;
        }
      }
    }
    else if (count == 10)
    {
      if (shi == 9)
      {
        shi = 0;
      }
      else
      {
        if (bai == 2 && shi == 5)
        {
          shi = 0;
        }
        else 
        {
          shi += 1;
        }
      }
    }
    else if (count == 100)
    {
      if (bai == 2)
      {
        bai = 0;
      }
      else
      {
        bai += 1;
      }
    }
  }
  
  
  return (bai*100+shi*10+ge);
}



/* 显示机号 */
void dis_inf(uint8_t inf_data)
{
  dis_buff[0] = 0xff;
  dis_buff[1] = 0xff;
  dis_buff[2] = 0xff;
  dis_buff[3] = 0xff;
  dis_buff[4] = 0xff;
  
  dis_buff[5] = LED_0F[inf_data/100];
  dis_buff[6] = LED_0F[inf_data%100/10];
  dis_buff[7] = LED_0F[inf_data%10];
  
}



void display_dis(uint8_t key)
{
  static uint8_t bit_count = 0;
  static uint16_t blink_timer = 0;
  uint8_t num_buff[3] = {1, 10, 100};
  
  
    if (key == (KEY_VALUE_KEY1|KEY_VALUE_LONGPRESS))
    {
      if (sys_mode == NORM_MODE)
      {
        sys_mode = SET_MODE;
        dis_inf(own_addr);
      }
      else if (sys_mode == SET_MODE)
      {
        sys_mode = NORM_MODE;
        dis_inf(own_addr);
        inf_write(own_addr);
      }
    }
  
  if (sys_mode == SET_MODE)
  {
    if ((blink_timer ++/200)%2)
    {
      dis_buff[7-bit_count] = 0xff;
    }
    else 
    {
      dis_inf(own_addr);
    }
    if (key&KEY_VALUE_KEY1)
    {
      bit_count ++;
      if (bit_count == 3)
      {
        bit_count = 0;
      }
    }
  
    if (key&KEY_VALUE_KEY2)
    {
      own_addr = dis_num(own_addr, num_buff[bit_count], 0);
      
            
    }
    if (key&KEY_VALUE_KEY3)
    {
      own_addr = dis_num(own_addr, num_buff[bit_count], 1);
    }
    
  
  }
  else if(sys_mode == NORM_MODE)
  {
    
  }
}




