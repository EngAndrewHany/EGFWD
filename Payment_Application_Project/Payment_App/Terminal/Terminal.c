/*
 * Terminal.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */

#include "..\Card\Card.h"
#include "Terminal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	printf ("Enter Transaction Date in format \" DD/MM/YYYY \": ");
	fflush(stdout);
	gets (termData->transactionDate);

	//check date length
	if (strlen(termData->transactionDate) != 10 )
	{
		return WRONG_DATE;
	}

	//check data expression if wrong or correct
	int i;
	for (i = 0; i < strlen(termData->transactionDate) ; i++)
	{
		if( ( (termData->transactionDate[3] == '0' && (termData->transactionDate[4] >= '1' && termData->transactionDate[4] <=  '9') )
				|| (termData->transactionDate[3] ==  '1' && (termData->transactionDate[4] >=  '0' && termData->transactionDate[4] <=  '2') ) )
				&& termData->transactionDate[2] ==  '/' && termData->transactionDate[5] ==  '/'
						&& (termData->transactionDate [6] >=  '0' && termData->transactionDate [6] <=  '9' )
						&& (termData->transactionDate [7] >=  '0' && termData->transactionDate [7] <=  '9' )
						&& (termData->transactionDate [8] >=  '0' && termData->transactionDate [8] <=  '9' )
						&& (termData->transactionDate [9] >=  '0' && termData->transactionDate [9] <=  '9' ))
		{
			continue;
		}
		else
		{
			return WRONG_DATE;
		}
	}
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	uint8_t Temp_date [] = {"0000"} ;
	int i , card_date , term_date;


	// check year
	for ( i=0 ; i<4 ; i++ )
	{
		Temp_date [i] = termData->transactionDate[i+6];
	}

	term_date = atoi(Temp_date);

	Temp_date[0] =  '2';
	Temp_date[1] =  '0';

	for ( i=2 ; i<4 ; i++ )
	{
		Temp_date [i] = cardData->cardExpirationDate[i+1];
	}
	card_date = atoi (Temp_date);

	if (card_date - term_date > 0)
	{
		return TERMINAL_OK ;
	}
	else if (card_date - term_date < 0)
	{
		return EXPIRED_CARD ;
	}
	else if (card_date - term_date == 0)
	{
		//check month
		for (i=0 ; i<4 ; i++)
		{
			Temp_date [i] = 0;
		}

		for ( i=0 ; i<2 ; i++ )
		{
			Temp_date [i] = termData->transactionDate[i+3];
		}

		term_date = atoi(Temp_date);

		for ( i=0 ; i<2 ; i++ )
		{
			Temp_date [i] = cardData->cardExpirationDate[i];
		}
		card_date = atoi (Temp_date);

		if (card_date - term_date < 0)
		{
			return EXPIRED_CARD ;
		}
	}
	return TERMINAL_OK ;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	printf ("Enter Transaction Amount: ");
	fflush(stdout);
	scanf ("%f" , &termData->transAmount);

	if ( termData->transAmount <= 0 )
	{
		return INVALID_AMOUNT;
	}
	return TERMINAL_OK ;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
	termData->maxTransAmount = maxAmount ;
	printf ("Enter Max Transaction Amount: ");
	fflush(stdout);
	scanf ("%f" , &termData->maxTransAmount);

	if ( termData->maxTransAmount <= 0 )
	{
		return INVALID_MAX_AMOUNT;
	}
	return TERMINAL_OK ;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	if (termData->maxTransAmount < termData->transAmount )
	{
		return EXCEED_MAX_AMOUNT ;
	}
	return TERMINAL_OK ;
}

