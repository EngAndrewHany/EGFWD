/*
 * First code.c
 *
 * Created: 2/11/2023 7:53:50 PM
 * Author : Andrew Hany Kamel 
 */ 

#include "DIO/DIO.h"

int main(void)
{
	//1. set directions	
	DIO_init(4 , PORT_A , OUT);
		DIO_init(4 , PORT_B , OUT);

    while (1) 
    {
		//2. write values
		DIO_write(4 , PORT_A , HIGH);
		DIO_write(3 , PORT_B , LOW);
    }
	return 0 ;
}

