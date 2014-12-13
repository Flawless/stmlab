/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <stdint.h> */
/* #include <math.h> */
#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
// #include "stm32f4xx_conf.h"
// #include "utils.h"

void Delay(__IO uint32_t nCount);
//void _init();

int main(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  while (1) {
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    Delay(500000);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    Delay(500000);
  }


  return 0;
}

void Delay(__IO uint32_t nCount) {
  while(nCount--){}
}

/*
 * Dummy function to avoid compiler error
 */
//void _init() {
//
//}

