/*
 * Ultrasonic_E25.c
 *
 * Created: 11/26/2021 3:04:59 PM
 * Author : user
 */

#include "LCD.h"
//ULTRASONIC
#include "Ultrasonic.h"

volatile long OVF_Count = 0;

//Servo
#define MinAngle 0
#define MaxAngle 180
#define MinCount 250
#define MaxCount 500

void Servo(long angle);
void MoveForward();
void MoveBackward();
void MoveLeft();
void MoveRight();

int main(void) {
	DIO_Init();
	LCD_Init();

	//ULTRASONIC
	double distance = 0;
	char string[30];

	//SERVO
	DDRD |= (1 << 7);
	TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (1 << CS22);
	OCR2 = 255;
	int current_angle = 0;
	int angles[4] = { 0, 45, 90, 180 };

	//DC MOTOR
	DDRD |= (1 << 5); //OCR1
	DDRC |= (1 << 5) | (1 << 6); //Motor1
	DDRC |= (1 << 3) | (1 << 4); //Motor2
	TCCR1A = (1 << WGM11) | (1 << COM1A1); // MODE 14 FAST PWM, non inverting
	TCCR1B = (1 << WGM12) | (1 << WGM13);
	OCR1A = 1000;
	ICR1 = 65535;

	while (1) {
		Servo(angles[current_angle]);
		if (current_angle == 3) {
			current_angle = -1;
		}
		current_angle++;

		distance = Ultrasonic_Read();
		dtostrf(distance, 2, 2, string); //convert from double to string
		LCD_Cmd(0x01); //clear LCD
		LCD_String("Distance = ");
		LCD_String(string);

		if (distance <= 20) {
			if ((current_angle == 0) | (current_angle == 45)) {
				MoveBackward();
				MoveLeft();
			} else {
				MoveBackward();
				MoveRight();
			}

		}
	}
}

ISR(TIMER1_OVF_vect) {
	OVF_Count++;
}
void Servo(long angle) {
	OCR2 =
			(((angle - MinAngle) * (MaxCount - MinCount))
					/ (MaxAngle - MinAngle)) + MinCount - 1;
}
void MoveForward() {
	PORTC |= (1 << 5); //L
	PORTC &= ~(1 << 6);
	PORTC |= (1 << 3); //L
	PORTC &= ~(1 << 4);
	_delay_ms(500);
	PORTC &= ~((1 << 6) | (1 << 5) | (1 << 3) | (1 << 4)); //stop
}
void MoveBackward() {
	PORTC |= (1 << 6); //L
	PORTC &= ~(1 << 5);
	PORTC |= (1 << 4); //L
	PORTC &= ~(1 << 3);
	_delay_ms(500);
	PORTC &= ~((1 << 6) | (1 << 5) | (1 << 3) | (1 << 4)); //stop
}
void MoveLeft() {
	PORTC |= (1 << 3); //L
	PORTC &= ~(1 << 4);
	_delay_ms(500);
	PORTC &= ~((1 << 6) | (1 << 5) | (1 << 3) | (1 << 4)); //stop
}
void MoveRight() {
	PORTC |= (1 << 5); //L
	PORTC &= ~(1 << 6);
	_delay_ms(500);
	PORTC &= ~((1 << 6) | (1 << 5) | (1 << 3) | (1 << 4)); //stop
}

