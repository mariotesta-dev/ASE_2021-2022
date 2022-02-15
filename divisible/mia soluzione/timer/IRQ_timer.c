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

extern int resetTimer;
extern int e1;
extern int e2;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int divisible(int e1, int e2); 
int module = 0;

void TIMER0_IRQHandler (void)
{
	//se interrupt è attivo per il timer, non appena verrà messo a 1 --> andrà nell'handler del timer
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	//when timer reaches 6.541s the interrupt is send and the following code executed...
	
	resetTimer = 2; //setting this to 2 prevents KEY1 and KEY2 to update e1 and e2 values
	
	LED_Out_4_Lower(e1);
	
	module = divisible(e1, e2);
	
	LED_Out_4_Higher(module);
		
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
