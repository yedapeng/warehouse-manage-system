/**
  ******************************************************************************
  * @file     TIM4_TimeBase\main.c
  * @author   MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief    This file contains the main function for TIM4 Time Base Configuration example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "dis.h"
//#include "stm8s_eval.h"

/**
  * @addtogroup TIM4_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM4_PERIOD       255
#define UART_BPS          9600    
    
    
#define TX_BUFFER_SIZE          32
__IO uint8_t TxBuffer[TX_BUFFER_SIZE];
__IO uint8_t TxCounter = 0;

 
__IO uint8_t RxBuffer[RX_BUFFER_SIZE];
__IO uint8_t RxCounter = 0;
__IO uint8_t rx_over_count = 0;
__IO uint8_t rx_flag = 0;

typedef struct
{
  uint8_t rx_len;
  __IO uint8_t *rx_index;
}rx_t;
    
rx_t uart_rx;
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static void CLK_Config(void);
static void TIM4_Config(void);
static void GPIO_Config(void);
static void UART1_Config(void);




void uart_send()
{
  TxBuffer[0] = '0';
  TxBuffer[1] = '1';
  TxBuffer[2] = '2';
  TxBuffer[3] = '3';
  TxCounter = 4;
  UART1_ITConfig(UART1_IT_TXE, ENABLE);
}

/* 处理串口接收到的数据 */
void dis_uart_data(rx_t *rx_data)
{
  
}




void rs485_tx(void)
{ 
  GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
  GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
}

void rs485_rx(void)
{ 
  GPIO_WriteLow(GPIOA, GPIO_PIN_1);
  GPIO_WriteLow(GPIOA, GPIO_PIN_2);
}

#define         ADDR_ADDR               0x4000

void inf_write(uint8_t addr)
{
    /* Define FLASH programming time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH_ProgramByte(ADDR_ADDR, addr);
}

uint8_t inf_read(void)
{
  return FLASH_ReadByte(ADDR_ADDR);
}


/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

  
  /* Clock configuration -----------------------------------------*/
  CLK_Config();  

  /* GPIO configuration -----------------------------------------*/
  GPIO_Config();  

  /* TIM4 configuration -----------------------------------------*/
  TIM4_Config();  
  
  own_addr = inf_read();

  UART1_Config();  
  
  while (1)
  {
    
    
    if (rx_flag)
    {
      rx_flag = 0;
      dis_uart_data(&uart_rx);
      
      
      
      
    }
  }
}




/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}


/**
  * @brief  Configure UART1 for the communication with HyperTerminal
  * @param  None
  * @retval None
  */
static void UART1_Config(void)
{
  /* EVAL COM (UART) configuration -----------------------------------------*/
  /* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Odd parity
        - Receive and transmit enabled
        - UART Clock disabled
  */
  UART1_Init((uint32_t)UART_BPS, UART1_WORDLENGTH_8D,UART1_STOPBITS_1, UART1_PARITY_NO,
                   UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Enable the UART Receive interrupt: this interrupt is generated when the UART
    receive data register is not empty */
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  
  /* Enable the UART Transmit complete interrupt: this interrupt is generated 
     when the UART transmit Shift Register is empty */
  UART1_ITConfig(UART1_IT_TXE, ENABLE);

  /* Enable UART */
  UART1_Cmd(ENABLE);
  
    /* Enable general interrupts */
  enableInterrupts();
}


/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  //enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

/**
  * @brief  Configure GPIO for LEDs available on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* Initialize LEDs mounted on STM8-128 EVAL board */
  GPIO_Init(SCLK_GPIO_Port, SCLK_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(RCLK_GPIO_Port, RCLK_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(DAT_GPIO_Port, DAT_Pin, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
  
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);

  rs485_rx();
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}


/* 
  定时器回调函数
  定时器频率为500Hz

 */
void timer_handle(void)
{
  uint8_t key;
  LED4_Display();
  TimingDelay_Decrement();
  
  key = key_read();
  
  display_dis(key);
  if (RxCounter)
  {
    rx_over_count ++;
    if (rx_over_count > 5)
    {
      rx_flag = 1;
      rx_over_count = 0;
      
      uart_rx.rx_index = RxBuffer;
      uart_rx.rx_len = RxCounter;
      RxCounter = 0;
    }
  }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
