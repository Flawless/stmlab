/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdint.h> */
/* #include <math.h> */
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_tim.h"
//#include "stm32l1xx_adc.h"
//#include "stm32l1xx_dma.h"
// #include "stm32f4xx_conf.h"
// #include "utils.h"

/* void Delay(__IO uint32_t nCount); */
//void _init();
void init_timer();
void init_leds();
void delay(uint32_t nCount);
//void init_adc();
//void init_dma();
//uint32_t ADC_Result;

void delay(__IO uint32_t nCount) {

  while (nCount--) {

  }

}

int main(void) {
  /* RCC_HSICmd(ENABLE); */
  /* while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); */
  /* RCC_HCLKConfig(RCC_SYSCLKSource_HSI); */

  /* RCC->CR|=RCC_CR_HSEON; // Включить генератор HSE. */
  /* while (!(RCC->CR & RCC_CR_HSIRDY)) {}; // Ожидание готовности HSE. */

  /* RCC->CR|=RCC_CR_CSSON;  */

  RCC_PCLK1Config(RCC_HCLK_Div1); 
  init_leds();
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  GPIO_SetBits(GPIOB, GPIO_Pin_7);
  
  
  init_timer();

  do
  {
    __NOP();
  }
  while(1);
}

void NMI_Handler(void)  // Обработчик NMI вызывается при сбое HSE.
{
  /* Сбросить флаг системы контроля CSS */
  if (RCC->CIR & RCC_CIR_CSSF) RCC->CIR|=RCC_CIR_CSSC;
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_7));
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_6));
}


void init_leds()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void init_timer()
{
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7EN, ENABLE);

  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  TIM_TimeBaseStructInit(&TIM_BaseInitStructure);

  TIM_BaseInitStructure.TIM_Prescaler = (1600000/1000) - 1;
  TIM_BaseInitStructure.TIM_Period = 1000;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_BaseInitStructure);
  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  /* TIM_BaseInitStructure.TIM_Prescaler = (160000/1000) - 1;   */
  /* TIM_BaseInitStructure.TIM_Period = 1000; */
  /* TIM_TimeBaseInit(TIM7, &TIM_BaseInitStructure); */

  /* TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); */

  TIM_Cmd(TIM6, ENABLE);
  NVIC_SetPriority(TIM6_IRQn, 1);
  NVIC_EnableIRQ(TIM6_IRQn);

  /* TIM_Cmd(TIM7, ENABLE); */
  /* NVIC_SetPriority(TIM7_IRQn, 2); */
  /* NVIC_EnableIRQ(TIM7_IRQn); */


  //Включем порт А
  RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
  //Включаем Таймер 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

  GPIO_InitTypeDef PORT;
  PORT.GPIO_Pin = (GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

  PORT.GPIO_Mode = GPIO_Mode_OUT;
  PORT.GPIO_Speed = GPIO_Speed_2MHz;
  PORT.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &PORT);
  
  TIM2->CCER |= (TIM_CCER_CC2E|TIM_CCER_CC3E|TIM_CCER_CC4E);

  TIM2->CCMR1|=(TIM_CCMR1_OC2M_0| TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
  TIM2->CCMR2|=(TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 |
		TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);

  TIM2->CR1 |= TIM_CR1_CEN;

  TIM2->CCR3=32768;
}

void TIM6_IRQHandler()
{
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_7));
}

void TIM7_IRQHandler()
{
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_6));
}


