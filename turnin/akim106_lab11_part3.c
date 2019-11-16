/*	Author: akim106
 *      Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 11  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//Work in Progress
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
#include "io.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char output;

enum KeypadStates{GetOutput};

int KeypadTick(int KeypadState) {
    unsigned char input;
    input = GetKeypadKey();
    switch (KeypadState) {
      case GetOutput:
        switch (input) {
          case '\0': output = 0x1F; break;
          case '1': output = 0x01; break;
          case '2': output = 0x02; break;
          case '3': output = 0x03; break;
          case '4': output = 0x04; break;
          case '5': output = 0x05; break;
          case '6': output = 0x06; break;
          case '7': output = 0x07; break;
          case '8': output = 0x08; break;
          case '9': output = 0x09; break;
          case 'A': output = 0x0A; break;
          case 'B': output = 0x0B; break;
          case 'C': output = 0x0C; break;
          case 'D': output = 0x0D; break;
          case '*': output = 0x0E; break;
          case '0': output = 0x00; break;
          case '#': output = 0x0F; break;
          default: output = 0x1B; break;
        }
        KeypadState = GetOutput;
        PORTB = output;
        break;
      }
      return KeypadState;
}

int main(void) {
    unsigned long GCD = 10;
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = GetOutput;
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadTick;

    TimerSet(GCD);
    TimerOn();
   
    
    while (1) {
      for (int i=0; i < numTasks; i++) {
        if (tasks[i]->elapsedTime == tasks[i]->period) {
          tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
          tasks[i]->elapsedTime = 0;   
        }
        tasks[i]->elapsedTime += GCD;
      }
      while(!TimerFlag);
      TimerFlag = 0;
    }
    return 1;
}
