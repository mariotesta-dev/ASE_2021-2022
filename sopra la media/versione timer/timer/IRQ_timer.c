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


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern int result;
extern unsigned char VAR, last_VAR;
int k = 0;

void TIMER0_IRQHandler (void)
{
	//LEFT
	
	VAR -= 1;						//se pressione >= 0.5s -> decrementa var di 1
				if(VAR>=0 && VAR<=255){	//se VAR compreso tra 0 e 255
					LED_Out(VAR);					//mostra il valore in binario sui led
					last_VAR = VAR;				//salva VAR nella variabile last_VAR
				}else{
					VAR = last_VAR;				//nel caso in cui VAR non sia compreso nel range, riportalo all'ultimo valore "coerente" con il range
				}												//NB. In realtà 0 - 1 = 255 in quanto il dato è inizializzato come unsigned char, quindi il problema del range non si pone!
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	//UP
	VAR += 1;								//se pressione >= 0.5s -> incrementa var di 1
	if(VAR>=0 && VAR<=255){	//se VAR compreso tra 0 e 255 --> ma sarà comunque compreso... perchè è definito come unsigned char, infatti ritorna un warning...
		LED_Out(VAR);					//mostra il valore in binario sui led
		last_VAR = VAR;				//salva VAR nella variabile last_VAR
	}else{
		VAR = last_VAR;				//nel caso in cui VAR non sia compreso nel range, riportalo all'ultimo valore "coerente" con il range
	}
	
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
	switch(k){
		case 0:
			LED_Out(result);
			k++;
			break;
		case 1:
			LED_Out(0xAA);
			k++;
			break;
		case 2:
			LED_Out(0x55);
			k = 0;
			break;
		default:
			break;
	}
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
