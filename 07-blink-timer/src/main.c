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
  //FLASH->ACR |= FLASH_ACR_PRFTEN;
  //FLASH->ACR |= FLASH_ACR_LATENCY; // Если 48< SystemCoreClock <= 72, пропускать 2 такта.

  //PWR->CR |= PWR_CR_VOS_0;
  //PWR->CR &=~PWR_CR_VOS_1; 
  //FLASH->ACR &= ~FLASH_ACR_LATENCY); // Предочистка.
  //FLASH->ACR |= FLASH_ACR_LATENCY_0; // Если SystemCoreClock <= 24 МГц, без пропусков.
  //FLASH->ACR |= FLASH_ACR_LATENCY_1; // Если 24< SystemCoreClock <= 48, пропускать 1 такт.

  RCC_HSICmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
  RCC_HCLKConfig(RCC_SYSCLKSource_HSE);
  //  *(__IO uint32_t *) RCC_CR_HSEON = (uint32_t)DISABLE; 
  //while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
  //RCC_PLLCmd(DISABLE);
  //
  /* RCC->CR|=~RCC_CR_PLLON; */
  /* RCC_PLLConfig(RCC_PLLSource_HSI,RCC_PLLMul_3,RCC_PLLDiv_2); */
  /* //RCC_PLLCmd(ENABLE); */
  /* RCC->CR|=RCC_CR_PLLON; // Включить генератор HSE. */
  /* while (!(RCC->CR & RCC_CR_PLLRDY)) {}; // Ожидание готовности HSE. */
  
  //  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);

  /* RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1. */
  /* RCC->CFGR |= RCC_CFGR_SW_HSE; // Выбрать HSE для тактирования SW0=1. */

  RCC->CR|=RCC_CR_HSEON; // Включить генератор HSE.
  while (!(RCC->CR & RCC_CR_HSIRDY)) {}; // Ожидание готовности HSE.
  //RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1.
  //RCC->CFGR |= RCC_CFGR_SW_HSI; // Выбрать HSE для тактирования SW0=1.

  RCC->CR|=RCC_CR_CSSON; 

  //while (!(RCC->CR & RCC_CR_HSERDY)) {}; // Ожидание готовности HSE.

  /* RCC->CR   &=~RCC_CR_PLLON; // Остановить PLL. */
  /* while ((RCC->CR & RCC_CR_PLLRDY)!=0) {} // Ожидание готовности PLL. */
  /* //RCC->CFGR &=~(RCC_CFGR_PPRE1_DIV1); // Предочистка делителя HSE. */
  /* //RCC->CFGR |= RCC_CFGR_PPRE1_DIV1; // Делить частоту HSE на 4. */
  /* RCC->CFGR &=~((RCC_CFGR_PLLSRC|RCC_CFGR_PLLMUL|RCC_CFGR_PLLDIV)); // Предочистка. */
  /* RCC->CFGR |= RCC_CFGR_PLLSRC_HSI; // Тактировать PLL от HSE/PREDIV1. */
  /* RCC->CFGR |= RCC_CFGR_PLLMUL6; // Умножать частоту на PLL_MUL. */
  /* RCC->CR   |= RCC_CR_PLLON; // Запустить PLL. */
  /* while ((RCC->CR & RCC_CR_PLLRDY)==0) {} // Ожидание готовности PLL. */
  //RCC->CFGR &=~RCC_CFGR_SW; // Очистить биты SW0, SW1.
  //RCC->CFGR |= RCC_CFGR_SW_PLL; // Тактирование с выхода PLL.
  //while ((RCC->CFGR&(uint32_t)RCC_CFGR_SWS)!=(uint32_t)RCC_CFGR_SWS_PLL) {} // Ожидание переключения на PLL.
  
  //RCC_HCLKConfig(RCC_SYSCLK_Div2); //set sysclk divider
  //RCC_DeInit();
  //RCC_PLLCmd(DISABLE);
  //RCC_PLLConfig(RCC_PLLSource_HSI,RCC_PLLMul_4,RCC_PLLDiv_3);
  //RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI, RCC_PLLMul_2); //configure pll / divider. _x=[2..16]
  //RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  //RCC_PLLCmd(ENABLE);
  //while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
  /* Set HSI as sys clock*/

  RCC_PCLK1Config(RCC_HCLK_Div1); 
  init_leds();
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
  
  
  init_timer();
  //RCC->CR|=RCC_CR_HSEON; // Включить генератор HSE.

  do
  {
    __NOP();
    //GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_6));
    //delay(1000000);
  }
  while(1);
  //__NOP(); while (1);
}

void NMI_Handler(void)  // Обработчик NMI вызывается при сбое HSE.
{
  /* Сбросить флаг системы контроля CSS */
  if (RCC->CIR & RCC_CIR_CSSF) RCC->CIR|=RCC_CIR_CSSC;
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_7));
}


/* void init_adc() */
/* { */
/*   ADC_InitTypeDef ADC_InitStruct; */
  
/*   RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE); */

  
/*   ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b; */
/*   ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; */

/*   ADC_Init(ADC1, &ADC_InitStruct); */

/*   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; */
/*   ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode; */
/*   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; */
/*   ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot; */
/*   ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0; */

/*   ADC_CommonInit(ADC1, &ADC_CommonInitStructure); */

/*   ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_16Cycles); */
  
/*   ADC_Cmd(ADC1, ENABLE); */
/* } */


/* void init_dma() */
/* { */
/*   DMA_InitTypeDef DMA_InitStructure; */

/*   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); */

/*   DMA_DeInit(DMA1_Channel5); */
  
/*   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); */
/*   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Result; */
/*   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; */
/*   DMA_InitStructure.DMA_BufferSize = 1; */
/*   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; */
/*   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; */
  
/*   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; */
/*   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; */
/*   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; */
/*   DMA_InitStructure.DMA_Priority = DMA_Priority_High; */
/*   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; */
/*   DMA_Init(DMA1_Channel1, &DMA_InitStructure); */
  
/*   DMA_Cmd(DMA1_Channel1, ENABLE); */
/* } */

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

  TIM_BaseInitStructure.TIM_Prescaler = (1600000/1000) - 1;
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
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_6));
}


