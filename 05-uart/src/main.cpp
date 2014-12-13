/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdint.h> */
/* #include <math.h> */
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_adc.h"
#include "misc.h"

// USART Receiver buffer
#define RX_BUFFER_SIZE 350
volatile uint8_t  rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_wr_index=0,rx_rd_index=0;
volatile uint16_t rx_counter=0;
volatile uint8_t rx_buffer_overflow=0;

// USART Transmitter buffer
#define TX_BUFFER_SIZE 350
volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
volatile uint16_t tx_wr_index=0,tx_rd_index=0;
volatile uint16_t tx_counter=0;

void USART_init(void);
uint8_t get_char(void);
void put_char(uint8_t);
void put_str(unsigned char *s);
void put_int(int32_t data);
void Delay(__IO uint32_t nCount);
void ADC_init(void);
uint16_t get_adc_value(void);

int main()
{
  const unsigned char BUFFER_ADC_LEN = 10;
  unsigned char adc_buffer[BUFFER_ADC_LEN];
  unsigned char ta=0;
  unsigned char i;
  unsigned short adc_sum;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_HSICmd(ENABLE);
  while (!RCC_GetFlagStatus(RCC_FLAG_HSIRDY));
  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  USART_init();
  put_str((unsigned char *)"Hello!\r\n");
  ADC_init();
  while(1)
  {
    ADC_SoftwareStartConv(ADC1);
    ta++;if(ta==BUFFER_ADC_LEN) ta=0;
    adc_buffer[ta]=ADC_GetConversionValue(ADC1);
    adc_sum=0;
    for(i=0;i<BUFFER_ADC_LEN;i++)
    {
      adc_sum += adc_buffer[i];
    }
    put_int(adc_sum/BUFFER_ADC_LEN);
    put_str((unsigned char *)"\r\n");
  }
}

void Delay(__IO uint32_t nCount) {
  while(nCount--){}
}

void ADC_init(void)
{
  
  ADC_InitTypeDef     ADC_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);

  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;

  ADC_Init(ADC1, &ADC_InitStruct);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_16Cycles);
  
  //ADC_Cmd(ADC1, ENABLE);

  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET);
  put_str((unsigned char *)"ADC initialized successful\r\n");

  
}
  
void USART_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //PA9 to TX USART1
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //PA10 to RX USART1
  // PB9 -> TX UART.
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //PB7  -> RX UART.
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  /* NVIC configuration */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
      if ((USART1->SR & (USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE|USART_FLAG_ORE)) == 0)
	{
	  rx_buffer[rx_wr_index++]=(uint8_t)(USART_ReceiveData(USART1)& 0xFF);
	  if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
	  if (++rx_counter == RX_BUFFER_SIZE)
	    {
	      rx_counter=0;
	      rx_buffer_overflow=1;
	    }
	}
      else USART_ReceiveData(USART1);//ex=0;
    }
  else
    {
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }
}

uint16_t get_adc_value(void)
{
  //  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  //while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  //return ADC_GetConversionValue(ADC1);
}

uint8_t get_char(void)
{
  uint8_t data;
  while (rx_counter==0);
    data=rx_buffer[rx_rd_index++];
  if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
  --rx_counter;
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  return data;
}

void put_char(uint8_t c)
{
  while (tx_counter == TX_BUFFER_SIZE);
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  if (tx_counter || (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET))
    {
      tx_buffer[tx_wr_index++]=c;
      if (tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
      ++tx_counter;
      USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
  else
    USART_SendData(USART1,c);
}

void put_str(unsigned char *s)
{
  while (*s != 0)
    put_char(*s++);
}


void put_int(int32_t data)
{
  unsigned char temp[10],count=0;
  if (data<0)
    {
      data=-data;
      put_char('-');
    }

  if (data)
    {
      while (data)
	{
	  temp[count++]=data%10+'0';
	  data/=10;
	}

      while (count)
	{
	  put_char(temp[--count]);
	}
    }
  else put_char('0');

}
 
