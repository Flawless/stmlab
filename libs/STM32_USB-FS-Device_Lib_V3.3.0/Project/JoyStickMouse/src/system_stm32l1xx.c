/**
  ******************************************************************************
  * @file    system_stm32l1xx.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    21-March-2011
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  *          This file contains the system clock configuration for STM32L1xx Ultra
  *          Low Medium-density devices, and is generated by the clock configuration
  *          tool "STM32L1xx_Clock_Configuration_V1.0.0.xls".
  *             
  * 1.  This file provides two functions and one global variable to be called from 
  *     user application:
  *      - SystemInit(): Setups the system clock (System clock source, PLL Multiplier
  *                      and Divider factors, AHB/APBx prescalers and Flash settings),
  *                      depending on the configuration made in the clock xls tool. 
  *                      This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l1xx_md.s" file.
  *                        
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.   
  *      
  * 2. After each device reset the MSI (2.1 MHz Range) is used as system clock source.
  *    Then SystemInit() function is called, in "startup_stm32l1xx_md.s" file, to
  *    configure the system clock before to branch to main program.    
  *    
  * 3. If the system clock source selected by user fails to startup, the SystemInit()
  *    function will do nothing and MSI still used as system clock source. User can 
  *    add some code to deal with this issue inside the SetSysClock() function.       
  * 
  * 4. The default value of HSE crystal is set to 8MHz, refer to "HSE_VALUE" define
  *    in "stm32l1xx.h" file. When HSE is used as system clock source, directly or
  *    through PLL, and you are using different crystal you have to adapt the HSE
  *    value to your own configuration.
  * 
  * 5. This file configures the system clock as follows:  
  *=============================================================================
  *                         System Clock Configuration
  *=============================================================================
  *        System Clock source          | PLL(HSE)
  *----------------------------------------------------------------------------- 
  *        SYSCLK                       | 32000000 Hz
  *----------------------------------------------------------------------------- 
  *        HCLK                         | 32000000 Hz
  *----------------------------------------------------------------------------- 
  *        AHB Prescaler                | 1
  *----------------------------------------------------------------------------- 
  *        APB1 Prescaler               | 1
  *----------------------------------------------------------------------------- 
  *        APB2 Prescaler               | 1
  *----------------------------------------------------------------------------- 
  *        HSE Frequency                | 8000000 Hz
  *----------------------------------------------------------------------------- 
  *        PLL DIV                      | 3
  *----------------------------------------------------------------------------- 
  *        PLL MUL                      | 12
  *----------------------------------------------------------------------------- 
  *        VDD                          | 3.3 V
  *----------------------------------------------------------------------------- 
  *        Vcore                        | 1.8 V (Range 1)
  *----------------------------------------------------------------------------- 
  *        Flash Latency                | 1 WS
  *----------------------------------------------------------------------------- 
  *        Require 48MHz for USB clock  | Enabled
  *----------------------------------------------------------------------------- 
  *=============================================================================
  ****************************************************************************** 
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l1xx_system
  * @{
  */  
  
/** @addtogroup STM32L1xx_System_Private_Includes
  * @{
  */

#include "stm32l1xx.h"

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Defines
  * @{
  */
/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. 
                                  This value must be a multiple of 0x200. */
/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Variables
  * @{
  */
uint32_t SystemCoreClock    = 32000000;
__I uint8_t PLLMulTable[9] = {3, 4, 6, 8, 12, 16, 24, 32, 48};
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

/**
  * @}
  */

/** @addtogroup STM32L1xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
  /*!< Set MSION bit */
  RCC->CR |= (uint32_t)0x00000100;

  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
  RCC->CFGR &= (uint32_t)0x88FFC00C;
  
  /*!< Reset HSION, HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xEEFEFFFE;

  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /*!< Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
  RCC->CFGR &= (uint32_t)0xFF02FFFF;

  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;
    
  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is MSI, SystemCoreClock will contain the MSI 
  *             value as defined by the MSI range.
  *                                   
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32l1xx.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32l1xx.h file (default value
  *              8 MHz), user has to ensure that HSE_VALUE is same as the real
  *              frequency of the crystal used. Otherwise, this function may
  *              have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.  
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmul = 0, plldiv = 0, pllsource = 0, msirange = 0;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp)
  {
    case 0x00:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
    case 0x04:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x08:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x0C:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> 18)];
      plldiv = (plldiv >> 22) + 1;
      
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock selected as PLL clock entry */
        SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
      }
      else
      {
        /* HSE selected as PLL clock entry */
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
      }
      break;
    default: /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
}

/**
  * @brief  Configures the System clock frequency, AHB/APBx prescalers and Flash 
  *         settings.
  * @note   This function should be called only once the RCC clock configuration  
  *         is reset to the default reset state (done in SystemInit() function).             
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }
  
  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable 64-bit access */
    FLASH->ACR |= FLASH_ACR_ACC64;
    
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* Flash 1 wait state */
    FLASH->ACR |= FLASH_ACR_LATENCY;
    
    /* Power enable */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
    /* Select the Voltage Range 1 (1.8 V) */
    PWR->CR = PWR_CR_VOS_0;
  
    /* Wait Until the Voltage Regulator is ready */
    while((PWR->CSR & PWR_CSR_VOSF) != RESET)
    {
    }
        
    /* HCLK = SYSCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
  
    /* PCLK2 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /*  PLL configuration */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
        
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  {
    /* If HSE fails to start-up, the application will have wrong clock
       configuration. User can add here some code to deal with this error */
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
