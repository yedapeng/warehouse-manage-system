


#include "stm8s.h"
#include "dis.h"







uint8_t dis_buff[8] = {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0};


unsigned char  LED_0F[] = 
{
  // 0123456789AbCdEF    -
  0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};






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