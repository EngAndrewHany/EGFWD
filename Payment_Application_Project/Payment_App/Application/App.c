/*
 * App.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */
#include "..\Card\Card.h"
#include "..\Terminal\Terminal.h"
#include "..\Server\Server.h"
#include "App.h"
#include <stdio.h>
#include <string.h>

ST_accountsDB_t accountsDB[255] =
{
		{ 100000.0, BLOCKED, "11111111111111111" } ,
		{ 60000.0, RUNNING, "22222222222222222" } ,
		{ 2000.0, BLOCKED, "33333333333333333" } ,
		{ 3000.0, RUNNING, "44444444444444444" } ,
		{ 500.0, BLOCKED, "55555555555555555" } ,
		{ 600.0, RUNNING, "66666666666666666" } ,
};

ST_transaction_t transDB [255] = { 0 };

int main ()
{
	appStart();
	return 0 ;
}

void appStart(void)
{
	//card variables
	ST_cardData_t cardData;
	ST_cardData_t *ptr_cardData = &cardData ;
	EN_cardError_t CardState = CARD_OK ;

	//terminal variables
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;
	EN_terminalError_t TerminalState = TERMINAL_OK ;

	// ************************** Card Test ********************************

	//	getCardHolderNameTest();

	//	getCardExpiryDateTest();

	//	 getCardPANTest();

	// ************************** Terminal Test ********************************

	//	getTransactionDateTest();

	//	isCardExpriedTest();

	//	setMaxAmountTest();

	//	getTransactionAmountTest();

	//	isBelowMaxAmountTest();

	// ************************** Server Test ********************************

	//	recieveTransactionDataTest();

	//	isValidAccountTest();

	//	isBlockedAccountTest();

	//	isAmountAvailableTest();

	//	saveTransactionTest();

	//	listSavedTransactionsTest();

	int ch = 1 ;
	while ( ch == 1 )
	{
		// ************************** Card ********************************

		//	get holder name
		CardState = getCardHolderName(ptr_cardData);

		while (CardState == WRONG_NAME)
		{
			printf("WRONG_NAME \n");
			CardState = getCardHolderName(ptr_cardData);
		}

		// get expire date
		CardState = getCardExpiryDate(ptr_cardData);
		while (CardState == WRONG_EXP_DATE)
		{
			printf("WRONG_EXP_DATE \n");
			CardState = getCardExpiryDate(ptr_cardData);
		}

		// get Card PAN
		CardState = getCardPAN(ptr_cardData);
		while (CardState == WRONG_PAN)
		{
			printf("WRONG_PAN \n");
			CardState = getCardPAN(ptr_cardData);
		}

		CardState = CARD_OK ;
		printf("CARD_OK \n");
		printf ("*********************************** \n");

		// ************************** Terminal ********************************

		//get transaction date
		TerminalState = getTransactionDate(ptr_terminalData);
		while (TerminalState == WRONG_DATE)
		{
			printf("WRONG_DATE \n");
			TerminalState = getTransactionDate(ptr_terminalData);
		}

		//check if card expired
		TerminalState = isCardExpired(ptr_cardData,ptr_terminalData);
		while (TerminalState == EXPIRED_CARD)
		{
			printf("EXPIRED_CARD \n");
			TerminalState = getTransactionDate(ptr_terminalData);
		}

		//get Transaction Amount
		TerminalState = getTransactionAmount(ptr_terminalData);
		while (TerminalState == INVALID_AMOUNT)
		{
			TerminalState = getTransactionAmount(ptr_terminalData);
		}

		//set max Transaction amount
		TerminalState = setMaxAmount(ptr_terminalData , 10000);
		while (TerminalState == INVALID_MAX_AMOUNT)
		{
			TerminalState = setMaxAmount(ptr_terminalData , 10000);
		}

		//check amount difference
		TerminalState = isBelowMaxAmount(ptr_terminalData);

		if (TerminalState == EXCEED_MAX_AMOUNT)
		{
			printf("EXCEED_MAX_AMOUNT\n");
			TerminalState = setMaxAmount(ptr_terminalData , 10000);
		}

		TerminalState = TERMINAL_OK ;
		printf("TERMINAL_OK \n");
		printf ("*********************************** \n");

		// ************************** Server ********************************
		ST_transaction_t transData ;
		ST_transaction_t *ptr_transData = &transData ;
		transData.cardHolderData = cardData ;
		transData.terminalData = terminalData;
		EN_transState_t transState = APPROVED ;

		//get Transaction Data
		transState = recieveTransactionData(ptr_transData);
		switch (transState)
		{
		case 0:
			printf("APPROVED\n");
			printf ("*********************************** \n");
			break;
		case 1:
			printf("DECLINED_INSUFFECIENT_FUND\n");
			printf ("*********************************** \n");
			break;
		case 2:
			printf("DECLINED STOLEN CARD\n");
			printf ("*********************************** \n");
			break;
		case 3:
			printf("FRAUD_CARD\n");
			printf ("*********************************** \n");
			break;
		case 4:
			printf("INTERNAL_SERVER_ERROR\n");
			printf ("*********************************** \n");
			break;
		}

		//save Transaction
		printf("SAVED TRANSACTIONS\n");
		listSavedTransactions();
		printf("Press 1 to evaluate again: ");
		fflush(stdout);
		scanf ("%d",&ch);
		getchar();
	}
}
