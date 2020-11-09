/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://youtu.be/_MlJeM78p98
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


  volatile unsigned char TimerFlag = 0; 
  unsigned long _avr_timer_M = 1;
  unsigned long _avr_timer_cntcurr = 0; 
  unsigned char tempB = 0x00;


  enum States{init, one, two, three}state;

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




void Tick(){
	switch (state)
	{
		case init:
			state = one;
			break;
		case one:
			state = two;
			break;
		case two:
			state = three;
			break;
		case three:
			state = one;
			break;
	}
	switch (state){
		case one:
			tempB = 0x01;
			break;
		case two:
			tempB = 0x02;
			break;
		case three:
			tempB = 0x04;
			break;

	}
  }






int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
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
