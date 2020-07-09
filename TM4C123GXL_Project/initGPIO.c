/* 
Definitions for the GPIO pins functions for the Tic Tac Toe game which includes 
	1. 18 outputs  (LED) (PB0-PB7, PA4), (PD2-PD3, PD6-PD7,  PC5-PC7, PE0,PE3)
	2. 3 inputs (Buttons) (PF0,PF4)
	3. 2 inputs (ADC) (PE1-PE2)
	4. 2 outputs (PWM) (PC4,PE4)
*/ 
#include <stdint.h>
#include "initGPIO.h"

void LEDInit(){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002;     // 1) B
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTB_CR_R |= 0xFF;           // allow changes to PB0-PB7  
  GPIO_PORTB_AMSEL_R &= ~(0xFF);        // 3) disable analog function
  GPIO_PORTB_PCTL_R &= ~(0x11111111);   // 4) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R |= 0xFF;             // 5) PB0-PB7 output   
  GPIO_PORTB_AFSEL_R &= ~(0xFF);        // 6) no alternate function      
  GPIO_PORTB_DEN_R |= 0xFF;            // 7) enable digital pins PB0-PB7
	

	SYSCTL_RCGC2_R |= 0x00000001;     // 1) A
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTA_CR_R |= 0x10;           // allow changes to PA4 
  GPIO_PORTA_AMSEL_R &= ~(0x10);        // 3) disable analog function
  GPIO_PORTA_PCTL_R &= ~(0x00010000);   // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R |= 0x10;          // 5) PA output   
  GPIO_PORTA_AFSEL_R &= ~(0x10);        // 6) no alternate function      
  GPIO_PORTA_DEN_R |= 0x10;          // 7) enable digital pins PA4
	
	SYSCTL_RCGC2_R |= 0x00000004;     // 1) C
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTC_CR_R |= 0xE0;           // allow changes to PC5-PC7
  GPIO_PORTC_AMSEL_R &= ~(0xE0);        // 3) disable analog function
  GPIO_PORTC_PCTL_R &= ~(0x11100000);   // 4) GPIO clear bit PCTL  
  GPIO_PORTC_DIR_R |= 0xE0;          // 5) PC5-PC7 output   
  GPIO_PORTC_AFSEL_R &= ~(0xE0);        // 6) no alternate function      
  GPIO_PORTC_DEN_R |= 0xE0;          // 7) enable digital pins PC5-PC7
	      
	SYSCTL_RCGC2_R |= 0x00000008;     // 1) D
  delay = SYSCTL_RCGC2_R;           // delay   
	GPIO_PORTD_LOCK_R = 0x4C4F434B;    // 2) unlock PortD PD7 (this is for PD7)
  GPIO_PORTD_CR_R |= 0xCC;           // allow changes to PD2-PD3, PD6-PD7  
  GPIO_PORTD_AMSEL_R &= ~(0xCC);        // 3) disable analog function
  GPIO_PORTD_PCTL_R &= ~(0x11000011);   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R |= 0xCC;          // 5) PD2-PD3, PD6-PD7 output   
  GPIO_PORTD_AFSEL_R &= ~(0xCC);        // 6) no alternate function      
  GPIO_PORTD_DEN_R |= 0xCC;          // 7) enable digital pins PD2-PD3, PD6-PD7
	
	
	SYSCTL_RCGC2_R |= 0x00000010;     // 1) E
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_CR_R |= 0x09;           // allow changes to PE0,PE3
  GPIO_PORTE_AMSEL_R &= ~(0x09);        // 3) disable analog function
  GPIO_PORTE_PCTL_R &= ~(0x00001001);   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R |= 0x09;          // 5) PE0, PE3 output   
  GPIO_PORTE_AFSEL_R &= ~(0x09);        // 6) no alternate function      
  GPIO_PORTE_DEN_R |= 0x09;          // 7) enable digital pins PE0,PE3

	
	SYSCTL_RCGC2_R |= 0x00000020;      // 1) F clock   used to turn on a specific port F E D C B A  in this case 1 0 0 0 0 0, F = 1 so port F is turned on
  delay = SYSCTL_RCGC2_R;            // delay - two clock cycles
  GPIO_PORTF_LOCK_R = 0x4C4F434B;    // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x0E;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R &= ~0x0E;       // 3) disable analog function  - set all to 0
  GPIO_PORTF_PCTL_R &= ~0x0000000E;   // 4) GPIO clear bit PCTL - set all to 0 - Same as Ox0 but Ox00000000 tells user its 32 bits long
  GPIO_PORTF_DIR_R |= 0x0E;          // 5) PF3,PF2,PF1 are output (LED)-  0 0 0 0 1 1 1 0  
  GPIO_PORTF_AFSEL_R &= ~0x1F;       // 6) no alternate function - set all to 0
  GPIO_PORTF_DEN_R |= 0x1F;          // 7) enable digital pins PF4-PF0 - 0 0 0 1 1 1 1 1 enable all pins
}

