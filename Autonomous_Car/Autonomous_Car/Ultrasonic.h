/*
 * Ultrasonic.h
 *
 * Created: 11/26/2021 3:08:24 PM
 *  Author: user
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "DIO.h"
#define F_CPU 16000000UL
#include <util/delay.h>

double Ultrasonic_Read(void);

#endif /* ULTRASONIC_H_ */