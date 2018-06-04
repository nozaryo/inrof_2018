#define F_CPU 8000000UL

#include<avr/io.h>
#include<util/delay.h>

int main(void){
	DDRC |= (1 << PC6);
	PORTC |= (1 << PC6);

    while(1){
	PORTC ^= (1 << PC6);
	_delay_ms(1000);
    }
}
