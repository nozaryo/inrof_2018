#define F_CPU 8000000UL

#define step_num 200

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

int init();
int set_stepper_step(int x,int num);
int set_stepper_speed(int x,int num);
int stepper_HIGH(int x);
int stepper_LOW(int x);
int stepper_dir(int x,bool dir);// 1:front 0:back


int timer[3] = {0,0,0};
int stepper[3][2]={{0,0},{0,20},{0,0}};//{steps,speed}
										 //if steps == 0 && speed == 0 stop
										 //if steps == 0 && speed != 0 infinty spin
int steps[3] = {0,0,0};

ISR(TIMER2_COMPA_vect){//1ms毎に割り込み
	int i;

	for(i=0;i<3;i++){
		if(stepper[i][0]==0 && stepper[i][1]==0){

		}else if(stepper[i][0]==0 && stepper[i][1] > 0){
			stepper_dir(i,1);
			timer[i]++;
			if(timer[i]>=stepper[i][1]){
				stepper_HIGH(i);
				timer[i]=0;
			}else if(timer[i]==1){
				stepper_LOW(i);
			}
		}else if(stepper[i][0]==0 && stepper[i][1] < 0){
			stepper_dir(i,0);
			timer[i]++;
			if(timer[i]>=(stepper[i][1]*(-1))){
				stepper_HIGH(i);
				timer[i]=0;
			}else if(timer[i]==1){
				stepper_LOW(i);
			}
		}else if(stepper[i][0]>0 && stepper[i][1] > 0){
			stepper_dir(i,1);
			timer[i]++;
			if(timer[i]>=stepper[i][1]){
				stepper_HIGH(i);
				timer[i]=0;
			}else if(timer[i]==1){
				stepper_LOW(i);
				if(stepper[i][0]==1){
					stepper[i][0] = 0;
					stepper[i][1] = 0;
				}
				stepper[i][0]--;
			}
		}else if(stepper[i][0]>0 && stepper[i][1] < 0){
			stepper_dir(i,0);
			timer[i]++;
			if(timer[i]>=stepper[i][1]){
				stepper_HIGH(i);
				timer[i]=0;
			}else if(timer[i]==1){
				stepper_LOW(i);
				if(stepper[i][0]==1){
					stepper[i][0] = 0;
					stepper[i][1] = 0;
				}
				stepper[i][0]--;
			}
		}
	}
}


int main(void){

	init();

  while(1){

  }
}

int init(){
	TCCR2A = 0b00000011;
	TCCR2B = 0b00001011;
	OCR2A = 250;
	TIMSK2 = 0b00000010;
	TCCR1A = 0b10100010;
	TCCR1B = 0b00011100;
	ICR1 = 625;
	OCR1A = 0;
	OCR1B = 0;
	DDRC |= 0b11111111;
	PORTC |= 0b01010100;
	//DDRA = 0b11111111;

	sei();
	return 0;
}

int set_stepper_step(int x,int num){
	if(x<3){
		stepper[x][0] = num;
		return 1;
	}else{
		return 0;
	}
}

int set_stepper_speed(int x,int num){
	if(x<3){
		stepper[x][1] = num;
		return 1;
	}else{
		return 0;
	}
}

int stepper_HIGH(int x){
	x+=2;
	if(2<=x && x<=4){
		PORTC |= (1 << (2*x-1));
		return 1;
	}else{
		return 0;
	}

	return 0;
}

int stepper_LOW(int x){
	x+=2;
	if(2<=x && x<=4){
		PORTC &= ~(1 << (2*x-1));
		return 1;
	}else{
		return 0;
	}

	return 0;
}

int stepper_dir(int x,bool dir){

	 if(0<=x && x<=2){
		if(dir == 1){
			x+=1;
			PORTC |= (1 << 2*x);
		  return 1;
		}else if(dir == 0){
			x+=1;
			PORTC &= ~(1 << 2*x);
			return 1;
		}
	}

	return 0;

}
