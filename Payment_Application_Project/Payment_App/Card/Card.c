/*
 * Card.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */

#include <stdio.h>
#include <string.h>
#include "Card.h"

ST_cardData card_data;

int main ()
{
//	getCardHolderNameTest();

	ST_cardData *ptr_card_data = &card_data ;

	//get holder name
	int error = getCardHolderName(ptr_card_data);
	if (error == 1)
	{
		printf("WRONG_NAME \n");
		return 0 ;
	}

//	getCardExpiryDateTest();

	//get expire date
	error = getCardExpiryDate(ptr_card_data);
	if (error == 2)
	{
		printf("WRONG_EXP_DATE \n");
		return 0 ;
	}

//	getCardPANTest();

	//get Card PAN
	error = getCardPAN(ptr_card_data);
	if (error == 3)
	{
		printf("WRONG_PAN \n");
		return 0 ;
	}
	else
	{
		printf("CARD_OK \n");
	}
}

EN_cardError getCardHolderName(ST_cardData *cardData)
{
	printf ("Enter Your name: ");
	fflush(stdout);
	gets (card_data.cardHolderName);

	if (strlen(card_data.cardHolderName) < 20 || strlen(card_data.cardHolderName) > 25  )
	{
		return WRONG_NAME;
	}
	else
	{
		return CARD_OK;
	}
}

void getCardHolderNameTest(void)
{
	ST_cardData *ptr_card_data = &card_data ;

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("Tester name: Andrew Hany \n");
		printf ("function name: getCardHolderName \n");
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected output: WRONG_NAME \n");
			break;
		case 1:
			printf ("Expected output: WRONG_NAME \n");
			break;
		case 2:
			printf ("Expected output: Card_OK \n");
			break;
		}

		int error = getCardHolderName(ptr_card_data);
		switch (error)
		{
		case 0:
			printf("actual result: Card OK \n");
			break;
		case 1:
			printf("actual result: Wrong name \n");
			break;
		}
		printf("************************\n");
	}
}

EN_cardError getCardExpiryDate(ST_cardData *cardData)
{
	printf ("Enter Expire Date in format \"MM/YY\": ");
	fflush(stdout);
	gets (card_data.cardExpirationDate);

	if (strlen(card_data.cardExpirationDate) != 5 )
	{
		return WRONG_EXP_DATE;
	}
	else
	{
		return CARD_OK;
	}
}

void getCardExpiryDateTest (void)
{
	ST_cardData *ptr_card_data = &card_data ;

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("Tester name: Andrew Hany \n");
		printf ("function name: getCardExpiryDate \n");
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected output: WRONG_EXP_DATE \n");
			break;
		case 1:
			printf ("Expected output: WRONG_EXP_DATE \n");
			break;
		case 2:
			printf ("Expected output: Card_OK \n");
			break;
		}

		int error = getCardExpiryDate(ptr_card_data);
		switch (error)
		{
		case 0:
			printf("actual result: Card_OK \n");
			break;
		case 2:
			printf("actual result: WRONG_EXP_DATE \n");
			break;
		}
		printf("************************\n");
	}
}

EN_cardError getCardPAN(ST_cardData *cardData)
{
	printf ("Enter Card PAN: ");
	fflush(stdout);
	gets (card_data.primaryAccountNumber);

	if (strlen(card_data.primaryAccountNumber) < 16 || strlen(card_data.primaryAccountNumber) > 19  )
	{
		return WRONG_PAN;
	}
	else
	{
		return CARD_OK;
	}
}

void getCardPANTest(void)
{
	ST_cardData *ptr_card_data = &card_data ;

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("Tester name: Andrew Hany \n");
		printf ("function name: getCardPAN \n");
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected output: WRONG_PAN \n");
			break;
		case 1:
			printf ("Expected output: WRONG_PAN \n");
			break;
		case 2:
			printf ("Expected output: Card_OK \n");
			break;
		}

		int error = getCardPAN(ptr_card_data);
		switch (error)
		{
		case 0:
			printf("actual result: Card_OK \n");
			break;
		case 3:
			printf("actual result: WRONG_PAN \n");
			break;
		}
		printf("************************\n");
	}
}