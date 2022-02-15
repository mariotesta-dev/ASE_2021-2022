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

extern int result;
int shiftAmount = 1;

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
		
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	//display 32 bit on LEDS, 8 bits at a time
	//I used a shift amount variable and depending on its value, the result value gets "ANDed" and then eventually shifted
	
	switch(shiftAmount){
		case 1:
			LED_Out(result & 0x000000ff);
			shiftAmount++;
			break;
		case 2:
			LED_Out(result & 0x0000ff00 >> 8);
			shiftAmount++;
			break;
		case 3:
			LED_Out(result & 0x00ff0000 >> 16);
			shiftAmount++;
			break;
		case 4:
			LED_Out(result & 0xff000000 >> 24);
			shiftAmount++;
			break;
		case 5:
			//initialize the program again from PT.1
			reset_timer(1);
			disable_timer(1);
			LED_Out(0);
			//stops timer 3 as well
			reset_timer(3);
			disable_timer(3);
			
			break;
		
	}
	
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
