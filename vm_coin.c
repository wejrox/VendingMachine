/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "vm_coin.h"

/**
 * Implement functions here for managing coins and the
 * "cash register" contained in the VmSystem struct.
 **/

/**
 * Some example functions:
 * init coins array, insert coin, change coin count for Denomination,
 * convert Denomination to coin value, deducting coins from register, etc...
 */

/** 
 * Inserts amount of coin given to the register
 **/
void insertCoin(VmSystem * system, Coin * coin)
{
	int i;
	for(i = 0; i < NUM_DENOMS; ++i)
	{
		if(system->cashRegister[i].denom == coin->denom)
		{
			changeCoinCount(&system->cashRegister[i], \
							system->cashRegister[i].count + coin->count);
			return;
		}
	}
}

/**
 * Removes amount of coin given from register
 **/
void removeCoin(VmSystem * system, Coin * coin)
{
	int i;
	for(i = 0; i < NUM_DENOMS; ++i)
	{
		if(system->cashRegister[i].denom == coin->denom)
		{
			changeCoinCount(&system->cashRegister[i], \
							system->cashRegister[i].count - coin->count);
			return;
		}
	}
}

/**
 * Changes the coin given's count to the value specified
 **/
void changeCoinCount(Coin * coin, int newCount)
{
	coin->count = newCount;
}

/**
 * Converts the denom enum given to the currency equivalent
 **/
int convertDenomToCoin(int denom)
{
	switch(denom)
	{
		case 0: return 5; break;
		case 1: return 10; break;
		case 2: return 20; break;
		case 3: return 50; break;
		case 4: return 100; break;
		case 5: return 200; break;
		case 6: return 500; break;
		case 7: return 1000; break;
	}

	return 0;
}

/**
 * Converts currency value given to the enum equivalent
 **/

int convertCoinToDenom(int coin)
{
	switch(coin)
	{
		case 5: return 0; break;
		case 10: return 1; break;
		case 20: return 2; break;
		case 50: return 3; break;
		case 100: return 4; break;
		case 200: return 5; break;
		case 500: return 6; break;
		case 1000: return 7; break;
	}

	return -1;
}

/**
 * Returns the string representation of an item's price
 **/
void textFormatPrice(char * price, int dollars, int cents)
{
	char tmpDollars[50];
	char tmpCents[50];
	/** Format the price **/
	strcpy(price, "$ ");
	sprintf(tmpDollars, "%d", dollars);
	strcat(price, tmpDollars);
	strcat(price, ".");

	/** Calculate cents **/
	if(cents == 5)
		sprintf(tmpCents, "%s", "05");
	else if(cents != 0)
		sprintf(tmpCents, "%d", cents);
	else
		sprintf(tmpCents, "%s", "00");

	strcat(price, tmpCents);
}

/**
 * Prints the change currently in the array 
 **/
void printChange(Coin * coins)
{
	int i, j;
	/** Print coins returning **/
	for(i = 0; i < NUM_DENOMS; ++i)
	{
		if(coins[i].count > 0)
		{
			for(j = 0; j < coins[i].count; ++j)
			{
				switch(coins[i].denom)
				{
					case TEN_DOLLARS: printf("$10 "); break;
					case FIVE_DOLLARS: printf("$5 "); break;
					case TWO_DOLLARS: printf("$2 "); break;
					case ONE_DOLLAR: printf("$1 "); break;
					case FIFTY_CENTS: printf("50c "); break;
					case TWENTY_CENTS: printf("20c "); break;
					case TEN_CENTS: printf("10c "); break;
					case FIVE_CENTS: printf("5c "); break;
				}
			}
		}
	}
	printf("\n");
}

/**
 * Removes the amount given from the remaining cost
 * Updates the price text
 * Returns false if anything fails
 **/
void handlePartPayment(int * remaining, int payment, char * price)
{
	int dollars, cents;
	char sCents[10];
	/** Remove from remaining cost **/
	*remaining -= payment;

	/** Transaction finished **/
	if(*remaining == 0)
	{
		strcpy(price, "$ 0.00");
		return;
	}

	/** Get dollars and cents **/
	dollars = *remaining / 100;
	sprintf(sCents, "%d", *remaining % 100);
	cents = atoi(sCents);

	/** Set the new price remaining text **/
	textFormatPrice(price, dollars, cents);
}

