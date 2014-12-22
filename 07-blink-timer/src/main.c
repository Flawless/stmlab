/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdint.h> */
/* #include <math.h> */
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_adc.h"
#include "stm32l1xx_dma.h"
// #include "stm32f4xx_conf.h"
// #include "utils.h"

/* void Delay(__IO uint32_t nCount); */
//void _init();
void init_timer();
void init_leds();
void init_adc();
void init_dma();
uint32_t ADC_Result;

int main(void) {
  RCC_HSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  //RCC_HCLKConfig(RCC_SYSCLK_Div1); //set sysclk divider
  //RCC_DeInit();
  //RCC_PLLCmd(DISABLE);
  //RCC_PLLConfig(RCC_PLLSource_HSI,RCC_PLLMul_4,RCC_PLLDiv_3);
  //RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI, RCC_PLLMul_2); //configure pll / divider. _x=[2..16]
  //RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  //RCC_PLLCmd(ENABLE);
  //while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* Set HSI as sys clock*/
  //  RCC_HCLKConfig(RCC_SYSCLK_Div512);
  
  RCC_PCLK1Config(RCC_HCLK_Div1); 
  init_leds();
  GPIO_SetBits(GPIOB, GPIO_Pin_7);
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);

  init_timer();

  do __NOP(); while (1);
}

void init_adc()
{
  ADC_InitTypeDef ADC_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);

  
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;

  ADC_Init(ADC1, &ADC_InitStruct);

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;

  ADC_CommonInit(ADC1, &ADC_CommonInitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_16Cycles);
  
  ADC_Cmd(ADC1, ENABLE);
}


void init_dma()
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel5);
  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Result;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  DMA_Cmd(DMA1_Channel1, ENABLE);
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

  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  TIM_TimeBaseStructInit(&TIM_BaseInitStructure);

  TIM_BaseInitStructure.TIM_Prescaler = (16000000/1000) - 1;
  TIM_BaseInitStructure.TIM_Period = 1000;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_BaseInitStructure);

  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM6, ENABLE);
  NVIC_SetPriority(TIM6_IRQn, 1);
  NVIC_EnableIRQ(TIM6_IRQn);
}

void TIM6_IRQHandler()
{
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_6 | GPIO_Pin_7));
}


