/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "vm_options.h"

/**
 * vm_options.c this is where you need to implement the system handling
 * functions (e.g., init, free, load, save) and the main options for
 * your program. You may however have some of the actual work done
 * in functions defined elsewhere.
 **/

/**
 * Initialise the system to a known safe state. Look at the structure
 * defined in vm_system.h.
 **/
Boolean systemInit(VmSystem * system)
{
	/** Stock init (must free current pointer) **/
	initStockList(system);

    return FALSE;
}

/**
 * Free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind.
 **/
void systemFree(VmSystem * system)
{
	/** Free stock **/
	freeList(system->itemList);

	/** Free system **/
	free(system);
}

/**
 * Loads the stock and coin data into the system. You will also need to assign
 * the char pointers to the stock and coin file names in the system here so
 * that the same files will be used for saving. A key part of this function is
 * validation. A substantial number of marks are allocated to this function.
 **/
Boolean loadData(
    VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
	system->stockFileName = stockFileName;
	system->coinFileName = coinsFileName;

	if(!loadStock(system, system->stockFileName))
		return FALSE;
	else if(!loadCoins(system, system->coinFileName))
		return FALSE;

    return TRUE;
}

/**
 * Loads the stock file data into the system.
 **/
Boolean loadStock(VmSystem * system, const char * fileName)
{
	/** Data to process **/
	char ** fileContents = GetFileContents(fileName);
	/** After tokenisation of each line **/
	char ** tokenisedData;
	/** Counting **/
	int i,y,x = 0;
	int stockCount = 0;
	/** The amount of elements needed to split **/
	int arrElements = 6;
	int delCount = 0;
	int delReq = 4;
	/** Validation **/
	int eolCount = 0;
	char *tmp;
	/** Data entry **/
	Node *newNode = NULL;

	/** Count elements **/
	while(fileContents[stockCount] != '\0') { ++stockCount; }

	/** Set array size **/
	tokenisedData = malloc((arrElements + NULL_SPACE) * sizeof(char*));

	/** Exit if no stock or too much on one line **/
	if(stockCount == 0 || strlen(fileContents[0]) > DESC_LEN)
	{
		freeStringArray(fileContents);
		free(tokenisedData);
		free(newNode);
		return FALSE;
	}

	/** Perform validity tests on each line **/
	for(y = 0; y < stockCount; ++y, delCount = 0, eolCount = 0)
	{
		/** Check delimiters **/
		for(i = 0; i < strlen(fileContents[y]); ++i)
			if (fileContents[y][i] == STOCK_DELIM[0])
				++delCount;

		/** Wrong number of entries **/
		if(delCount != delReq)
		{
			freeStringArray(fileContents);
			free(tokenisedData);
			free(newNode);
			return FALSE;
		}

		/** Malloc **/
		tokenisedData[0] = malloc(sizeof(char) * (strlen(fileContents[y]) + \
			NULL_SPACE));

		/** Split line **/
		tmp = strtok(fileContents[y], STOCK_DELIM_COMB);
		strcpy(tokenisedData[0], tmp);

		for(i = 1; i < arrElements; ++i)
		{
			tmp = strtok(NULL, STOCK_DELIM_COMB);
			tokenisedData[i] = malloc(sizeof(char) * (strlen(tmp) + \
				NULL_SPACE));
			strcpy(tokenisedData[i], tmp);
		}

		/** Sentinel ('i' was incremented at the final for loop) **/
		tokenisedData[i] = malloc(sizeof(char) * (NULL_SPACE));
		strcpy(tokenisedData[i], "");

		/** Count each of the new line characters in each string **/
		for(i = 0; i < arrElements; ++i)
		{
			for(x = 0; x < strlen(tokenisedData[i]) + NULL_SPACE; ++x)
			{
				if(tokenisedData[i][x] == '\0')
					++eolCount;
			}
		}

		/** Overflow **/
		if(eolCount != arrElements)
		{
			freeStringArray(fileContents);
			free(tokenisedData);
			free(newNode);
			return FALSE;
		}

		/** Ensure strings not too large for the struct **/
		if(strlen(tokenisedData[0]) > ID_LEN || \
			strlen(tokenisedData[1]) > NAME_LEN || \
			strlen(tokenisedData[2]) > DESC_LEN || \
			strlen(tokenisedData[3]) > 9 || \
			strlen(tokenisedData[4]) > 9 || \
			strlen(tokenisedData[5]) > 9)
		{
			freeStringArray(fileContents);
			free(tokenisedData);
			free(newNode);
			return FALSE;
		}

		/** Create node **/
		newNode = createNode(tokenisedData);

		/** Add to the list **/
		pushNode(system->itemList, newNode); 

		/** Free part of array for next loop **/
		for(i = 0; i < arrElements + 1; ++i)
		{
			free(tokenisedData[i]);
			tokenisedData[i] = NULL;
		}
	}

	/** Free temp memory **/
	freeStringArray(fileContents);
	free(tokenisedData);

	/** Sort the list **/
	getListLength(system->itemList);
	sortStock(system->itemList);

    return TRUE;
}

