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


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


//extern unsigned int asm_function(unsigned int VAR1); <-- use it and put ASM_functions inside Timer Folder

extern int number_of_negatives;

void TIMER0_IRQHandler (void)
{
	//first iteration --> leds are off, timer 0 waits 0.9s then turns them on
	//from second iteration --> turns leds on when interrupt occurs, interrupt occurs after 0.9s it has been enabled by timer1 handler
	if(number_of_negatives == 0){
		LED_Out(128); //if value is 0, just blink the first led from the left -> 2^7 = 128
	}else{
		LED_Out_Custom(number_of_negatives); //if value is different than 0, a custom led out function shows the value from LD05 to LD11
	}
	
	disable_timer(0); //disable itself
	enable_timer(1); //enable timer of 1.3s... so 1.3s will pass before the leds are turned off
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	LED_Out(0); //shuts the leds on interrupt, so after 0.9s being ON
	
	disable_timer(1);	//same thing happening on timer 0, but with opposite timers
	enable_timer(0);
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
		
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
