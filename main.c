#define F_CPU 8000000UL

#define step_num 200

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

int timer = 0;
int steper1[3]

ISR(TIMER2_COMPA_vect){//1ms毎に割り込み
	timer ++;
	if(timer>=1000){
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
	TCCR2A = 0b00000011;
	TCCR2B = 0b00001011;
	OCR2A = 250;
	TIMSK2 = 0b00000010;
	sei();
	return 0;
}
