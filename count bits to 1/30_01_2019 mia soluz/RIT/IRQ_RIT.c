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


unsigned int VAR1 = 9; //VAR1 initially set to 9

unsigned int count;

int key2=0;
int key1=0;
int int0=0;

int ended = 0;

int down=0;

void RIT_IRQHandler (void)
{					
	
	//il joystick è già antirimbalzo per hardware
	// catturare valore del timer: val_int = LPC_TIM1->TC;	

	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick down pressed */
		down++;
		switch(down){
			case 1:
				//prolonged pressure of joystick down > 3s concludes number construction
				//at the very start enable timer 2 --> after 3s it sends an interrupt
				enable_timer(2);
			
				break;
			default:
				break;
				/*if(down == 0){
					break;
				}
				if(down > 1){
					//catch if any interrupt has occurred
					ended = LPC_TIM3 -> CR0;
					if(ended == 1){ //interrupt has occurred 
						LED_Out(1); //show 1 on the leds
					}else{
						break;
					}
				}*/
		}
	}
	else{
		//if joystick down is not pressed, disable and reset timer
		reset_timer(2);
		disable_timer(2);
			down=0;
	}
	
	//KEY1
		if(key1 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0)){	/* KEY1 pressed */				
			switch(key1){
				case 2:
				//YOUR CODE HERE
				
				
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

	/* button management */
		if(key2 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0)){	/* KEY2 pressed */				
			switch(key2){
				case 2:
					
				//current value of timer 3 needs to be captured into VAR1
					disable_timer(3);
					VAR1 = LPC_TIM3->TC;	
					enable_timer(3);

				
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

	if(int0 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */				
			switch(int0){
				case 2:
					//divides VAR1 by 4
				  VAR1 = VAR1/4;
				
					break;
				default:
					break;
			}
			int0++;
		}
		else {	/* button released */
			int0=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	else {
		if (int0 == 1)
			int0++;
	}
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
