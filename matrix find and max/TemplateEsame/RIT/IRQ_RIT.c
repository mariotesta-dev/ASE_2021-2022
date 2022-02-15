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

#define M 3
#define N 2

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int find_and_max(int MAT[][N], int X1, int Y1, int X2, int Y2, int m, int n);

int MAT[M][N] = {{0,1},{2,3},{4,5}};
unsigned int positions[4];
int i = 0;
unsigned int pos_not_normalized;
int max = 0;

//Buttons variable
int key2=0;
int key1=0;
int int0=0;


void RIT_IRQHandler (void)
{					
	
	/**** JOYSTICK CONTROL ****/
	
	//il joystick è già antirimbalzo per hardware
	// catturare valore del timer: val_int = LPC_TIM1->TC;
	
	//KEY2
		if(key2 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0)){	/* KEY2 pressed */				
			switch(key2){
				case 2:
					
					if(i < 4){
						LED_Out(0);																//pulisci LEDs
						pos_not_normalized = LPC_TIM3->TC;				//catturo valore dal timer
						if(i == 0) {															//X1
							LED_On(7);															//on LD4
							LED_On(1);															//on LD10
							positions[i] = pos_not_normalized%M;		//lo normalizzo su M e lo salvo in un vettore [x1, y1, x2, y2]
						}else if(i == 1) {												//Y1
							LED_On(7);															//on LD4
							LED_On(0);															//on LD11
							positions[i] = pos_not_normalized%N;		//lo normalizzo su N e lo salvo in un vettore [x1, y1, x2, y2]
						}else if(i == 2) {												//X2
							LED_On(6);															//on LD5
							LED_On(1);															//on LD10
							positions[i] = pos_not_normalized%M;		//lo normalizzo su M e lo salvo in un vettore [x1, y1, x2, y2]
						}else if(i == 3) {												//Y2
							LED_On(6);															//on LD5
							LED_On(0);															//on LD11
							positions[i] = pos_not_normalized%N;		//lo normalizzo su N e lo salvo in un vettore [x1, y1, x2, y2]
						}
						
						
						i++;
					}
					
					if(i==4){
						max = find_and_max(MAT, positions[0], positions[1], positions[2], positions[3], M, N);
						enable_timer(2);
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
