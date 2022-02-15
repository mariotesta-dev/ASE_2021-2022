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
#include "../enum.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


extern unsigned char duty_cycle(unsigned char valore_iniziale, unsigned int tempo_totale, unsigned int tempi_variazioni[], unsigned int numero_variazioni);
extern STATE state;
extern unsigned char valore_iniziale;
extern unsigned int tempo_totale;
extern unsigned int tempi_variazioni[];
extern unsigned int num_variazioni;
extern int duty_result;

int i = 0;

void TIMER0_IRQHandler (void)
{
	//se interrupt è attivo per il timer, non appena verrà messo a 1 --> andrà nell'handler del timer
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


void TIMER1_IRQHandler (void)
{  
	
	if(state==MONITOR){
		if(i%2 == 0){ 			//show num_variazioni
			LED_On(7); 				//accendi LED7
			LED_Out_0to6(num_variazioni); //mostra numero variazioni sui LED0-LED6
			i++;
		}else{							//show duty cycle
			LED_Off(7);				//spegni LED7
			LED_Out_0to6(duty_result);	//mostra duty cycle sui LED0-LED6
			i++;
		}
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
	//after 1 second has passed, calculate duty cycle and set state back to MONITOR
	
	duty_result = duty_cycle(valore_iniziale, tempo_totale, tempi_variazioni, num_variazioni);
	state = MONITOR;
	
	enable_timer(1); //enables timer 1 to show values on LEDs
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
