/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../GLCD/GLCD.h"

#define N 8



/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int i = 2;
extern int count;
extern unsigned int VAR1;
extern unsigned int count_bits_to_1 (unsigned int VAR1);

void TIMER0_IRQHandler (void)
{
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	
	if(i%2==0){
		LED_Out(255);
	}else{
		LED_Out(0);
	}
	
	i++;

	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	//if down is pressed for more than 3s, it enters here and execute a function
	
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	
	count = count_bits_to_1(VAR1);
  //returns the number of bits setup to value 1 in the VAR1 value
	
	NVIC_EnableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
	
	if(count >= 0 && count <= 3){
		LED_Out(0);
	}
	
	if(count >= 4 && count <= 11){
		LED_Out(count);
	}
	if(count >= 12 && count <= 31){
		enable_timer(1);
	}
		
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
