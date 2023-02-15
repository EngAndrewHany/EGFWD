/*
 * DIO.h
 *
 * Created: 2/12/2023 9:42:40 PM
 *  Author: andre
 */ 


#ifndef DIO_H_
#define DIO_H_

#include "registers.h"

//internal driver typedefs

//driver macros

//define ports for switch 
#define  PORT_A  'A'
#define  PORT_B  'B'
#define  PORT_C  'C'
#define  PORT_D  'D'

//define input or output
#define IN 0
#define OUT 1

//define value low or high
#define LOW 0
#define HIGH 1

//driver functions prototypes 
void DIO_init (uint8_t pinNumber , uint8_t portNumber , uint8_t direction); // Initialize DIO direction

void DIO_write (uint8_t pinNumber , uint8_t portNumber , uint8_t value); // Write data on DIO

void DIO_toggle (uint8_t pinNumber , uint8_t portNumber); // Toggle DIO 

void DIO_read (uint8_t pinNumber , uint8_t portNumber , uint8_t *value); // Read DIO value


#endif /* DIO_H_ */