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
#include "../enum.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern unsigned char duty_cycle(unsigned char valore_iniziale, unsigned int tempo_totale, unsigned int tempi_variazioni[], unsigned int numero_variazioni);

extern STATE state;
unsigned char valore_iniziale;
unsigned char ultimo_valore;
unsigned int tempi_variazioni[5];
unsigned int tempo_totale = 0;
unsigned int num_variazioni = 0;
int duty_result = 0;

//Buttons variable
int key2=0;
int key1=0;
int int0=0;

//Joystick variables
int up=0;

void RIT_IRQHandler (void)
{					
	
	/**** JOYSTICK CONTROL ****/
	
	//il joystick è già antirimbalzo per hardware
	// catturare valore del timer: val_int = LPC_TIM1->TC;
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0 && state==ACQUISIZIONE){	/* Joytick up pressed */
		up++;
		switch(up){
			case 1:
					if(num_variazioni < 5 && ultimo_valore!=1){		//se non ho ancora recepito 5 variazioni e il valore precedente non era 1, cioè UP
						
						disable_timer(2);
						tempi_variazioni[num_variazioni] = LPC_TIM2 -> TC; //catturo la durata della variazione, cioè il tempo passato con SEGNALE=0, e lo salvo nel vettore tempi_variazioni
						tempo_totale += LPC_TIM2 -> TC;	//sommo il tempo totale con il valore attuale del timer2
						reset_timer(2); //reset timer2 to start counting time elapsed again
						enable_timer(2);
						
						num_variazioni++;	//incremento il numero di variazioni
						ultimo_valore = 1; //ultima variazione recepita 0 -> 1
					}
					
					if(num_variazioni == 5){ 			//se ho recepito 5 variazioni, disabilito tutto e torno in stato=MONITOR
						reset_timer(3);
						disable_timer(3);
						disable_timer(2);
						duty_result = duty_cycle(valore_iniziale, tempo_totale, tempi_variazioni, num_variazioni);
						LED_Out(0);
						enable_timer(1);
						state=MONITOR;
					}
				break;
			default:
				break;
		}
	}
	else if((LPC_GPIO1->FIOPIN & (1<<29)) != 0 && state==ACQUISIZIONE){
		
			if(num_variazioni < 5 && ultimo_valore!=0){ //faccio le stesse cose che succedono per il caso UP=1, ma nel caso UP va a 0 
				disable_timer(2);
				tempi_variazioni[num_variazioni] = LPC_TIM2 -> TC; //salvo la durata della variazione, cioè il tempo passato con SEGNALE = 1
				tempo_totale += LPC_TIM2 -> TC; //sommo con il tempo totale
				
				reset_timer(2); //resetto timer2 per ricominciare a contare la durata della nuova variazione
				enable_timer(2);
				
				num_variazioni++; //incremento numero variazioni
				ultimo_valore = 0;	//ultima variazione che ho letto è stata da 1 -> 0
			}
					
			if(num_variazioni == 5){
						reset_timer(3);
						disable_timer(3);
						disable_timer(2);
						duty_result = duty_cycle(valore_iniziale, tempo_totale, tempi_variazioni, num_variazioni);
						LED_Out(0);
						enable_timer(1);
						state=MONITOR;
			}
					
			up=0;
	}
	
	
	//KEY2
		if(key2 > 1){ 
		if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0) && state==MONITOR){	/* KEY2 pressed */				
			switch(key2){
				case 2:
					
					//change status to Acquisizione and enable timer 3
					state=ACQUISIZIONE;
					enable_timer(3);
					LED_Out(255); //in modalità acquisizione tutti i led sono accesi
					
					//set RIT @25ms --> per campionare da qui in poi il joystick con frequenza di 25ms
					disable_RIT();
					init_RIT(0x002625A0);
					enable_RIT();
				
					//get initial signal value from joystick UP
					if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
						//if up is already pressed
						valore_iniziale = 1;
					}else{
						valore_iniziale = 0;
					}
					
					ultimo_valore = valore_iniziale; //setto ultimo valore letto = al valore iniziale
					
					//enable timer 2 to start count the time elapsed 
					enable_timer(2);
					
				
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
