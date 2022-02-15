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
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../TouchPanel/TouchPanel.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int search_in_pool(unsigned int VAR);

//Buttons variable
int key2=0;
int key1=0;
int int0=0;

int flag = 0;
unsigned int VAR = 0;
extern int search_in_pool (unsigned int VAR);
int result = 0;

void RIT_IRQHandler (void)
{					
	
	//KEY2
		if(key2 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0)){	/* KEY2 pressed */				
			switch(key2){
				case 2:
					if(flag == 0){
						enable_timer(1); //first pressure, enables timer 1
						flag = 1;
					}else if(flag == 1){
						disable_timer(1); //second pressure, stops timer and capture current value into VAR
						VAR = LPC_TIM1->TC; 
						result = search_in_pool(VAR); //then launches the assembly function
						enable_timer(3);							//enables timer 3 to show result on LEDs
					}

				
					break;
				default:
					break;
			}
			key2++;
		}
		else {	/* button released */
			key2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	else { //gestione BOUNCING KEY2
		if(key2 == 1)
			key2++;
	}


	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
