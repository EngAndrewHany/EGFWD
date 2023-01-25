/*
 * Server.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */

#include "..\Card\Card.h"
#include "..\Terminal\Terminal.h"
#include "..\Server\Server.h"
#include <stdio.h>
#include <string.h>

int accRef = -1 ;
static int indexLastTrans = 0;
extern ST_accountsDB_t accountsDB [255];
extern ST_transaction_t transDB [255];

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
	int i ;
	EN_serverError_t serverState =  ACCOUNT_NOT_FOUND;

	for(i = 0 ; i <6 ; i++)
	{
		if (strcmp(cardData->primaryAccountNumber , accountsDB[i].primaryAccountNumber ) == 0 )
		{
			serverState = SERVER_OK ;
			accRef = i ;
		}
	}
	return serverState ;
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t serverState = SERVER_OK ;
	if ( accountRefrence [accRef].state == BLOCKED )
	{
		serverState = BLOCKED_ACCOUNT ;
	}
	return serverState ;
}

EN_serverError_t isAmountAvailable (ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
	EN_serverError_t serverState = SERVER_OK ;
	if ( accountRefrence [accRef].balance < termData->transAmount  )
	{
		serverState = LOW_BALANCE ;
	}
	return serverState ;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	static int x = 1 ;
	transData->transactionSequenceNumber = x ;
	//transData->transactionSequenceNumber++;
	x++ ;
	transDB[indexLastTrans++] = *(transData);
	return SERVER_OK;
}

char* getTransStateS(EN_transState_t transState)
{
	char* ReturnStatus;
	switch ( transState )
	{
	case DECLINED_INSUFFECIENT_FUND:
		ReturnStatus = "DECLINED_INSUFFECIENT_FUND";
		break;
	case DECLINED_STOLEN_CARD:
		ReturnStatus = "DECLINED_STOLEN_CARD";
		break;
	case FRAUD_CARD :
		ReturnStatus = "FRAUD_CARD";
		break;
	case INTERNAL_SERVER_ERROR:
		ReturnStatus = "INTERNAL_SERVER_ERROR";
		break;
	default:
		ReturnStatus = "APPROVED";
		break;
	}
	return ReturnStatus ;
}

void listSavedTransactions(void)
{
	int i;
	for ( i = 0; i < 255 ; i++)
	{
		if (transDB[i].transactionSequenceNumber == 0)
		{
			continue;
		}
		else
		{
			char* transState = getTransStateS(transDB[i].transState);
			printf("#########################\n");
			printf ("Transaction Sequence Number is: %lu \n" , transDB[i].transactionSequenceNumber);
			printf("Transaction Date : %s \n", (transDB[i].terminalData).transactionDate );
			printf("Transaction Amount : %f \n", (transDB[i].terminalData).transAmount );
			printf("Transaction State : %s \n",transState);
			printf("Terminal Max Amount : %f \n", (transDB[i].terminalData).maxTransAmount);
			printf("Card holder Name : %s \n", (transDB[i].cardHolderData).cardHolderName );
			printf("PAN : %s \n", (transDB[i].cardHolderData).primaryAccountNumber );
			printf("Card Expiration Date : %s \n", (transDB[i].cardHolderData).cardExpirationDate );
			printf("#########################\n");
		}
	}
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	EN_transState_t transState = APPROVED ;

	//check Transaction Data valid or not
	if ( isValidAccount( &(transData->cardHolderData) , accountsDB ) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		transState= FRAUD_CARD;
	}
	else
	{
		if ( isBlockedAccount( accountsDB ) == BLOCKED_ACCOUNT)
		{
			transData->transState = DECLINED_STOLEN_CARD;
			transState = DECLINED_STOLEN_CARD;
		}
		else
		{
			if (isAmountAvailable((&transData->terminalData), &accountsDB[accRef]) == LOW_BALANCE)
			{
				transData->transState = DECLINED_INSUFFECIENT_FUND;
				transState = DECLINED_INSUFFECIENT_FUND;
			}
			else
			{
				//update  balance if approved
				transData->transState = APPROVED;
				accountsDB[accRef].balance -= (transData->terminalData).transAmount;
			}
		}
	}

	//check if save transaction failed or not
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		transState = INTERNAL_SERVER_ERROR;
	}
	return transState ;
}