void ButtonInit(){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;      // 1) F clock   used to turn on a specific port F E D C B A  in this case 1 0 0 0 0 0, F = 1 so port F is turned on
  delay = SYSCTL_RCGC2_R;            // delay - two clock cycles
  GPIO_PORTF_LOCK_R = 0x4C4F434B;    // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R |= 0x11;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R &= ~0x11;       // 3) disable analog function  - set all to 0
  GPIO_PORTF_PCTL_R &= ~0x00000011;   // 4) GPIO clear bit PCTL - set all to 0 - Same as Ox0 but Ox00000000 tells user its 32 bits long
	GPIO_PORTF_DIR_R &= ~0x11;         // 5) PF4,PF0 are input (Switch) -  0 0 0 0 1 1 1 0 
  GPIO_PORTF_AFSEL_R &= ~0x11;       // 6) no alternate function - set all to 0
  GPIO_PORTF_PUR_R |= 0x11;          // enable pullup resistors on PF4,PF0 - 0 0 0 1(sw1) 0 0 0 1(sw2) enables pull up resistor for each switch
  GPIO_PORTF_DEN_R |= 0x11;          // 7) enable digital pins PF4-PF0 - 0 0 0 1 1 1 1 1 enable all pins
	GPIO_PORTF_IS_R &= ~0x11;		// PF4,0 are edge sensitive
	GPIO_PORTF_IBE_R &= ~0x11;	// PF4,0 are not both edge sensitive
	GPIO_PORTF_IEV_R &= ~0x11;	// PF4,0 are negedge triggered
	GPIO_PORTF_ICR_R = 0x11;		// clear flag on PF4,0
	GPIO_PORTF_IM_R |= 0x11;		// interrupt on PF4,0
	NVIC_PRI7_R = ( NVIC_PRI7_R & 0xFF1FFFFF ) | 0x00A00000;	// Set interrupt priority to 5
	NVIC_EN0_R |= 0x40000000; 
	
	SYSCTL_RCGC2_R |= 0x00000010;      // 1) E 
  delay = SYSCTL_RCGC2_R;            // delay - two clock cycles 
  GPIO_PORTE_CR_R |= 0x20;           // allow changes to PE5     
  GPIO_PORTE_AMSEL_R &= ~0x20;       // 3) disable analog function  - set PE5 to 0
  GPIO_PORTE_PCTL_R &= ~0x00000020;   // 4) GPIO clear bit PCTL - set PE5 to 0
	GPIO_PORTE_DIR_R &= ~0x20;         // 5) PE5 are input (Switch) 
  GPIO_PORTE_AFSEL_R &= ~0x20;       // 6) no alternate function - PE5 to 0
  GPIO_PORTE_DEN_R |= 0x20;          // 7) enable digital pins PE5 
	GPIO_PORTE_IS_R &= ~0x20;		// PE5 are edge sensitive
	GPIO_PORTE_IBE_R &= ~0x20;	// PE5 are not both edge sensitive
	GPIO_PORTE_IEV_R |= 0x20;	// PE5 are posedge triggered
	GPIO_PORTE_ICR_R |= 0x20;		// clear flag on PE5
	GPIO_PORTE_IM_R  |= 0x20;		// interrupt on PE5
	NVIC_PRI1_R = ( NVIC_PRI1_R & 0xFFFFFF1F ) | 0x00000020;	// Set interrupt priority to 2
	NVIC_EN0_R |= 0x00000010; 
}
//initialize ports PE1-PE2 for ADC functionality 
void ThumbstickInit(){
	 volatile unsigned long delay;
	SYSCTL_RCGCADC_R |= 0x02;
  SYSCTL_RCGC2_R |= 0x00000010;   							// 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;         							//  delay
  GPIO_PORTE_DIR_R &= ~0x06;      							// 2) make PE1,PE2 input
  GPIO_PORTE_AFSEL_R |= 0x06;    								// 3) enable alternate function on PE1,PE2
  GPIO_PORTE_DEN_R &= ~0x06;      							// 4) disable digital I/O on PE1, PE2
  GPIO_PORTE_AMSEL_R |= 0x06;     							// 5) enable analog function on PE1, PE2
  SYSCTL_RCGC0_R |= 0x00020000;   							// 6) enable ADC1 (pg 458)
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  							// 7) configure for 125K  (pg458)
  ADC1_SSPRI_R = 0x0123;          							// 8) Sequencer 2 is highest priority (pg841)
																								//  [3:0] - disable(0) enable(1) ASEN0-ASEN3
  ADC1_EMUX_R |= 0x0F00;         							// 9) seq2 is always (continuosly sampling) (pg 833)
																								// [15:12]- seq3, [11:8] - seq2, [7:4]-seq1, [3:0] - seq0
  ADC1_SSMUX2_R |= 	0x0000021;									// 10) channel Ain2 (PE1, 2)  Ain1 (PE2, 1) pg801, pg875
  ADC1_SSCTL2_R |= 0x0060;         						  // 11) disable TS0 D0, enable IE0 END0, pg876
  ADC1_ACTSS_R |= 0x0004;         							// 12) enable sample sequencer 2 (pg821)
	ADC1_IM_R = 0x00000004;  											// enable interrupt ss2, pg825
	NVIC_PRI4_R = ( NVIC_PRI4_R & 0xFFFF0FFF ) | 0x00002000;	// Set interrupt priority to 1
	NVIC_EN1_R          |=  0x00040000;           // pg 104, 134, 142	
}

