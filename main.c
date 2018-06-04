#define F_CPU 8000000UL

#define step_num 200

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

int timer = 0;
int stepper[3][3][2]={{{0,0},{0,0},{0,0}},//{steps,speed}
									    {{0,0},{0,0},{0,0}},
										  {{0,0},{0,0},{0,0}}};
										 //if steps == 0 && speed == 0 stop
										 //if steps == 0 && speed != 0 infinty spin

ISR(TIMER2_COMPA_vect){//1ms毎に割り込み
	timer ++;
	if(timer>=1000){
		PORTC ^= (1 << PC6);
		timer=0;
	}
}
int init();
int set_stepper_step(int x,int y,int num);
int set_stepper_speed(int x,int y,int num);

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

int set_stepper_step(int x,int y,int num){
	if(x<3 && y<3){
		stepper[x][y][0] = num;
		return 1;
	}else{
		return 0;
	}
}

int set_stepper_speed(int x,int y,int num){
	if(x<3 && y<3){
		stepper[x][y][1] = num;
		return 1;
	}else{
		return 0;
	}
}
