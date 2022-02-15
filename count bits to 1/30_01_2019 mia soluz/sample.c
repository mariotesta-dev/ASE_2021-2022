/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT and joystick buttons
 * Note(s):
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "adc/adc.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {

	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	
	//init e enable allo start di timer 3, count di 1.3 min 
	init_timer(3,0x743AA380);							/* 25MHz * 1,3*60s	= 743AA380				*/
	enable_timer(3);
	
	//init timer 2 che manda un interrupt e si stoppa
	init_timer(2,0x007270E0);							/* 25MHz * 0.3s = 0x007270E0		       */
	//Il timer opera a 25Mhz, per lampeggiare ad una frequenza di 10Hz (ciclo di acceso e spento => 2) il K si calcola: 25Mhz * (2/freq)|| 25Mhz * periodo
	//se lavoro sulla scheda e uso timer 2 o timer 3 devo prima abilitarli in system_LPC.c-> configuration wizard -> PCONP
	
	init_timer(1,0x0003E1C0); //enable timer with period 1.2s, half time for each function 
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
