/*
 * Card.h
 *
 *  Created on: Dec 13, 2022
 *      Author: Andrew Hany
 */

//file guard
#ifndef CARD_CARD_H_
#define CARD_CARD_H_

typedef char uint8_t ;

//card info structure
typedef struct ST_cardData_t
{
	uint8_t  cardHolderName[25];
	uint8_t  primaryAccountNumber[20];
	uint8_t  cardExpirationDate[6];
}ST_cardData_t;

//card states
typedef enum EN_cardError
{
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

//card prototype functions
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
void getCardHolderNameTest(void);
void getCardExpiryDateTest (void);
void getCardPANTest (void);



#endif /* CARD_CARD_H_ */
