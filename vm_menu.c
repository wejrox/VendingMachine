/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "vm_menu.h"

/**
 * vm_menu.c handles the initialisation and management of the menu array.
 **/

/**
 * In this function you need to initialise the array of menu items
 * according to the text to be displayed for the menu. This array is
 * an array of MenuItem with text and a pointer to the function
 * that will be called.
 **/
void initMenu(MenuItem * menu)
{
	/** Init text **/
	strcpy(menu[0].text, "Display Items");
	strcpy(menu[1].text, "Purchase Items");
	strcpy(menu[2].text, "Save and Exit");
	strcpy(menu[3].text, "Add Item");
	strcpy(menu[4].text, "Remove Item");
	strcpy(menu[5].text, "Display Coins");
	strcpy(menu[6].text, "Reset Stock");
	strcpy(menu[7].text, "Reset Coins");
	strcpy(menu[8].text, "Abort Program");

	/** Init func **/
	menu[0].function = displayItems;
	menu[1].function = purchaseItem;
	menu[2].function = saveAndExit;
	menu[3].function = addItem;
	menu[4].function = removeItem;
	menu[5].function = displayCoins;
	menu[6].function = resetStock;
	menu[7].function = resetCoins;
	menu[8].function = abortProgram;
}

/**
 * Gets input from the user and returns a pointer to the MenuFunction
 * that defines how to perform the user's selection. NULL is returned
 * if an invalid option is entered.
 **/
MenuFunction getMenuChoice(MenuItem * menu)
{
	char buffer[1 + NULL_SPACE];
	long op;

	printf(BOLDWHITE);
	fgets(buffer, sizeof(buffer), stdin);
	printf(RESET);
	readRestOfLine();
	
	/** Parse check **/
	if(!parseLong(buffer, &op))
		return NULL;
	/** Range check **/
	if(op < 1 || op > NUM_MENU_OPS)
		return NULL;

	return menu[op - 1].function;
	if(menu->function)
		return *menu->function;


    return NULL;
}

void printMenu(MenuItem * menu)
{
	int i;

	printf("Main Menu:\n");
	for(i = 0; i < 3; ++i)
		printf("%d.%s\n", i+1, menu[i].text);

	printf("Administrator-Only Menu:\n");
	for(i = i; i < NUM_MENU_OPS; ++i)
		printf("%d.%s\n", i+1, menu[i].text);

	printf("Select your option (1-%d)", NUM_MENU_OPS);
}