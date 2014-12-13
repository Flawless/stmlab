#define GPIO_DIR_REG  0x40020800  /* FIO1DIR задает направление для блока GPIO 1 */
#define GPIO_REG_VAL  0x40020400  /* FIO1PIN задает значение для блока GPIO 1 */
#define GPIO_PIN_NO   (1<<58)     /* 18-й бит отвечает за 18-й пин */

void wait()
{
  volatile int i=0x20000;
  while(i>0) {
    --i;
  }
}

void main()
{
  *((volatile unsigned int *)GPIO_DIR_REG) = GPIO_PIN_NO;

  while(1) {
    *((volatile unsigned int *)GPIO_REG_VAL) = GPIO_PIN_NO;
    wait();
    *((volatile unsigned int *)GPIO_REG_VAL) = 0;
    wait();
  }

  /* main() *никогда* не должен вернуться! */
}