void getTransactionDateTest(void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: getTransactionDateTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <4 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected input: wrong length \n");
			printf ("Expected output: WRONG_DATE \n");
			break;
		case 1:
			printf ("Expected input: use \\ \n");
			printf ("Expected output: WRONG_DATE \n");
			break;
		case 2:
			printf ("Expected input: wrong month \n");
			printf ("Expected output: WRONG_DATE \n");
			break;
		case 3:
			printf ("Expected input: right input \n");
			printf ("Expected output: TERMINAL_OK \n");
			break;
		}

		int error = getTransactionDate(ptr_terminalData);
		switch (error)
		{
		case 0:
			printf("actual result: TERMINAL_OK \n");
			break;
		case 1:
			printf("actual result: WRONG_DATE \n");
			break;
		}
		printf ("*********************************** \n");
	}
}

void isCardExpriedTest(void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;
	ST_cardData_t cardData;
	ST_cardData_t *ptr_cardData = &cardData ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: isCardExpriedTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected input: year expired \n");
			printf ("Expected output: EXPIRED_CARD \n");
			strcpy(terminalData.transactionDate , "15/10/2022");
			strcpy(cardData.cardExpirationDate , "05/19");
			break;
		case 1:
			printf ("Expected input: same year and expired month \n");
			printf ("Expected output: EXPIRED_CARD \n");
			strcpy(terminalData.transactionDate , "15/10/2022");
			strcpy(cardData.cardExpirationDate , "05/22");
			break;
		case 2:
			printf ("Expected input: valid card \n");
			printf ("Expected output: TERMINAL_OK \n");
			strcpy(terminalData.transactionDate , "15/10/2022");
			strcpy(cardData.cardExpirationDate , "05/24");
			break;
		}

		int error = isCardExpired(ptr_cardData , ptr_terminalData );
		switch (error)
		{
		case 0:
			printf("actual result: TERMINAL_OK \n");
			break;
		case 2:
			printf("actual result: EXPIRED_CARD \n");
			break;
		}
		printf ("*********************************** \n");
	}
}

void getTransactionAmountTest(void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: getTransactionAmountTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected input: negative amount \n");
			printf ("Expected output: INVALID_AMOUNT \n");
			break;
		case 1:
			printf ("Expected input: 0 Amount \n");
			printf ("Expected output: INVALID_AMOUNT \n");
			break;
		case 2:
			printf ("Expected input: positive amount \n");
			printf ("Expected output: TERMINAL_OK \n");
			break;
		}

		int error = getTransactionAmount(ptr_terminalData);
		switch (error)
		{
		case 0:
			printf("actual result: TERMINAL_OK \n");
			break;
		case 4:
			printf("actual result: INVALID_AMOUNT \n");
			break;
		}
		printf ("*********************************** \n");
	}
}

void setMaxAmountTest (void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: setMaxAmountTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <3 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected input: negative amount \n");
			printf ("Expected output: INVALID_MAX_AMOUNT \n");
			break;
		case 1:
			printf ("Expected input: 0 Amount \n");
			printf ("Expected output: INVALID_MAX_AMOUNT \n");
			break;
		case 2:
			printf ("Expected input: positive amount \n");
			printf ("Expected output: TERMINAL_OK \n");
			break;
		}

		int error = setMaxAmount(ptr_terminalData , 10000);
		switch (error)
		{
		case 0:
			printf("actual result: TERMINAL_OK \n");
			break;
		case 6:
			printf("actual result: INVALID_MAX_AMOUNT \n");
			break;
		}
		printf ("*********************************** \n");
	}
}

void isBelowMaxAmountTest(void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: isBelowMaxAmountTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <2 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			printf ("Expected input: above max amount \n");
			printf ("Expected output: EXCEED_MAX_AMOUNT \n");
			terminalData.transAmount = 50000;
			terminalData.maxTransAmount = 10000;
			break;
		case 1:
			printf ("Expected input: below max amount \n");
			printf ("Expected output: TERMINAL_OK \n");
			terminalData.transAmount = 5000;
			terminalData.maxTransAmount = 10000;
			break;
		}

		int error = isBelowMaxAmount(ptr_terminalData);
		switch (error)
		{
		case 0:
			printf("actual result: TERMINAL_OK \n");
			break;
		case 5:
			printf("actual result: EXCEED_MAX_AMOUNT \n");
			break;
		}
		printf ("*********************************** \n");
	}
}
