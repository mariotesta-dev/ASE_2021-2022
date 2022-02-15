/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int find_best_3 (int DATA_IN[], int N, uint8_t BEST_3[]);

extern int DATA_IN[];
extern uint8_t BEST_3[];
extern int N;

int result = 0;

//Buttons variable
int key2=0;
int key1=0;
int int0=0;

void RIT_IRQHandler (void)
{					
	
	/**** BUTTONS CONTROL ****/
	//KEY1
		if(key1 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0)){	/* KEY1 pressed */				
			switch(key1){
				case 2:
					result = find_best_3(DATA_IN, N, BEST_3);	//result will get the value of N in return
					enable_timer(0);
					break;
				default:
					break;
			}
			key1++;
		}
		else {	/* button released */
			key1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	else { //gestione BOUNCING KEY1
		if(key1 == 1)
			key1++;
	}
  
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
