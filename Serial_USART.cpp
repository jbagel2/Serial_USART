/*
 * Serial_USART.cpp
 *
 * Created: 10/18/2014 11:45:25 PM
 *  Author: jpagel
 */ 


#include <avr/io.h>
#include "Serial_USART.h"
#include "ProgMemData.h"
#include <util/setbaud.h>

#ifndef F_CPU
#error You Must declare F_CPU
#endif

#ifndef BAUD
#error You must define BAUD to your desired BAUDrate
#endif

int main(void)
{
	
}

#if defined(__AVR_ATXMEGA128D3__) || defined(__AVR_ATXMEGA64A1U__) //Really any XMEGA Since they are all pretty much much compatible minus USB variations.

void Serial::sendChar(char c)
{
	
	while( !(USARTC0_STATUS & USART_DREIF_bm) ); //Wait until DATA buffer is empty
	
	USARTC0_DATA = c;
	
}

void Serial::sendString(char *text)
{
	while(*text)
	{
		sendChar(*text++);
	}
}

char Serial::receiveByte() //Blocking
{
	while( !(USARTC0_STATUS & USART_RXCIF_bm) ); //Interesting DRIF didn't work.
	return USARTC0_DATA;
}

void Serial::SerialBeginXMEGA(baud_t baud)
{
	//baud_t temp = baud;
	
	int16_t scaler = baud.Scaler;
	int32_t bsel = baud.Bsel;
	int16_t doubleSpeed = baud.DoubleSpeed;
	
	USARTC0_BAUDCTRLB = USARTC0.BAUDCTRLB = 0| (bsel >> 8) | (scaler << USART_BSCALE0_bp);
	USARTC0_BAUDCTRLA = bsel;
	
	//Disable interrupts, just for safety
	USARTC0_CTRLA = 0;
	//8 data bits, no parity and 1 stop bit
	USARTC0_CTRLC = USART_CHSIZE_8BIT_gc;
	
	//Enable receive and transmit
	
	if(doubleSpeed)
	{
		USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm | USART_CLK2X_bm;
	}
	else
	{
		USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm | ~USART_CLK2X_bm;
	}


}

#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || defined(__AVR_ATmega168P)



#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168P__)

#undef UDR
#undef UCSRA
#undef UCSRB
#undef UDRE
#undef UBRRH
#undef UBRRL
#undef U2X
#undef RXEN
#undef TXEN
#undef TXC
#undef RXC
#define UDR UDR0
#define UCSRA UCSR0A
#define UCSRB UCSR0B
#define UDRE UDRE0
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define U2X U2X0
#define RXEN RXEN0
#define TXEN TXEN0
#define TXC TXC0
#define RXC RXC0


#endif

void configureSerial()
{
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	UCSRA |= (1<<U2X);
	#else
	UCSRA &= ~(1<<U2X);
	#endif
	
}

void startSerial()
{
	UCSRB |= (1<<RXEN) | (1<<TXEN);	
}


void Serial::sendChar(char c) {
	UDR = c;
	loop_until_bit_is_set(UCSRA, TXC); /* Wait until transmission ready. */
}

void Serial::sendString(char *text)
{
	
	while(*text)
	{
		sendChar(*text++)	;
	}
}

char receiveByte() {
	loop_until_bit_is_set(UCSRA, RXC); /* Wait until data exists. */
	return UDR;
}




	void Serial::SerialBegin()
	{
		configureSerial();
		startSerial();
		
	}

#endif

