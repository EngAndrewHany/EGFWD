/*
 * Simple traffic light.c
 *
 * Created: 2/15/2023 3:00:29 PM
 * Author : Andrew Hany Kamel
 */ 

#define F_CPU 1000000U
#include "DIO.h"
#include <util/delay.h>

int main(void)
{
	int x = 0 ;
	// set output pins for 3 led of traffic light
	for ( x ; x <3 ; x++)
	{
		DIO_init(x , PORT_A , OUT);
	}
	
	//set output pins for 2 led of cross road 
	for ( x = 0 ; x <2 ; x++)
	{
		DIO_init(x , PORT_C , OUT);
	}

	
    while (1) 
    {
		//code for traffic light each led on for 1 sec (red >> yellow >> green) & if red cross & if green do not cross
	for ( x = 0 ; x <3 ; x++)
	{
		if (x == 0)
		{
			DIO_write(1 , PORT_C , LOW);
			DIO_write(0 , PORT_C , HIGH);
		}
		else if (x !=0 )
		{
			DIO_write(0 , PORT_C , LOW);
			DIO_write(1 , PORT_C , HIGH);
		}
		DIO_write(x , PORT_A , HIGH);
		_delay_ms(2000);
		DIO_write(x , PORT_A , LOW);
	}	
	}
}

