
#ifndef __DIS_H
#define __DIS_H



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







#endif
