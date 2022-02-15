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


extern unsigned char get_and_sort(unsigned char* VETT, unsigned char VAL, int n);

unsigned char VETT[N]; //vettore di N unsigned char da 8bit -> valori <256
int i = 0;
unsigned int VAL_INT;
unsigned char VAL;

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
					
					if(i<N){
						
						VAL_INT = LPC_TIM2->TC;  	//catturare valore del timer: LPC_TIM1->TC;
						VAL = (VAL_INT & 0x00ff0000) >> 16; //estraggo bit 16-23 e shifto a dx di 16 bit
						
						LED_Out(i+1);							//"inizialmente 0, dopo aver catturato il valore, mostrerà 1..."
																			//"... ho supposto quindi che i led mostrassero l'indice successivo"
						
						get_and_sort(VETT, VAL, i); //asm function that get VAL, add it to VETT and SORT VETT, then returns VAL
						
						i++;
						
						if(i==N){ //if after updating i, it become == N
							enable_timer(1);
							NVIC_DisableIRQ(EINT1_IRQn); //disable clicking this button
						}
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
