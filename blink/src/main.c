#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"

void Delay(__IO uint32_t nCount);

int main(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  while (1) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    Delay(100000);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    Delay(100000);
  }


  return 0;
}

void Delay(__IO uint32_t nCount) {
  while(nCount--){}
}
