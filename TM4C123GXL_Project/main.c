/* 
GPIO ports
	1. 18 outputs  (LED) 
		a. set1(PB0-PB7, PA4)
		b. set2(PD2-PD3, PD6-PD7,  PC5-PC7, PE0,PE3)
	2. 3 inputs (Buttons) (PF0,PF4,)
	3. 2 inputs (ADC) (PE1-PE2)
	4. 2 outputs (PWM) (PC4,PE4)
*/ 
// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground


#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "initGPIO.h"
#include "ST7735.h"
#include "PLL.h"
#include "TicTacToe.h"

unsigned long In1;

int main() {
	LEDInit();
	PLL_Init();  
	ButtonInit();
	ThumbstickInit();
	PWMInit(16000,1600, 13000);
  ST7735_InitR(INITR_REDTAB);
	GPIO_PORTF_DATA_R = 0x02;  
  while (1){
		GameStart();
	}

}

