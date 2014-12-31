/*******************************************************************************
    gpio_emcu.h - ������������ ���� ������ GPIO.
    Author: eugenemcu.ru. Date: 16.11.2011.
    �������� ������� �������� �������� � GPIO STM32.

*******************************************************************************/
#ifndef gpio_emcu_H
#define gpio_emcu_H

#include "stm32F10x.h"
/*      ������� ������ GPIO     */
#define PORTA   ((u32*)(APB2PERIPH_BASE + 0x0800))
#define PORTB   ((u32*)(APB2PERIPH_BASE + 0x0C00))
#define PORTC   ((u32*)(APB2PERIPH_BASE + 0x1000))
#define PORTD   ((u32*)(APB2PERIPH_BASE + 0x1400))
#define PORTE   ((u32*)(APB2PERIPH_BASE + 0x1800))
#define PORTF   ((u32*)(APB2PERIPH_BASE + 0x1C00))
#define PORTG   ((u32*)(APB2PERIPH_BASE + 0x2000))

/*------------------------------------------------------------------------------
    ������� ��� GPIO.
    ����� �������� �� ������� ������� ��������� �:
    (port) + 0 = GPIOx_CRL, + 1 = GPIOx_CRH, + 2 = GPIOx_IDR, + 3 = GPIOx_ODR
------------------------------------------------------------------------------*/

// Set pin - ��������� ����� ����� �� �����. ��������: spin_m (PORTC,(1<<4)|((1<<5)); // ���������� PC4, PC5.
#define spin_m(port,mask) (*(port+4)=(u32)(mask))

// Clear pin - ����� ����� ����� �� �����. ��������: �pin_m (PORT�,(1<<0)|((1<<1)); // �������� PB0, PB1.
#define cpin_m(port,mask) (*(port+4)=(u32)((mask)<<16))

// Invert pin - �������� ������ �����. ��������: ipin_m (PORTC,((1<<5)); // �������� ��������� PC5.
#define ipin_m(port,mask) (*(port+3)^=(u32)(mask)) 

// Write port - ������ ��������� �������� ����� ODR. ��������: wr_port(PORTC, 0xFFFF); // �������� 0xFFFF � GPIOC->ODR.
#define wr_port(port,word) (*(port+3)=(u32)(word))

// Read port - ������ �������� �������� ����� IDR. ��������: tmp=rd_port(PORTA); // ������� GPIOA->IDR � tmp.
#define rd_port(port) (*(port+2))

// Get pin - �������� ������ �����. ��������: if (getpin_b(PORTA, 0)) // ���� ����� PA.0 ����������...
#define getpin_b(port,bit) (*(port+2)&(1<<(bit)))



// Output Push Pull - ��������� ������ b �� ����������� ����� (MODE=11, CNF=00). ��������: pinOutPP_b (PORTC,8); 
#define pinOutPP_b(port,bit)  {*(port+((bit)/8))|=3UL<<(((bit)-(8*((bit)/8)))<<2); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2));}

// Output Open Drain - ����� b - �� ����� c �������� ������ (MODE=11, CNF=01). ��������: pinOutOD_b (PORTA,0); 
#define pinOutOD_b(port,bit)  {*(port+((bit)/8))|=3UL<<(((bit)-(8*((bit)/8)))<<2); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(1UL<<((((bit)-(8*((bit)/8)))<<2)+2));}

// Output Alternative Function - ������ b - ����������� ����� c �������������� �������� (MODE=11, CNF=10). ��������: pinOutAF_b (PORTB,14); 
#define pinOutAF_b(port,bit)  {*(port+((bit)/8))|=3UL<<(((bit)-(8*((bit)/8)))<<2); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(2UL<<((((bit)-(8*((bit)/8)))<<2)+2));}

// Output Alternative Function & Open Drain - ������ b - C �������� ������ � �������������� �������� (MODE=11, CNF=11). ��������: pinOutAFOD_b (PORTB,14); 
#define pinOutAFOD_b(port,bit)  {*(port+((bit)/8))|=3UL<<(((bit)-(8*((bit)/8)))<<2); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(3UL<<((((bit)-(8*((bit)/8)))<<2)+2));}

// Input Z-state - ����� b - ���� � ������� Z-��������� (MODE=00, CNF=01). pinOutZ (PORTA,0); // ����� PA0 �� ����. 
#define pinInputZ_b(port,bit)  {*(port+((bit)/8))&=~(3UL<<(((bit)-(8*((bit)/8)))<<2)); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(1UL<<((((bit)-(8*((bit)/8)))<<2)+2)); }

// Input Pull Up - ����� b - ���� c ���������� �� ���� ������� (MODE=00, CNF=10, ODR=1). pinInputPU_b (PORTD,12); // ����� PD12 - PullUp ����. 
#define pinInputPU_b(port,bit)  {*(port+((bit)/8))&=~(3UL<<(((bit)-(8*((bit)/8)))<<2)); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(2UL<<((((bit)-(8*((bit)/8)))<<2)+2)); (*(port+3)|=(1<<(bit)));}

// Input Pull Down - ����� b - ���� c ���������� �� ����� (MODE=00, CNF=10, ODR=0). pinInputPD_b (PORTD,5); // ����� PD5 - PullDown ����. 
#define pinInputPD_b(port,bit)  {*(port+((bit)/8))&=~(3UL<<(((bit)-(8*((bit)/8)))<<2)); *(port+((bit)/8))&=~(3UL<<((((bit)-(8*((bit)/8)))<<2)+2)); *(port+((bit)/8))|=(2UL<<((((bit)-(8*((bit)/8)))<<2)+2)); (*(port+3)&=~(1<<(bit)));}

#endif