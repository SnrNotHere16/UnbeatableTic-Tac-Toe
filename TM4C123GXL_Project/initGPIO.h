/* 
Declarations for the GPIO pins functions for the Tic Tac Toe game which includes 
	1. 18 outputs  (LED) (PB0-PB7, PA4 (R1-R9)), (PD2-PD3, PD6-PD7,  PC5-PC7, PE0,PE3(R1-R9)))
	2. 3 inputs (Buttons) (PF0,PF4)
	3. 2 inputs (ADC) (PE1-PE2)
	4. 2 outputs (PWM) (PC4,PE4)
*/ 
#include "tm4c123gh6pm.h"
#include <stdint.h>
 #ifndef INITGPIO_H
 #define INITGPIO_H
//GPIO Declarations
	//Initialize the ports for the LED outputs 9 (PB0-PB7,PC4)
		void LEDInit(void);
	//Initialize the ports for the buttons inputs 3 (PF0,PF4, PA4)
		void ButtonInit(void);
	//Initialize the ports for the ADC inputs 2 (PE1-PE2)
		void ThumbstickInit(void);
	//Initialize the ports for PWN outputs 1 (PC5)
		void PWMInit(uint16_t period, uint16_t duty1, uint16_t duty2);
 #endif 