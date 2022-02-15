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


extern int max;
extern int i;
int k = 0;
int j = 0;
int value = 0;
extern int positions[];

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
		switch(k){
		case 0:
			value = (max & 0xff000000) >> 24;
			LED_Out(value);
			k++;
			break;
		case 1:
			value = (max & 0x00ff0000) >> 16;
			LED_Out(value);
			k++;
			break;
		case 2:
			value = (max & 0x0000ff00) >> 8;
			LED_Out(value);
			k++;
			break;
		case 3:
			value = (max & 0x000000ff);
			LED_Out(value);
			k++;
			break;
		case 4:
			LED_Out(0);	//turn OFF all LEDs
			reset_timer(2);		//reset and disable this timer (timer 2)
			disable_timer(2);
			k = 0;							//reset k
			i = 0;
			max = 0;
		
			for(j = 0; j < 4; j++){
				positions[j] = 0; //reset x1,y1,x2,y2
			}
			break;
			
		default:
				break;
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
