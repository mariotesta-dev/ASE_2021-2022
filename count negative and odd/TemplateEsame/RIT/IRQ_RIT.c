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

#define N 3

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int count_negative_and_odd(int* VETT, unsigned int n);


//Buttons variable
int key1=0;
int key2=0;
int int0=0;

//Joystick variables
int select=0;
int down=0;
int up=0;

int ready = 0;

int VAR1; //signed integer
int VETT[N];
int i = 0;
int number_of_negatives = 0;

void RIT_IRQHandler (void)
{					
	
	/**** JOYSTICK CONTROL ****/
	
	//il joystick è già antirimbalzo per hardware
	// catturare valore del timer: val_int = LPC_TIM1->TC;

 if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	/* Joytick Select pressed */
		select++;
		switch(select){
			case 1:
					//since i cannot try select on the emulator, i replaced it with KEY1
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
				if(ready==0){
					VAR1 -= 7;
				}
				break;
			default:
				break;
		}
	}
	else{		
			down=0;
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	/* Joytick up pressed */
		up++;
		switch(up){
			case 1:
				if(ready==0){
					VAR1 += 12;
				}
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
					//pushing KEY1, sets ready value to 1 -> i cannot change VAR1 anymore by pushing joystick up or down
					ready = 1; 
				if(i<N){
					VETT[i] = VAR1; 
					i++;
					VAR1 = 0;							//resets VAR1
					ready = 0;						//enables again joystick up and down to edit VAR1
				}
				
				if(i == N){
					number_of_negatives = count_negative_and_odd(VETT, N); //when N values have been added, it calls the asm function
					i = 0;		//resets i to 0, so VETT can be filled starting again from position 0
					
					//... LEDs show number of negatives value 
					enable_timer(0); //starts with leds OFF for 0.9s, then interrupt -> go to IRQ_timer.c for the rest
				}
				
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
