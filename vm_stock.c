/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "vm_stock.h"

/**
 * vm_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

/**
 * Some example functions:
 * create list, free list, create node, free node, insert node, etc...
 */

/**
 * Create a new list and return a pointer
 **/
void initStockList(VmSystem * system)
{
	system->itemList->size = 0;
	system->itemList->head = NULL;
}

void freeList(List * list)
{
	/** Counters **/
	int i;
	Node * tmp;

	/** Free each node in the list **/
	if(list->head && list)
	{
		for(i = 0; i < list->size; ++i)
		{
			tmp = list->head;
			if(list->head->next)
				list->head = list->head->next;

			if(tmp->data)
				free(tmp->data);
			if(tmp)
				free(tmp);
		}
	}

	/** Free the list itself **/
	free(list);
}

/** 
 * Create a new node
 * Data should have been checked beforehand
 **/
Node* createNode(char **tokenisedData)
{
	unsigned int dollars;
	unsigned int cents;
	unsigned int onHand;
	Node * newNode = malloc(sizeof(Node));
	newNode->data = malloc(sizeof(Stock));

	strcpy(newNode->data->id, tokenisedData[0]);
	strcpy(newNode->data->name, tokenisedData[1]);
	strcpy(newNode->data->desc, tokenisedData[2]);
	parseUInt(tokenisedData[3], &dollars);
	parseUInt(tokenisedData[4], &cents);
	parseUInt(tokenisedData[5], &onHand);
	newNode->data->price.dollars = dollars;
	newNode->data->price.cents = cents;
	newNode->data->onHand = onHand;

	return newNode;
}

/** 
 * This function adds a stock entry to the system, at the front of the list
 **/
void pushNode(List * list, Node * newNode)
{
	newNode->next = list->head;
	list->head = newNode;
	++list->size;
}

/** 
 * Gets the node in the position specified
 **/
Node* getNodeByPos(List * list, int pos)
{
	/** Counter **/
	int i;
	/** Holder until found **/
	Node * tmp;

	/** Pre-check **/
	if(pos > list->size)
		return NULL;

	/** Set tmp **/
	tmp = list->head;

	/** First node should be free'd **/
	if(pos == 0)
		return tmp;
	/** Some other node should be free'd **/
	else
	{
		/** Get the node at pos **/
		for(i = 0; i < pos; ++i)
		{
			tmp = tmp->next;
		}
	}

	return tmp;
}

/** 
 * Gets node with the ID specified 
 **/