/**
 * Loads the coin file data into the system.
 **/
Boolean loadCoins(VmSystem * system, const char * fileName)
{
	/** Count Variables **/
	int i,y;
	int lineCount = 0;
	/** Contents of file **/
	char ** fileContents = GetFileContents(fileName);
	/** Denomination Check **/
	char *denomSize;
	char *denomAmt;
	unsigned int denomSizeParsed;

	while(fileContents[lineCount] != '\0') { ++lineCount; }

	if(lineCount != NUM_DENOMS)
	{
		freeStringArray(fileContents);
		return FALSE;
	}

	/** Perform validity tests on each line **/
	for(y = 0; y < lineCount; ++y)
	{
		/** Check dilimeters **/
		for(i = 0; i < strlen(fileContents[y]); ++i)
		{
			if(fileContents[y][i] == COIN_DELIM[0])
				break;
			if(fileContents[y][i] == '\0')
			{
				freeStringArray(fileContents);
				return FALSE;
			}
		}

		/** Ensure valid denomination **/
		denomSize = strtok(fileContents[y], COIN_DELIM);
		denomAmt = strtok(NULL, COIN_DELIM);

		/** Check that there isn't more data than intended **/
		if(strtok(NULL, COIN_DELIM) != NULL)
		{
			freeStringArray(fileContents);
			return FALSE;
		}

		/** Format doesn't match **/
		if(strcmp(denomSize, "1000") != 0 && strcmp(denomSize, "500") != 0 && \
			strcmp(denomSize, "200") != 0 && strcmp(denomSize, "100") != 0 && \
			strcmp(denomSize, "50") != 0 && strcmp(denomSize, "20") != 0 && \
			strcmp(denomSize, "10") != 0 && strcmp(denomSize, "5") != 0)
		{
			freeStringArray(fileContents);
			return FALSE;
		}

		/** Validate Denomination **/
		if(!parseUInt(denomSize, &denomSizeParsed))
		{
			freeStringArray(fileContents);
			return FALSE;
		}

		switch(denomSizeParsed)
		{
			case 1000:
				system->cashRegister[y].denom = TEN_DOLLARS;
			break;
			case 500:
				system->cashRegister[y].denom = FIVE_DOLLARS;
			break;
			case 200:
				system->cashRegister[y].denom = TWO_DOLLARS;
			break;
			case 100:
				system->cashRegister[y].denom = ONE_DOLLAR;
			break;
			case 50:
				system->cashRegister[y].denom = FIFTY_CENTS;
			break;
			case 20:
				system->cashRegister[y].denom = TWENTY_CENTS;
			break;
			case 10:
				system->cashRegister[y].denom = TEN_CENTS;
			break;
			case 5:
				system->cashRegister[y].denom = FIVE_CENTS;
			break;
			default:
				freeStringArray(fileContents);
				return FALSE;
			break;
		}

		/** Validate Coin count **/
		if(!parseUInt(denomAmt, &system->cashRegister[y].count))
		{
			freeStringArray(fileContents);
			return FALSE;
		}
	}
	freeStringArray(fileContents);
    return TRUE;
}

/**
 * Saves all the stock back to the stock file.
 **/
Boolean saveStock(VmSystem * system)
{
	/** Counters **/
	int i;

	Node * node;

	FILE *fp;

	/** Overwrite file **/
	fp = fopen(system->stockFileName, "w");

	if (fp == NULL)
		return FALSE;

	/** Get each node from the system, and print to the file **/
	for(i = 0; i < system->itemList->size; ++i)
	{
		node = getNodeByPos(system->itemList, i);
		fprintf(fp, "%s|%s|%s|%d.%d|%d\n", \
			node->data->id, \
			node->data->name, \
			node->data->desc, \
			node->data->price.dollars, \
			node->data->price.cents, \
			node->data->onHand);
	}

	/** Close the file **/
	fclose(fp);

    return TRUE;
}

