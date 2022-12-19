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
