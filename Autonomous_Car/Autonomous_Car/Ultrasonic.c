/*
* Ultrasonic.c
*
* Created: 11/26/2021 3:07:54 PM
*  Author: user
*/

#include "Ultrasonic.h"
/*
Fos = 16Mhz
Ftimer = 16/8 = 2Mhz
Ttimer = 1/Ftimer = 0.5*10^-6 s

D = 17150 * timerValue * Ttimer(0.5*10^-6) = 0.008575 * timerValue

*/
extern long OVF_Count;

double Ultrasonic_Read(void){
	sei();//enable globle interrupt
	TCNT1 = 0;
	OVF_Count = 0;
	TCCR1A = 0;//normal mode
	TCCR1B |= ((1<<ICES1)|(1<<CS11));//N = 8 & rising edge
	TIMSK |= (1<<TOV1);//enable the overflow interrupt
	TIFR |= (1<<TOV1);//clear the overflow flag
	TIFR |= (1<<ICF1);//clear the input capture flag
	
	DIO_WriteChannel(DIO_ChannelA0,STD_High);
	_delay_us(10);
	DIO_WriteChannel(DIO_ChannelA0,STD_Low);
	
	while ((TIFR & (1<<ICF1)) == 0);//waiting rising edge
	TCNT1 = 0;
	OVF_Count = 0;
	TCCR1B &=~ (1<<ICES1);//falling edge
	TIFR |= (1<<TOV1);//clear the flag
	TIFR |= (1<<ICF1);//clear the flag
	
	while ((TIFR & (1<<ICF1)) == 0);//waiting falling edge
	long count = ICR1 + (65535*OVF_Count);
	double distance = (double)count * 0.008575;
	return distance;
}