void PWMInit(uint16_t period, uint16_t duty1, uint16_t duty2){
//	//Select which PWM pg 1233, we will select Module 0 generator 2 PWM4, which is dictated by PE4
//	// PWM0 - PWM module 0 generator 2 (PE4)(PWM4), PWM module 0 generator 3 (PC4)(PWM6)
//  // setup on pg 1239
	SYSCTL_RCGCPWM_R |=0x01;  // activate PWM0 (module 0) pg354
	SYSCTL_RCGCGPIO_R|=0x14; //activate portE and portC
	while ((SYSCTL_PRGPIO_R&0x14) == 0) {};
	GPIO_PORTC_AFSEL_R |= 0x10; // enable alt funct on PC4
	GPIO_PORTC_PCTL_R &= ~0x000F0000; // configure PC4 as PWM0
	GPIO_PORTC_PCTL_R |= 0x00040000; // (pg1352,688)
	GPIO_PORTC_AMSEL_R &= ~0x10; // disable analog functionality on PC4
	GPIO_PORTC_DEN_R |= 0x10; // enable digital I/O on PC4
	SYSCTL_RCC_R = 0x00100000 | // 3) use PWM divider
(	SYSCTL_RCC_R & (~0x000E0000)); // configure for /2 divider
	PWM0_3_CTL_R = 0; // 4) re-loading down-counting mode (pg1266)
	PWM0_3_GENA_R = 0xC8; // low on LOAD, high on CMPA down (pg1282)
	// PC4 goes low on LOAD
	// PC4 goes high on CMPA down
	PWM0_3_LOAD_R = period - 1; // 5) cycles needed to count down to 0
	PWM0_3_CMPA_R = duty1 - 1; // 6) count value when output rises
	PWM0_3_CTL_R |= 0x00000001; // 7) start PWM0
	PWM0_ENABLE_R |= 0x00000040; // enable PC4/PWM0(PWM6) (generator 3) (1247)

	GPIO_PORTE_AFSEL_R |= 0x10; // enable alt funct on PE4
	GPIO_PORTE_PCTL_R &= ~0x000F0000; // configure PE4 as PWM0
	GPIO_PORTE_PCTL_R |= 0x00040000; // (pg1352,688)
	GPIO_PORTE_AMSEL_R &= ~0x10; // disable analog functionality on PE4
	GPIO_PORTE_DEN_R |= 0x10; // enable digital I/O on PE4
	SYSCTL_RCC_R = 0x00100000 | // 3) use PWM divider
(	SYSCTL_RCC_R & (~0x000E0000)); // configure for /2 divider
	PWM0_2_CTL_R = 0; // 4) re-loading down-counting mode (pg1266)
	PWM0_2_GENA_R = 0xC8; // low on LOAD, high on CMPA down (pg1282)
	// PE4 goes low on LOAD
	// PE4 goes high on CMPA down
	PWM0_2_LOAD_R = period - 1; // 5) cycles needed to count down to 0
	PWM0_2_CMPA_R = duty2 - 1; // 6) count value when output rises
	PWM0_2_CTL_R |= 0x00000001; // 7) start PWM0
	PWM0_ENABLE_R |= 0x00000010; // enable PE4/PWM0(PWM4) (generator 2) (1247)
}