void recieveTransactionDataTest(void)
{
	ST_transaction_t transData ;
	ST_transaction_t *ptr_transData = &transData ;
	EN_transState_t transState = APPROVED ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: recieveTransactionDataTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <4 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:

			//card data
			strcpy(transData.cardHolderData.primaryAccountNumber , "12345678911234567" ) ;
			strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
			strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

			//Terminal data
			strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
			transData.terminalData.transAmount = 500 ;
			transData.terminalData.maxTransAmount = 10000 ;

			printf ("Expected input: Account not in Data Base (PAN = 12345678911234567) \n");
			printf ("Expected output: FRAUD_CARD \n");
			break;

		case 1:

			//card data
			strcpy(transData.cardHolderData.primaryAccountNumber , "11111111111111111" ) ;
			strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
			strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

			//Terminal data
			strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
			transData.terminalData.transAmount = 500 ;
			transData.terminalData.maxTransAmount = 10000 ;

			printf ("Expected input: Blocked Account (PAN = 11111111111111111)\n");
			printf ("Expected output: DECLINED_STOLEN_CARD \n");
			break;

		case 2:

			//card data
			strcpy(transData.cardHolderData.primaryAccountNumber , "66666666666666666" ) ;
			strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
			strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

			//Terminal data
			strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
			transData.terminalData.transAmount = 1000 ;
			transData.terminalData.maxTransAmount = 10000 ;

			printf ("Expected input: amount not available (1000 LE from PAN = 66666666666666666 )\n");
			printf ("Expected output: DECLINED_INSUFFECIENT_FUND \n");
			break;

		case 3:

			//card data
			strcpy(transData.cardHolderData.primaryAccountNumber , "22222222222222222" ) ;
			strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
			strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

			//Terminal data
			strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
			transData.terminalData.transAmount = 1000 ;
			transData.terminalData.maxTransAmount = 10000 ;

			printf ("Expected input: Approved (1000 LE from PAN = 22222222222222222) \n");
			printf ("Expected output: APPROVED \n");
			break;
		}

		//get Transaction Data
		transState = recieveTransactionData(ptr_transData);
		switch (transState)
		{
		case 0:
			printf("Actual Result: APPROVED\n");
			printf ("*********************************** \n");
			break;
		case 1:
			printf("Actual Result: DECLINED_INSUFFECIENT_FUND\n");
			printf ("*********************************** \n");
			break;
		case 2:
			printf("Actual Result: DECLINED STOLEN CARD\n");
			printf ("*********************************** \n");
			break;
		case 3:
			printf("Actual Result: FRAUD_CARD\n");
			printf ("*********************************** \n");
			break;
		case 4:
			printf("Actual Result: INTERNAL_SERVER_ERROR\n");
			printf ("*********************************** \n");
			break;
		}
	}
}

void isValidAccountTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t *ptr_cardData = &cardData ;
	EN_serverError_t ServerState = SERVER_OK ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: isValidAccountTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <2 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:

			//card data
			strcpy(cardData.primaryAccountNumber , "12345678911234567" ) ;

			printf ("Expected input: Account not in Data Base (PAN = 12345678911234567) \n");
			printf ("Expected output: ACCOUNT_NOT_FOUND \n");
			break;

		case 1:

			//card data
			strcpy(cardData.primaryAccountNumber , "22222222222222222" ) ;

			printf ("Expected input: Account in Data Base (PAN = 22222222222222222) \n");
			printf ("Expected output: SERVER_OK \n");
			break;
		}

		ServerState = isValidAccount (ptr_cardData , accountsDB );
		switch (ServerState)
		{
		case 0:
			printf("Actual Result: SERVER_OK\n");
			printf ("*********************************** \n");
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			printf("Actual Result: ACCOUNT_NOT_FOUND\n");
			printf ("*********************************** \n");
			break;

		case 4:
			break;
		case 5:
			break;

		}
	}
}