/**
 * Saves all the coins back to the coins file.
 **/
Boolean saveCoins(VmSystem * system)
{
	/** Counters **/
	int i, coinVal;

	FILE *fp;

	/** Overwrite file **/
	fp = fopen(system->coinFileName, "w");

	if (fp == NULL)
		return FALSE;

	coinVal = -1;
	/** Print each coin denom and amount **/
	for(i = 0; i <= NUM_DENOMS; ++i)
	{
		switch(system->cashRegister[i].denom)
		{
			case FIVE_CENTS: coinVal = 5; break;
			case TEN_CENTS: coinVal = 10; break;
			case TWENTY_CENTS: coinVal = 20; break;
			case FIFTY_CENTS: coinVal = 50; break;
			case ONE_DOLLAR: coinVal = 100; break;
			case TWO_DOLLARS: coinVal = 200; break;
			case FIVE_DOLLARS: coinVal = 500; break;
			case TEN_DOLLARS: coinVal = 1000; break;
			default: return FALSE;
		}

		fprintf(fp, "%d,%d\n", \
			coinVal, \
			system->cashRegister[i].count);
	}

	/** Close the file **/
	fclose(fp);

    return TRUE;
}

/**
 * This option allows the user to display the items in the system.
 * This is the data loaded into the linked list in the requirement 2.
 **/
void displayItems(VmSystem * system)
{
	int i, tableWidth;
	char price[10];
	/** Holds the width of the columns **/
	int colSizes[4];
	Node * stock;

	/** Get max sizes **/
	colSizes[0] = getMaxWidth(system->itemList, 0);
	colSizes[1] = getMaxWidth(system->itemList, 1);
	colSizes[2] = getMaxWidth(system->itemList, 3);
	colSizes[3] = getMaxWidth(system->itemList, 2);

	tableWidth = 10;
	for(i = 0; i < 4; ++i)
	{
		tableWidth += colSizes[i];
		tableWidth += 2;
	}

	printf("Items Menu\n\n");

	printf("%-*s | %-*s | %-*s | %-*s\n", \
		colSizes[0], "ID", \
		colSizes[1], "Name", \
		colSizes[2], "Available", \
		colSizes[3], "Price");

	/** Dashes **/
	for(i = 0; i < tableWidth; ++i)
	{
		printf("-");
	}
	printf("\n");


	/** Print all of the items **/
	for(i = 0; i < system->itemList->size; ++i)
	{
		stock = getNodeByPos(system->itemList, i);

		textFormatPrice(price, stock->data->price.dollars, \
							   stock->data->price.cents);

		/** Print the line **/
		printf("%-*s | %-*s | %-*d | %-*s\n", \
		colSizes[0], stock->data->id, \
		colSizes[1], stock->data->name, \
		9, stock->data->onHand, \
		colSizes[3], price);
	}
}

/**
 * This option allows the user to purchase an item.
 * This function implements requirement 5 of the assignment specification.
 **/
