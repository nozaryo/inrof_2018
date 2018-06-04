#define F_CPU 8000000UL

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

int timer = 0;

ISR(TIMER2_COMPB_vect){
	timer ++;
	if(timer>=999){
		PORTC ^= (1 << PC6);
		timer=0;
	}
}
int init();

int main(void){

	init();

	DDRC |= (1 << PC6);
	PORTC |= (1 << PC6);

    while(1){
    }
}

int init(){
	TCCR2B = 3;
	OCR2B = 125;
	TIMSK2 = 0b00000100;
	sei();
	return 0;
}
