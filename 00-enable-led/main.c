/**
 * Main code of firmware
 */
#include "stm32f10x.h"
/* General defines */
#define LED_PORT GPIOC
#define LED_BLUE (1 << 8)
#define LED_GREEN (1 << 9)
void main();
/* provided by linker script */
extern unsigned long _estack;
/* Reset handler */
void Reset_Handler(void) {
  main();
}
/* Table of Cortex vectors */
void *vector_table[] __attribute__ ((section(".vectors"))) = {
  &_estack,
  Reset_Handler,
};
/* Main function */
void main(void) {
  /* setup i/o port */
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  LED_PORT->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_0;
  LED_PORT->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9);
  /* enable led */
  LED_PORT->ODR = LED_BLUE | LED_GREEN;
  /* infinity loop */
  while (1);
}
