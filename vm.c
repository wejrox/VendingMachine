/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "vm.h"

int main(int argc, char ** argv)
{
	/** Menu **/	
	MenuItem menuItems[NUM_MENU_OPS];
	void (*menuFunction)(VmSystem *);
	/** System **/
	VmSystem * system;
	system = malloc(sizeof(VmSystem));
	system->itemList = malloc(sizeof(List));
	
	systemInit(system);
	initMenu(menuItems);

	/** Handle cmd arguments **/
	switch(argc)
	{
		/** Exit when no arguments presented **/
		case 1:
			printf("No files given for data.\n");
			abortProgram(system);
		break;
		/** Validate both stock and coin list **/
		case 3:
			if(!loadData(system, argv[1], argv[2]))
			{
				printf("Error parsing data files.\n");
				abortProgram(system);
			}
		break;
		/** Abort if too many options have been given **/
		default:
			printf("Too many command line arguments specified.\n");
			abortProgram(system);
		break;
	}

	/** Menu **/
	while(TRUE)
	{
		printMenu(menuItems);
		menuFunction = getMenuChoice(menuItems);
		if(!menuFunction)
		{
			printf("Option entered is invalid\n\n");
			continue;
		}

		menuFunction(system);
		/** Create space for the menu **/
		printf("\n\n");
	}

    printf("Goodbye. \n\n");
    systemFree(system);
    return EXIT_SUCCESS;
}