void purchaseItem(VmSystem * system)
{
	int i, iVal;
	char op[5 + NULL_SPACE];
	char price[10];
	Node * item;
	Coin coinsGiven[NUM_DENOMS];

	int dollars, cents, remaining, payment, change;
	change = -1;

	/** Initialise the coins **/
	for(i = 0; i < NUM_DENOMS; ++i)
	{
		coinsGiven[i].denom = system->cashRegister[i].denom;
		coinsGiven[i].count = 0;
	}

	printf("Purchase Item\n");
	printf("--------------\n");
	printf("Please enter the id of the item you wish to purchase: ");
	
	iVal = getInput(op, sizeof(op));
	if(iVal == 0)
	{
		printf("Input entered is invalid, cancelling transaction.\n");
		return;
	}
	else if (iVal == -1)
	{
		printf("Cancelling purchase\n");
		printf("Returning to main menu..\n");
		return;
	}

	/** Get the item with that id (if any) **/
	item = getNodeByID(system->itemList, op);
	if(!item)
	{
		printf("No item exists with the ID '%s', cancelling transaction.\n", \
				op);
		return;
	}
	else if(item->data->onHand < 1)
	{
		printf("Item with ID '%s' is out of stock, cancelling transaction.\n",\
				op);
		return;
	}

	/** Temporary holders for calculations **/
	dollars = item->data->price.dollars;
	cents = item->data->price.cents;
	remaining = (dollars * 100);
	remaining += cents;
	textFormatPrice(price, dollars, cents);

	/** Print description **/
	printf("You have selected \"%s  %s\". This will cost you %s.\n", \
		item->data->name, item->data->desc, price);

	printf("Please hand over the money - type in the value of each ");
	printf("note/coin in cents.\n");
	printf("Press enter on a new and empty line to cancel this purchase:\n");

	while(change < 0)
	{
		/** Reset change for check **/
		change = -1;
		printf("You still need to give us %s:", price);
		/** User input **/
		iVal = getInput(op, sizeof(op));
		if(iVal == 0)
			printf("Error: '%s' is not a valid denomination of money.\n", op);
		else if (iVal == -1)
		{
			printf("Cancelling purchase\n");
			printf("Returning to main menu...\n");
			return;
		}

		payment = atoi(op);
		if(!isPaymentValid(payment))
		{
			printf("Error: '%s' is not a valid denomination of money.\n", \
				   op);
			continue;
		}

		/** Calculate change **/
		if(payment > remaining)
			change = (remaining - payment) * -1;
		else if(payment == remaining)
			change = 0;

		handlePartPayment(&remaining, payment, price);
		coinsGiven[getDenomPos(system->cashRegister, \
				   convertCoinToDenom(payment))].count += 1;

		/** Finish transaction, give change **/
		if(change >= 0)
		{
			if(!calculateChange(change, coinsGiven, system, item))
			{
				printf("Not enough change to process your request.\n");
				printf("Returning you to the main menu.\n");
				return;
			}

			/** Reduce amount available **/
			item->data->onHand -= 1;

			printf("Please come back soon.\n");
		}
	}
}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{
	saveStock(system);
	saveCoins(system);

	printf("Goodbye\n");
	
	systemFree(system);
	exit(EXIT_SUCCESS);
}

/**
 * This option adds an item to the system. This function implements
 * requirement 7 of of assignment specification.
 **/
void addItem(VmSystem * system)
{ 
	int i, iVal;
	Node *newItem;
	Boolean priceValid = FALSE, dolExist = FALSE;
	char price[10], tmp[10];
	newItem = malloc(sizeof(Node));
	newItem->data = malloc(sizeof(Stock));
	getNewID(system->itemList, newItem->data->id);

	printf("This new meal item will have the Item id of %s.\n", newItem->data->id);

	printf("Enter the item name: ");
	while(TRUE)
	{
		iVal = getInput(newItem->data->name, NAME_LEN);
		if(iVal == 0)
			printf("Name is invalid, must be less than %d characters.\n", \
				NAME_LEN);
		else if (iVal == -1)
		{				
			printf("Cancelling new item\n");
			free(newItem->data);
			free(newItem);
			printf("Returning to main menu..\n");
			return;
		}
		else
			break;
	}

	printf("Enter the item description: ");
	while(TRUE)
	{
		iVal = getInput(newItem->data->desc, DESC_LEN);
		if(iVal == 0)
			printf("Description is invalid, must be less than %d characters.\n", \
				DESC_LEN);
		else if (iVal == -1)
		{
			printf("Cancelling new item\n");
			free(newItem->data);
			free(newItem);
			printf("Returning to main menu..\n");			
			return;
		}
		else
			break;
	}

	printf("Enter the price for this item: ");
	while(!priceValid)
	{
		while(TRUE)
		{
			iVal = getInput(price, sizeof(price));
			if(iVal == 0)
				printf("\nPrice entered is invalid, must be less than 10 characters: ");
			else if (iVal == -1)
			{
				printf("Cancelling new item\n");
				free(newItem->data);
				free(newItem);

				printf("Returning to main menu..\n");
				return;
			}
			else
				break;
		}
		if(price[strlen(price) - 3] != '.')
		{
			printf("\nPrice entered is invalid, must be in form DOLLAR.CENTS: ");
			continue;
		}

		/** Validate cents **/
		tmp[0] = price[strlen(price) - 2];
		tmp[1] = price[strlen(price) - 1];
		tmp[2] = '\0';

		if(atoi(tmp) == 0 && tmp[0] != '0' && tmp[1] != '0')
		{
			printf("\nPrice entered is invalid, must be in form DOLLAR.CENTS: ");
			continue;
		}

		/** Validate dollars (remove cents then try atoi) **/
		price[strlen(price) - 3] = '\0';

		for(i = 0; i < strlen(price); ++i)
		{
			if(price[i] != '0')
				dolExist = TRUE;
		}

		if(atoi(price) == 0 && dolExist)
		{
			printf("\nPrice entered is invalid, must be in form DOLLAR.CENTS: ");
			continue;
		}

		/** Set price **/
		newItem->data->price.dollars = atoi(price);
		newItem->data->price.cents = atoi(tmp);

		priceValid = TRUE;
	}

	/** Add default amount **/
	newItem->data->onHand = DEFAULT_STOCK_LEVEL;

	/** Print details **/
	printf("This item \"%s - %s\" has now been added to the menu.\n", \
			newItem->data->name, \
			newItem->data->desc);

	/** Add stock to system **/
	pushNode(system->itemList, newItem);
	/** Re-sort the stock **/
	sortStock(system->itemList);
}

