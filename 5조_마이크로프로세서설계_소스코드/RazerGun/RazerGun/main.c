/*
 * RazerGun.c
 *
 * 2014920056 최주영
 * 2015920005 김대현
 * 
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
unsigned char digit[10] = {0x3F, 0x06, 0x5B, 0x4f, 0x66, 0x6D, 0x7D, 0x27,0x7F, 0x6F}; // FND 0~9
int main()
{
	int num = 9;	//초기 총알 갯수 9개 
	DDRE = 0xff;	//FND는 출력
	DDRF = 0xff;	//레이저 센서 출력
	DDRG = 0x00;	//스위치 입력
	PORTE = digit[num];	//FND숫자 출력
	while(1)
	{		
		if(num == 0){
			PORTA == 0x00;	//총알 갯수 0개면 레이저가 더 이상 나가지 않음
		}
		else if((PING & 0x01) == 0x00){	//0번 포트의 스위치를 입력 시에
			num = num - 1;	//총알 발사시, FND 1 감소
			PORTE = digit[num];
			PORTF = 0xff;	//레이저 발사
			_delay_ms(500);	//레이저 0.5초 유지
			PORTF = 0x00;	//레이저 종료
			while((PING & 0x01) == 0x00){	//스위치를 꾹 누를 때를 대비한 딜레이
				_delay_ms(10);
			}
		}
		else//누르지 않으면 레이저 0
		{	
			PORTF = 0x00;
		}
	}
}