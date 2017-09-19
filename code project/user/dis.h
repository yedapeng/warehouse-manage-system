
#ifndef __DIS_H
#define __DIS_H



#define RX_BUFFER_SIZE          32  


#define SCLK_GPIO_Port          GPIOC
#define SCLK_Pin                GPIO_PIN_6      
#define RCLK_GPIO_Port          GPIOC   
#define RCLK_Pin                GPIO_PIN_7
#define DAT_GPIO_Port           GPIOA   
#define DAT_Pin                 GPIO_PIN_3


#define SCLK_H          GPIO_WriteHigh(SCLK_GPIO_Port, SCLK_Pin)  
#define SCLK_L          GPIO_WriteLow(SCLK_GPIO_Port, SCLK_Pin)  

#define RCLK_H          GPIO_WriteHigh(RCLK_GPIO_Port, RCLK_Pin)  
#define RCLK_L          GPIO_WriteLow(RCLK_GPIO_Port, RCLK_Pin) 

#define DIO_H          GPIO_WriteHigh(DAT_GPIO_Port, DAT_Pin)  
#define DIO_L          GPIO_WriteLow(DAT_GPIO_Port, DAT_Pin) 



extern void LED4_Display (void);
extern uint8_t key_read (void);

#define KEY_VALUE_KEY1                  0x01
#define KEY_VALUE_KEY2                  0x02
#define KEY_VALUE_KEY3                  0x04
#define KEY_VALUE_LONGPRESS             0x80

#define NORM_MODE                       0
#define SET_MODE                        1


extern uint8_t own_addr;

extern void display_dis(uint8_t key);


#define COM_DATA_HEAD                   0xA55A
#define COM_DATA_LEN                    8

typedef struct 
{
  uint8_t  len;
  uint8_t  data[COM_DATA_LEN];
}body_t;

typedef struct 
{
  uint16_t head;
  uint8_t slave_addr;
  uint8_t point_addr;
  body_t  body;
  uint8_t  check_sum;
}com_data_t;




#endif