/**
 * Checks that the payment is a valid denomination
 * Returns true on success or false on failure
 **/
Boolean isPaymentValid(int payment)
{
	if(payment == 1000 || payment == 500 || payment == 200 || \
	   payment == 100  || payment == 50  || payment == 20  || \
	   payment == 10   || payment == 5)
		return TRUE;
	else
		return FALSE;
}

/**
 * Calculates the change required
 * Informs user 
 * Removes the change from cash register
 * If there aren't enough coins, returns false
 **/
Boolean calculateChange(int change, Coin * coinsGiven, VmSystem * system, Node * item)
{
	int i, tmpChange;
	Boolean outcome = TRUE, hasChange = FALSE;
	Coin changeDenoms[NUM_DENOMS];
	tmpChange = change;

	/** Initialise the change **/
	for(i = 0; i < NUM_DENOMS; ++i)
	{
		changeDenoms[i].denom = system->cashRegister[i].denom;
		changeDenoms[i].count = 0;
	}

	/** Add the coins given to our system **/
	for(i = 0; i < NUM_DENOMS; ++i)
		insertCoin(system, &coinsGiven[i]);

	/** Gives the most amount possible from each denomination **/
	fillChange(changeDenoms, system->cashRegister, &change, 1000, 7);
	fillChange(changeDenoms, system->cashRegister, &change, 500, 6);
	fillChange(changeDenoms, system->cashRegister, &change, 200, 5);
	fillChange(changeDenoms, system->cashRegister, &change, 100, 4);
	fillChange(changeDenoms, system->cashRegister, &change, 50, 3);
	fillChange(changeDenoms, system->cashRegister, &change, 20, 2);
	fillChange(changeDenoms, system->cashRegister, &change, 10, 1);
	fillChange(changeDenoms, system->cashRegister, &change, 5, 0);

	/** Ensure coins available for change **/
	for(i = 0; i < NUM_DENOMS; ++i)
		if(changeDenoms[i].count > system->cashRegister[i].count)
			outcome = FALSE;

	/** Make sure there's no change left to allocate **/
	if(change != 0)	outcome = FALSE;

	/** Remove the coins on failure as we are returning them **/
	if(!outcome)
	{
		printf("Not enough coins in the system to give change.\n");
		printf("We will now return your coins: ");

		printChange(coinsGiven);

		for(i = 0; i < NUM_DENOMS; ++i)
			system->cashRegister[i].count -= coinsGiven[i].count;
		return outcome;
	}

	/** Should we give change? **/
	for(i = 0; i < NUM_DENOMS; ++i)
		if(changeDenoms[i].count > 0)
			hasChange = TRUE;

	if(hasChange)
	{
		printf("Thank you. Here is your %s, and your change of $%d.%d: ",\
				item->data->name, tmpChange / 100, tmpChange % 100);

		printChange(changeDenoms);
		
		/** Remove change from register **/
		for(i = 0; i < NUM_DENOMS; ++i)
			system->cashRegister[i].count -= changeDenoms[i].count;
	}
	else
		printf("You have given us exact change, thank you!\n");

	return outcome;
}

/**
 * Uses as many coins as possible in the current tier.
 **/
void fillChange(Coin * changeDenoms, Coin * cashRegister, int * change, int tier, int denom)
{
	int pos;

	while(*change >= tier)
	{
		pos = getDenomPos(cashRegister, denom);

		/** Make sure there will be enough change of this amount **/
		if((cashRegister[pos].count - changeDenoms[pos].count - 1) <= 0)
			break;
		++changeDenoms[pos].count;
		*change -= tier;
	}
}

/** 
 * Returns the array index in the coin register 
 * which contains the payment amount specified 
 * Parameter required for whether or not the payment is in Enum form
 **/
int getDenomPos(Coin * cashRegister, int valEnum)
{
	int i;

	/** Get the position **/
	for(i = 0; i < NUM_DENOMS; ++i)
		if(cashRegister[i].denom == valEnum)
			return i;

	return -1;
}