void isBlockedAccountTest(void)
{
	ST_cardData_t cardData;
	EN_serverError_t ServerState = SERVER_OK ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: isBlockedAccountTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <2 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:

			//card data
			strcpy(cardData.primaryAccountNumber , "11111111111111111" ) ;
			accRef = 0;

			printf ("Expected input: Account Blocked (PAN = 11111111111111111) \n");
			printf ("Expected output: BLOCKED_ACCOUNT \n");
			break;

		case 1:

			//card data
			strcpy(cardData.primaryAccountNumber , "22222222222222222" ) ;
			accRef = 1;

			printf ("Expected input: Account running (PAN = 22222222222222222) \n");
			printf ("Expected output: SERVER_OK \n");
			break;
		}

		ServerState = isBlockedAccount ( accountsDB  );
		switch (ServerState)
		{
		case 0:
			printf("Actual Result: SERVER_OK\n");
			printf ("*********************************** \n");
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;

		case 5:
			printf("Actual Result: BLOCKED_ACCOUNT\n");
			printf ("*********************************** \n");
			break;
		}
	}
}

void isAmountAvailableTest(void)
{
	ST_terminalData_t terminalData ;
	ST_terminalData_t *ptr_terminalData = &terminalData;
	EN_serverError_t ServerState = SERVER_OK ;
	accRef = 5 ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: recieveTransactionDataTest \n");
	printf ("*********************************** \n");

	for (int i = 0 ; i <2 ; i++)
	{
		printf ("test case %d \n", i+1);

		switch (i)
		{
		case 0:
			terminalData.transAmount = 10000 ;

			printf ("Expected input: Amount not Available (1000 LE from PAN = 66666666666666666) \n");
			printf ("Expected output: LOW_BALANCE \n");
			break;

		case 1:
			terminalData.transAmount = 300 ;

			printf ("Expected input: amount available (300 LE from PAN = 66666666666666666 )\n");
			printf ("Expected output: SERVER_OK \n");
			break;
		}

		ServerState = isAmountAvailable(ptr_terminalData , accountsDB );
		switch (ServerState)
		{
		case 0:
			printf("Actual Result: SERVER_OK\n");
			printf ("*********************************** \n");
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			printf("Actual Result: LOW_BALANCE\n");
			printf ("*********************************** \n");
			break;
		case 5:
			break;
		}
	}
}

void saveTransactionTest(void)
{
	ST_transaction_t transData ;
	ST_transaction_t *ptr_transData = &transData ;
	EN_serverError_t ServerState = SERVER_OK ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: recieveTransactionDataTest \n");
	printf ("*********************************** \n");

	//card data
	strcpy(transData.cardHolderData.primaryAccountNumber , "22222222222222222" ) ;
	strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
	strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

	//Terminal data
	strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
	transData.terminalData.transAmount = 500 ;
	transData.terminalData.maxTransAmount = 10000 ;

	ServerState = saveTransaction(ptr_transData);

	switch (ServerState)
	{
	case 0:
		printf("Actual Result: SERVER_OK\n");
		printf ("*********************************** \n");
		break;
	case 1:
		printf("Actual Result: SAVING_FAILED\n");
		printf ("*********************************** \n");
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	}
	listSavedTransactions();
}

void listSavedTransactionsTest(void)
{
	ST_transaction_t transData ;
	ST_transaction_t *ptr_transData = &transData ;

	printf ("Tester name: Andrew Hany \n");
	printf ("function name: recieveTransactionDataTest \n");
	printf ("*********************************** \n");

	//card data
	strcpy(transData.cardHolderData.primaryAccountNumber , "22222222222222222" ) ;
	strcpy(transData.cardHolderData.cardHolderName , "Andrew Hany Kamel Hany" ) ;
	strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

	//Terminal data
	strcpy(transData.terminalData.transactionDate , "10/10/2022" ) ;
	transData.terminalData.transAmount = 500 ;
	transData.terminalData.maxTransAmount = 10000 ;

	saveTransaction(ptr_transData);

	//card data
	strcpy(transData.cardHolderData.primaryAccountNumber , "66666666666666666" ) ;
	strcpy(transData.cardHolderData.cardHolderName , "Omar Ahmed Essam Amr" ) ;
	strcpy(transData.cardHolderData.cardExpirationDate , "05/25" ) ;

	//Terminal data
	strcpy(transData.terminalData.transactionDate , "04/12/2022" ) ;
	transData.terminalData.transAmount = 300 ;
	transData.terminalData.maxTransAmount = 10000 ;

	saveTransaction(ptr_transData);
	listSavedTransactions();
}