Node* getNodeByID(List * list, char * id)
{
	/** Counter **/
	int i;
	/** Holder until found **/
	Node * tmp;

	/** Pre-check **/
	if(id == NULL)
		return NULL;

	/** Set tmp **/
	tmp = list->head;

	/** Get the node with ID **/
	for(i = 0; i < list->size; ++i)
	{
		if(!tmp)
			break;
		if(strcmp(tmp->data->id, id) == 0)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}

/**
 * Frees the node specified
 * updates list
 **/
void freeNode(List * list, Node * target)
{
	/** Count **/
	int i;
	Node * current, * prev;
	Boolean found = FALSE;

	current = list->head;

	printf("Finding node\n");
	/** Iterate through the array and find the node **/
	for(i = 0; i < list->size; ++i)
	{
		/** 
		 * Remove node if it's the one required, 
		 * Set the previous node to point to the next node
		 **/
		printf("Comparing node %d\n", i);
		if(strcmp(current->data->id, target->data->id) == 0)
		{
			/** 
			 * Changes the previous node to point to the next node instead 
			 * If prev is null, it must be the first node
			 **/
			if(prev)
				prev->next = current->next;
			else
				list->head = list->head->next;

			free(current->data);
			free(current);
			found = TRUE;
		}
		/** 
		 * Set the next node to check and save this node in case 
		 * the next node is the required one
		 **/
		else
		{
			prev = current;
			current = current->next;
		}

		if(found) break;
	}
	
	if(found)
		list->size -= 1;
}

/**
 * Calculates the current length of the list
 * Returns the length and sets the list length variable
 **/
int getListLength(List * list)
{
	/** Temp Node **/
	Node * current = list->head;
	/** Reset list size **/
	list->size = 0;

	/** Count elements in list **/
	while(current != NULL)
	{
		list->size = list->size + 1;
		current = current->next;
	}

	return list->size;
}

/**
 * Sorts the stock based on name
 **/
void sortStock(List * list)
{
	/** Counter **/
	int i;
	/** Where will this node go in the array? **/
	int posInList;

	Node** sortedArray;
	Node* target;

	sortedArray = malloc(sizeof(int *) * list->size);
	target = list->head;

	/** Compare until nothing left **/
	while (target)
	{
		posInList = 0;
		for(i = 0; i < list->size; ++i)
			if(cmpNode(target, getNodeByPos(list, i)) > 0)
				++posInList;

		/** Move the target into its position on the list **/
		sortedArray[posInList] = target;
		target = target->next;
	}
	
	/** Update pointers in array **/
	for(i = 0; i < list->size - 1; ++i)
	{
		sortedArray[i]->next = NULL;

		if(sortedArray[i+1])
			sortedArray[i]->next = sortedArray[i+1];
	}

	/** Update list **/
	list->head = sortedArray[0];

	/** Free memory **/
	free(sortedArray);
}

/** 
 * Compares one node to the next based on their name
 * Returns the node who has letters before the other (alphabetical)
 **/
int cmpNode(Node * first, Node * second)
{
	int i, str1Length, str2Length, minLength;
	str1Length = strlen(first->data->name);
	str2Length = strlen(second->data->name);

	/** Determine length to use **/
	if(str1Length > str2Length)
		minLength = str2Length;
	else
		minLength = str1Length;

	if(first == second)
		return 0;

	/** Compare each character **/
	for(i = 0; i < minLength; ++i)
	{
		if(first->data->name[i] > second->data->name[i])
			return 1;
		else if(first->data->name[i] < second->data->name[i])
			return -1;
	}

	if(str1Length > str2Length)
		return 1;
	else
		return -1;
}

/** 
 * Prints the stock list (debugging)
 **/
void printStock(List * stock)
{
	Node *n;
	int i;
	
	for(i = 0; i < stock->size; ++i)
	{
		n = getNodeByPos(stock, i);
		printf("%s\n", n->data->name);
	}
}

/**
 * Returns the size of the biggest string in the list
 * Columns: 0=id, 1=name, 2=price, 3=onhand
 **/
int getMaxWidth(List * stock, int param)
{
	int longest, i;
	Node * current;
	longest = 0;
	current = stock->head;

	switch(param)
	{
		case 0:
			for(i = 0; i < stock->size; ++i)
			{
				if(strlen(current->data->id) > longest)
					longest = strlen(current->data->id);

				current = current->next;
			}
		break;
		case 1:
			for(i = 0; i < stock->size; ++i)
			{
				if(strlen(current->data->name) > longest)
					longest = strlen(current->data->name);

				current = current->next;
			}
		break;
		case 2:
			for(i = 0; i < stock->size; ++i)
			{
				if((getIntDigits(current->data->price.dollars) +  \
					getIntDigits(current->data->price.cents)) > longest)
				{
					longest = (getIntDigits(current->data->price.dollars) +  \
					getIntDigits(current->data->price.cents));
				}
				current = current->next;
			}
		break;
		case 3:
			for(i = 0; i < stock->size; ++i)
			{
				if(getIntDigits(current->data->onHand) > longest)
					longest = getIntDigits(current->data->onHand);
				
				current = current->next;
			}
		break;
	}

	return longest;
}

/**
 * Generates a new ID for a new item and returns it
 **/
void getNewID(List * stock, char * newId)
{
	int i, tmpId, idLen;
	char * largest;
	char tmp[10];
	Node * curNode;

	curNode = stock->head;
	if(!curNode)
		return;

	for(i = 0; i < stock->size; ++i)
	{
		if(!curNode->next)
			break;

		if(strcmp(curNode->data->id, curNode->next->data->id) < 0)
			largest = curNode->next->data->id;
		curNode = curNode->next;
	}

	/** Change the string to an int to increment **/
	largest[0] = '0';
	tmpId = atoi(largest);
	++tmpId;

	idLen = getIntDigits(tmpId);

	newId[0] = 'I';
	for(i = 1; i < (ID_LEN - idLen); ++i)
	{
		newId[i] = '0';
	}

	newId[i] = '\0';

	/** Paste in the new ID **/
	sprintf(tmp, "%d", tmpId);
	strcat(newId, tmp);
}