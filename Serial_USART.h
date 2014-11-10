/*
 * Serial_USART.h
 *
 * Created: 10/18/2014 11:46:13 PM
 *  Author: jpagel
 */ 


#ifndef SERIAL_USART_H_
#define SERIAL_USART_H_

#define F_CPU 16000000UL
#define BAUD 9600

#include "ProgMemData.h"

class Serial
{
	public:
	void sendChar(char c);
	void sendString(char *text);
	char usart_receiveByte();
	void SerialBegin();
	void SerialBeginXMEGA(baud_t baud);
	};








#endif /* SERIAL_USART_H_ */