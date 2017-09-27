/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#ifndef VM_COIN_H
#define VM_COIN_H

#include "vm_system.h"

/**
 * The default coin level used when resetting values.
 **/
#define DEFAULT_COIN_COUNT 20

#define COIN_DELIM ","

void insertCoin(VmSystem * system, Coin * coin);
void removeCoin(VmSystem * system, Coin * coin);
void changeCoinCount(Coin * coin, int newCount);
int convertDenomToCoin(int denom);
int convertCoinToDenom(int coin);
void textFormatPrice(char * price, int dollars, int cents);
void printChange(Coin * coins);
void handlePartPayment(int * remaining, int payment, char * price);
Boolean isPaymentValid(int payment);
Boolean calculateChange(int change, Coin * coinsGiven, VmSystem * system, Node * item);
void fillChange(Coin * changeDenoms, Coin * cashRegister, int * change, int tier, int denom);
int getDenomPos(Coin * cashRegister, int valEnum);
#endif
