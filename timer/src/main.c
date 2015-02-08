#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_tim.h"

void init_timer();
void init_leds();
void delay(uint32_t nCount);

void delay(__IO uint32_t nCount) {

  while (nCount--) {

  }

}

int main(void) {
  RCC_PCLK1Config(RCC_HCLK_Div1); 
  init_leds();
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
  
  
  init_timer();

  do
  {
    __NOP();
  }
  while(1);
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
  GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) ^ (GPIO_Pin_7) ^ (GPIO_Pin_6));
}
