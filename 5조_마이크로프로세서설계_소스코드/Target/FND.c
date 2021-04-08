#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char digit[11] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67, 0x00};
unsigned char fnd_sel[4] = {0x08, 0x04, 0x02, 0x01};
unsigned char fnd[4];

void adc_init() // Initialize ADC
{
   DDRF=0x00; // Use F Port as Input
   DDRD=0x00; // Use D Port as Input

   ADCSRA=0xe7; //1110 0111
   ADMUX=0x40;
}

void initServo() // Initialize Servo Motor
{
	DDRE |= 0x10;
    // mode select : #14 Fast PWM
    TCCR3A |= (1 << WGM31) | (0 << WGM30);
    TCCR3B |= (1 << WGM33) | (1 << WGM32);
    // Fast PWM output pin mode    
    TCCR3A |= (1 << COM3B1) | (0 << COM3B0);
    // set timer clock scale
    TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30); 
    ICR3 = 6000;
	
    DDRB=0x20;   // PB5 out 
    TCCR1A=0x82; TCCR1B=0x1A; OCR1A=3000; ICR1=19999; // OCR1A -> OC Clear / Fast PWM TOP = ICR1 / 8╨паж
}

void initFND() // Initialize FND
{
   DDRC = 0xff;
   DDRG = 0x0f;
}

void couchServo1() // couch servomotor 1
{
	OCR3B = 1000;
}

void couchServo2() // couch servomotor 2
{
	OCR1A = 1000;
}

void standServo1() // stand up servomotor 1
{
	OCR3B = 3000;
}

void standServo2() // stand up servomotor 2
{
	OCR1A = 3000;
}

void readyForLazer()
{
	int i, j, k;
	int a;
	int time = 60; // time limit
	int cnt = 0; // correct
	int up = 0; // using servomotor & adc

	i = PINF; // read PIN F
	a = PIND; // read PIN D

	while(1)
	{

		for (k = 0; k < 2; k++) // twice
		{
			while(i > 5000) // error
			{
				i = PINF; // error 
			}
			i = PINF; // read PIN F
			a = PIND; // read PIN D
 
			j = rand() % 2; // random value ... 0 or 1

			if(up == 0) // if no servomotor up
			{
				if(j < 0.5) // if j < 0.5
				{
					standServo1(); // stand up servomotor 1
					up = 1; // using 1
				}
				else if (j > 0.5) // else
				{
					standServo2(); // stand up servomotor 2
					up = 2; // using 2
				}
			}

			if(i % 2 == 0 && up == 1) // if adc1 get lazer and using 1
			{
				couchServo1(); // couch servomotor 1
				cnt++; // increase cnt (correct)
				up = 0; // no servomotor up
			}

			if( a % 2 == 0 && up == 2) // if adc2 get lazer and using 2
			{
				couchServo2(); // couch servomotor 2
				cnt++; // increase cnt (correct)
				up = 0; // no servomotor up
			} 

			for(j = 0; j < 185; j++) // for 1000 ms
			{
				display(time, cnt); // display fnd

			}
		}

		time--; // decrease time

		if(time <= 0) // if game end
		{
			couchServo1(); // couch servomotor 1
			couchServo2(); // couch servomotor 2
			break; // break loop
		}

	}

	while(1)
	{
		display(0, cnt); // display cnt
	}
}

void display(int time, int cnt) // display fnd
{
	int i =0;
	fnd[3] = time / 10;
	fnd[2] = time % 10;
	fnd[1] = 11;
	fnd[0] = cnt;
	

	for(i = 0; i < 4; i++)
	{
	 PORTC = digit[fnd[3 - i]];
	 PORTG = fnd_sel[i];
	 _delay_ms(2);
	}
}

int main(void)
{
	adc_init(); // initialize adc
	initServo();  // initialize servomotor
	initFND(); // initiailize fnd

	couchServo1(); // couch servomotor 1
	couchServo2(); // couch servomotor 2
	_delay_ms(1000); // wait 1000 ms
	readyForLazer(); // start game
}
