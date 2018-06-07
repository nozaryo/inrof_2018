#define F_CPU 8000000UL

#define step_num 200

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>

int init();
int set_stepper_step(int x,int y,int num);
int set_stepper_speed(int x,int y,int num);
int stepper_HIGH(int x,int y);
int stepper_LOW(int x,int y);
int stepper_dir(int x,int y,bool dir);// 1:front 0:back


int timer[2][3] = {{0,0,0},{0,0,0}};
int stepper[2][3][2]={{{0,0},{0,20},{0,0}},//{steps,speed}
									    {{0,50},{0,50},{0,50}}};
										 //if steps == 0 && speed == 0 stop
										 //if steps == 0 && speed != 0 infinty spin
int steps[2][3] = {{0,0,0},{0,0,0}};

ISR(TIMER2_COMPA_vect){//1ms毎に割り込み
	int i,j;
	for(i=0;i<2;i++){
		for(j=0;j<3;j++){
			if(stepper[i][j][0]==0 && stepper[i][j][1]==0){

			}else if(stepper[i][j][0]==0 && stepper[i][j][1] > 0){
				stepper_dir(i,j,1);
				timer[i][j]++;
				if(timer[i][j]>=stepper[i][j][1]){
					stepper_HIGH(i,j);
					steps[i][j]++;
					timer[i][j]=0;
				}else if(timer[i][j]==1){
					stepper_LOW(i,j);
				}
			}else if(stepper[i][j][0]==0 && stepper[i][j][1] < 0){
				stepper_dir(i,j,0);
				timer[i][j]++;
				if(timer[i][j]>=(stepper[i][j][1]*(-1))){
					stepper_HIGH(i,j);
					steps[i][j]--;
					timer[i][j]=0;
				}else if(timer[i][j]==1){
					stepper_LOW(i,j);
				}
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
	DDRC |= 0b11111111;
	PORTC |= 0b01010100;
	//DDRA = 0b11111111;

	sei();
	return 0;
}

int set_stepper_step(int x,int y,int num){
	if(x<2 && y<3){
		stepper[x][y][0] = num;
		return 1;
	}else{
		return 0;
	}
}

int set_stepper_speed(int x,int y,int num){
	if(x<2 && y<3){
		stepper[x][y][1] = num;
		return 1;
	}else{
		return 0;
	}
}

int stepper_HIGH(int x,int y){
	y+=2;
	if(x < 0 && 1 < x && y < 2 && 4 < y){
		return 0;
  }else if(x == 0){
		PORTC |= (1 << (2*y-1));
		return 1;
	}else if(x == 1){
		PORTB |= (1 << (2*y-1));
		return 1;
	}else{
		return 0;
	}

}

int stepper_LOW(int x,int y){
	y+=2;
	if(x < 0 && 1 < x && y < 2 && 4 < y){
		return 0;
	}else if(x == 0){
		PORTC &= ~(1 << (2*y-1));
		return 1;
	}else if(x == 1){
		PORTB &= ~(1 << (2*y-1));
		return 1;
	}else{
		return 0;
	}

}

int stepper_dir(int x,int y,bool dir){

	 if(0<=x && x<=1 && 0<=y && y<=2){
		 if(x == 0){
				if(dir == 1){
					y+=1;
					PORTC |= (1 << 2*y);
				  return 1;
				}else if(dir == 0){
					y+=1;
					PORTC &= ~(1 << 2*y);
					return 1;
				}
			}else if(x == 1){
				if(dir == 1){
					y+=1;
					PORTB |= (1 << 2*y);
				  return 1;
				}else if(dir == 0){
					y+=1;
					PORTB &= ~(1 << 2*y);
					return 1;
				}
			}
	}

	return 0;

}
