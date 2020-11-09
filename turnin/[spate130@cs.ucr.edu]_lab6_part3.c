/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://youtu.be/_MlJeM78p98
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


  volatile unsigned char TimerFlag = 0; 
  unsigned long _avr_timer_M = 1;
  unsigned long _avr_timer_cntcurr = 0; 



  void TimerOn() {
	  TCCR1B = 0x0B;

	  OCR1A = 125;	
	  TIMSK1 = 0x02;

	  TCNT1=0;

	  _avr_timer_cntcurr = _avr_timer_M;

	  SREG |= 0x80; // 0x80: 1000000
  }

  void TimerOff() {
	  TCCR1B = 0x00; 
  }

  void TimerISR() {
	  TimerFlag = 1;
  }

  ISR(TIMER1_COMPA_vect) {
	  _avr_timer_cntcurr--;
	  if (_avr_timer_cntcurr == 0) { 
		  TimerISR();
		  _avr_timer_cntcurr = _avr_timer_M;
	  }
  }

  void TimerSet(unsigned long M) {
	  _avr_timer_M = M;
	  _avr_timer_cntcurr = _avr_timer_M;
  }

enum States{init, inc, dec, reset, wait,wait2} state;

unsigned char button0;
unsigned char button1;

unsigned char tempB = 0x07;


void Tick(){
	button0 = ~PINA & 0x01;
	button1 = ~PINA & 0x02;

	switch(state){ // Transitions
		case init:
			if(!button0 && !button1){
				state = init;
			}
			else if(button0 && !button1){
				state = inc;
			}
			else if(!button0 && button1){
				state = dec;
			}
			else if(button0 && button1){
				state = reset;
			}
			break;
		case inc:
			if(button0 && !button1){
				state = inc;
			}
			else if(button0 && button1){
				state = reset;
			}
			else{
				state = wait2;
			}
			break;
		case dec:
			if(!button0 && button1){
				state = dec;
			}
			else if(button0 && button1){
				state = reset;
			}
			else{
				state = wait2;
			}
			break;
		case reset:
			if(!button0 && !button1){
				state = init;
			}
			else if (button0 && !button1){
				state = inc;
			}
			else if(!button0 && button1){
				state = dec;
			}
			else if(button0 && button1){
				state = reset;
			}
			break;
		case wait:
			if(button0 && button1){
				state = reset;
			}
			else if(button0 && !button1){
				state = inc;
			}
			else if(!button0 && button1){
				state = dec;
			}
			else{
				state = wait;
			}
			break;
		case wait2:
			if(!button0 && !button1){
				state = wait;
			}
			else if(button0 && button1){
				state = reset;
			}
			else{
				state = wait2;
			}
			break;

	}
	switch(state){ // State actions
		case init:
		//	tempB = 0x07;
			break;
		case inc:
			if(tempB < 9){
				tempB = tempB + 1;
			}
			break;
		case dec:
			if(tempB > 0){
				tempB = tempB - 1;
			}
			break;
		case reset:
			tempB = 0x00;
			break;
	}
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(100);
	TimerOn();
	state = init;
    /* Insert your solution below */
    while (1) {    
	Tick();
	while(!TimerFlag);
        TimerFlag = 0;
	
	PORTB = tempB;	
    }
    return 1;
}
