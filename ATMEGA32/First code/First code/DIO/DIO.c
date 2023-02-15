/*
 * DIO.c
 *
 * Created: 2/12/2023 9:42:28 PM
 *  Author: Andrew Hany Kamel
 */ 

/************************************************************************/
/* include .h files                                                     */
/************************************************************************/
#include "DIO.h"



/************************************************************************/
/* global variables                                                     */
/************************************************************************/



/************************************************************************/
/* function definitions                                                 */
/************************************************************************/
void DIO_init (uint8_t pinNumber , uint8_t portNumber , uint8_t direction)
{
	switch (portNumber)
	{
		// PORTA
		case PORT_A:
			if (direction == IN)
			{
				DDRA &= ~(1<<pinNumber);
			}
			else
			{
				DDRA |= (1<<pinNumber);
			}
			break;
		
		//PORTB
		case PORT_B:
			if (direction == IN)
			{
				DDRB &= ~(1<<pinNumber);
			}
			else
			{
				DDRB |= (1<<pinNumber);
			}
			break;
		
		//PORTC
		case PORT_C:
			if (direction == IN)
			{
				DDRC &= ~(1<<pinNumber);
			}
			else
			{
				DDRC |= (1<<pinNumber);
			}
			break;
		
		//PORTD
		case PORT_D:
			if (direction == IN)
			{
				DDRD &= ~(1<<pinNumber);
			}
			else
			{
				DDRD |= (1<<pinNumber);
			}
			break;
	}
}

void DIO_write (uint8_t pinNumber , uint8_t portNumber , uint8_t value)
{
	switch (portNumber)
	{
		// PORTA
		case PORT_A:
		if (value == LOW)
		{
			PORTA &= ~(1<<pinNumber);	//write 0
		}
		else
		{
			PORTA |= (1<<pinNumber);	//write 1
		}
		break;
		
		//PORTB
		case PORT_B:
		if (value == LOW)
		{
			PORTB &= ~(1<<pinNumber);	//write 0
		}
		else
		{
			PORTB |= (1<<pinNumber);	//write 1
		}
		break;
		
		//PORTC
		case PORT_C:
		if (value == LOW)
		{
			PORTC &= ~(1<<pinNumber);	//write 0
		}
		else
		{
			PORTC |= (1<<pinNumber);	//write 1
		}
		break;
		
		//PORTD
		case PORT_D:
		if (value == LOW)
		{
			PORTD &= ~(1<<pinNumber);	//write 0
		}
		else
		{
			PORTD |= (1<<pinNumber);	//write 1
		}
		break;
	}	
}

void DIO_toggle (uint8_t pinNumber , uint8_t portNumber); // Toggle DIO

void DIO_read (uint8_t pinNumber , uint8_t portNumber , uint8_t *value); // Read DIO value