/*
 * Terminal.h
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */

//file guard
#ifndef TERMINAL_TERMINAL_H_
#define TERMINAL_TERMINAL_H_

// terminal info structure
typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

// terminal states
typedef enum EN_terminalError_t
{
	TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t ;

// terminal prototype functions
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
void getTransactionDateTest(void);
void isCardExpriedTest(void);
void getTransactionAmountTest(void);
void setMaxAmountTest(void);
void isBelowMaxAmountTest(void);

#endif /* TERMINAL_TERMINAL_H_ */
