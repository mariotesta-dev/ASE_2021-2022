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

#define N 5
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int sort_vector(int VETT[], unsigned int n);

int VETT[N] = {2,3,5,-2,0};

//Buttons variable
int key2=0;
int key1=0;
int int0=0;

//Joystick variables
int select=0;
int down=0;
int left=0;
int right=0;
int up=0;

void RIT_IRQHandler (void)
{					
	
	/**** JOYSTICK CONTROL ****/
	
	//il joystick è già antirimbalzo per hardware
	// catturare valore del timer: val_int = LPC_TIM1->TC;

 if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	/* Joytick Select pressed */
		select++;
		switch(select){
			case 1:
					//YOUR CODE HERE
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}	
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	/* Joytick down pressed */
		down++;
		switch(down){
			case 1:
				//YOUR CODE HERE
				break;
			default:
				break;
		}
	}
	else{		
			down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joytick left pressed */
		left++;
		switch(left){
			case 1:
				//YOUR CODE HERE
				break;
			default:
				break;
		}
	}
	else{		
			left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	/* Joytick right pressed */
		right++;
		switch(right){
			case 1:
				//YOUR CODE HERE
				break;
			default:
				break;
		}
	}
	else{		
			right=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	/* Joytick up pressed */
		up++;
		switch(up){
			case 1:
					//YOUR CODE HERE
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
	
	/**** BUTTONS CONTROL ****/
	//KEY1
		if(key1 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0)){	/* KEY1 pressed */				
			switch(key1){
				case 2:
					//YOUR CODE HERE
					sort_vector(VETT,N);
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

	
	
	//KEY2
		if(key2 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0)){	/* KEY2 pressed */				
			switch(key2){
				case 2:
					
					//YOUR CODE HERE

				
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

	
	
	//INT0
	if(int0 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */				
			switch(int0){
				case 2:
					//YOUR CODE HERE
				
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
	else { //gestione BOUNCING INT0
		if (int0 == 1)
			int0++;
	}
  
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
