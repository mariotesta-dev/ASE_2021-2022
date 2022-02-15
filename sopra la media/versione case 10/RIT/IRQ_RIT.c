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

#define N 10


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern unsigned int sopra_la_media(unsigned char VETT[], unsigned int n); 

unsigned char VAR = 0;
unsigned char last_VAR = 0;
unsigned char VETT[N];
unsigned int i = 0;
unsigned int j = 0;
unsigned int result;
int showing_result = 0;

//Buttons variable
int key2=0;
int key1=0;
int int0=0;

//Joystick variables
int select=0;
int left=0;
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
				
					VAR = 0;		//premendo select, resetta VAR a 0
					last_VAR = VAR;
					LED_Out(VAR);
			
					if(showing_result == 1){ //se sto mostrando i risultati, 0xAA e 0x55 in sequenza
						reset_timer(3);
						disable_timer(3);				//ferma e resetta il timer così da fermare la sequenza sui led
						showing_result = 0;
						
						for(j = 0; j<i; j++){		//reset valori VETT
							VETT[j] = 0;
						}
						
						i = 0;								//reset index i
					}
					
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}	

	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	/* Joytick left pressed */
		left++;
		switch(left){
			case 10:
				VAR -= 1;						//se pressione >= 0.5s -> decrementa var di 1
				if(VAR>=0 && VAR<=255){	//se VAR compreso tra 0 e 255
					LED_Out(VAR);					//mostra il valore in binario sui led
					last_VAR = VAR;				//salva VAR nella variabile last_VAR
				}else{
					VAR = last_VAR;				//nel caso in cui VAR non sia compreso nel range, riportalo all'ultimo valore "coerente" con il range
				}												//NB. In realtà 0 - 1 = 255 in quanto il dato è inizializzato come unsigned char, quindi il problema del range non si pone!
				break;
			default:
				break;
		}
	}
	else{		
			left=0;
	}
	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	/* Joytick up pressed */
		up++;
		switch(up){
			case 10:
					VAR += 1;						//se pressione >= 0.5s -> incrementa var di 1
					if(VAR>=0 && VAR<=255){	//se VAR compreso tra 0 e 255 --> ma sarà comunque compreso... perchè è definito come unsigned char, infatti ritorna un warning...
						LED_Out(VAR);					//mostra il valore in binario sui led
						last_VAR = VAR;				//salva VAR nella variabile last_VAR
					}else{
						VAR = last_VAR;				//nel caso in cui VAR non sia compreso nel range, riportalo all'ultimo valore "coerente" con il range
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
	
	//INT0
	if(int0 != 0){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */				
			switch(int0){
				case 2:
					
					if(i<N){						//alla pressione di INT0
						VETT[i] = VAR;		//salvo il valore attuale di VAR all'interno di VETT fino a che i == N-1
						VAR = 0;					//una volta salvato, VAR viene resettato
						LED_Out(0);
						i++;
					}
					
					if(i == N || VETT[i-1]==VETT[i-2]){					//qualora il sistema abbia acquisito N valori o gli ultimi 2 inseriti sono uguali...
						result = sopra_la_media(VETT, i); //invoca funzione ASM
						showing_result = 1;
						enable_timer(3);				//abilita timer che mostrerà in sequenza: result, 0xAA, 0x55... finchè select non viene premuto
					}
				
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