/**
 * Remove an item from the system, including free'ing its memory.
 * This function implements requirement 8 of the assignment specification.
 **/
void removeItem(VmSystem * system)
{
	int iVal;
	char itemId[ID_LEN + NULL_SPACE];
	Node * remove;
	Boolean success = FALSE;

	while(!success)
	{
		printf("Enter the item id of the item to remove from the menu: ");
		while(TRUE)
		{
			iVal = getInput(itemId, ID_LEN);
			if(iVal == 0)
				printf("Id length invalid, try again: ");
			else if (iVal == -1)
			{
				printf("Cancelling item removal\n");
				remove = NULL;
				printf("Returning to main menu..\n");
				return;
			}
			else 
				break;
		}
		
		remove = getNodeByID(system->itemList, itemId);

		if(!remove)
		{
			printf("\nId entered does not exist.\n");
			continue;
		}

		printf("\"%s - %s %s\" has been removed from the system.\n", \
			remove->data->id, \
			remove->data->name, \
			remove->data->desc);

		success = TRUE;
	}
	
	freeNode(system->itemList, remove);
	printf("Returning to main menu...\n");
}

/**
 * This option will require you to display the coins from lowest to highest
 * value and the counts of coins should be correctly aligned.
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
void displayCoins(VmSystem * system)
{
	int i, titleLength = 12;
	char denomTitle[] = "Denomination";
	char countTitle[] = "Count";

	printf("Coins Summary\n");
	printf("---------\n");
	printf("%-*s | %s\n", titleLength, denomTitle, \
						countTitle);

	for(i = NUM_DENOMS; i >= 0; --i)
	{
		switch(system->cashRegister[i].denom)
		{
			case 0: printf("%-*s | %d", titleLength, "5 cents",\
										system->cashRegister[i].count);
			break;
			case 1: printf("%-*s | %d", titleLength, "10 cents",\
										system->cashRegister[i].count);
			break;
			case 2: printf("%-*s | %d", titleLength, "20 cents",\
										system->cashRegister[i].count);
			break;
			case 3: printf("%-*s | %d", titleLength, "50 cents",\
										system->cashRegister[i].count);
			break;
			case 4: printf("%-*s | %d", titleLength, "1 dollar",\
										system->cashRegister[i].count);
			break;
			case 5: printf("%-*s | %d", titleLength, "2 dollars",\
										system->cashRegister[i].count);
			break;
			case 6: printf("%-*s | %d", titleLength, "5 dollars",\
										system->cashRegister[i].count);
			break;
			case 7: printf("%-*s | %d", titleLength, "10 dollars",\
										system->cashRegister[i].count);
			break;
		}
		printf("\n");
	}

}

/**
 * This option will require you to iterate over every stock in the
 * list and set its onHand count to the default value specified in
 * the startup code.
 * This function implements requirement 9 of the assignment specification.
 **/
void resetStock(VmSystem * system)
{
	int i;
	Node * tmp;

	for(i = 0; i < system->itemList->size; ++i)
	{
		tmp = getNodeByPos(system->itemList, i);
		tmp->data->onHand = DEFAULT_STOCK_LEVEL;
	}

	printf("All stock has been reset to their default levels of '%d'\n", \
		DEFAULT_STOCK_LEVEL);
}

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code.
 * This requirement implements part 3 of requirement 18 in the
 * assignment specifications.
 **/
void resetCoins(VmSystem * system)
{
	int i;
	for (i = 0; i < NUM_DENOMS; ++i)
		changeCoinCount(&system->cashRegister[i], DEFAULT_COIN_COUNT);

	printf("All coins have been reset to their default levels of '%d'\n", \
		DEFAULT_COIN_COUNT);
}

/**
 * This option will require you to display goodbye and free the system.
 * This function implements requirement 10 of the assignment specification.
 **/
void abortProgram(VmSystem * system)
{
	systemFree(system);
	printf("Goodbye.\n");
	exit(EXIT_FAILURE);
}