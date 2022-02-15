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

#define N 5

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

int j = 0;
extern unsigned char VETT[N];
extern int i;
int k = 0;

void TIMER0_IRQHandler (void)
{

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	
	if(j < N){
		LED_Out(VETT[j]); 	//mostro i led in sequenza
	}else if (j==5){
		LED_Out(0);					//seguiti da tutti LED spenti
		i = 0;							//resetto i, posso ripartire dal punto 2
		j = 0;
		
		for(k = 0; k < N; k++){
			VETT[k] = 0;
		}
		
		reset_timer(1);
		disable_timer(1);		//resetto e stoppo il timer 1
		NVIC_EnableIRQ(EINT1_IRQn); //enable clicking key1
	}
	
	j++;
	
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
