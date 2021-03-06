/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#include "utility.h"

/**
 * Function required to be used when clearing the buffer. It simply reads
 * each char from the buffer until the buffer is empty again. Please refer
 * to the materials on string and buffer handling in the course for more
 * information.
 **/
void readRestOfLine()
{
    int ch;
    while(ch = getc(stdin), ch != EOF && ch != '\n')
    { } /* Gobble each character. */

    /* Reset the error status of the stream. */
    clearerr(stdin);
}

/** 
 * Gets input from the user of size given
 * consumes extra characters 
 * Returns 0 on failure, 1 on success, -1 when cancelling input
 **/
int getInput(char * output, int size)
{
	char *buffer;
	if(!(buffer = malloc(sizeof(char) * (size + EXTRA_SPACES))))
	{
		fprintf(stderr, "Fatal error: malloc() failed in getInput()");
		exit(EXIT_FAILURE);
	}

	/** Set colour to boldened white **/
	printf(BOLDWHITE);

	/** User input **/
	if(!fgets(buffer, size + EXTRA_SPACES, stdin))
	{
		if(buffer)
			free(buffer);
		readRestOfLine();
    	printf(RESET);
		return 0;
	}

	/** Reset colour **/
	printf(RESET);

	/** Check length **/
	if(buffer[strlen(buffer) - 1] != '\n')
	{
    	readRestOfLine();
		if(buffer)
			free(buffer);
		return 0;
	}

	if(buffer[0] == '\n')
	{
		if(buffer)
			free(buffer);
		return -1;
	}

	strcpy(output, buffer);

	/** Replace \n with \0 **/
	output[strlen(output) - 1] = '\0';

	free(buffer);

	return 1;
}

/** 
 * Function used to parse a string to a long.
 * Takes the string to parse, and the location to put it.
 * Returns false on error, or true on success.
 **/
Boolean parseLong(const char *str, long *val)
{
	char *ptr;
	Boolean outcome = TRUE;
	errno = 0;
	*val = strtol(str, &ptr, 10);

	if(*ptr !=  '\0' || ptr == str || \
		((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
		outcome = FALSE;

	return outcome;
}

/** 
 * Parse a string to an unsigned int.
 * Takes the string to parse, and the location to put it.
 * Returns false on error, or true on success.
 **/
Boolean parseUInt(const char *str, unsigned int *val)
{
	Boolean outcome = TRUE;
	errno = 0;
	*val = atoi(str);

	if(*val == UINT_MAX && errno == ERANGE)
		outcome = FALSE;

	return outcome;
}

/** 
 * Gets all lines from a file and returns the array, or NULL if failed 
 * Sentinel is \0 
 **/
char** GetFileContents(const char * fname)
{
	/** Init **/
	char ch;
	char line[256];
	int linesInFile = 0;
	/** Current line **/
	int curLine = 0;
	/** Array of lines **/
	char **fileContents;

	/** Open file for reading **/
	FILE *fp;
	fp = fopen(fname, "r");

	/** Handle error **/
	if(fp == NULL)
		return NULL;

	linesInFile = 0;

	/** Get array size **/
	while(!feof(fp))
	{
		ch = fgetc(fp);
		if(ch == '\n')
			linesInFile++;
	}

	/** Allocate sentinel **/
	++linesInFile;

	/** Define array **/
	fileContents = malloc(sizeof(char*) * linesInFile);

	/** Go back to start of file **/
	rewind(fp);

	/** Parse the contents of the file **/
	while(fgets(line, sizeof(line), fp))
	{
		fileContents[curLine] = malloc(sizeof(line));
		/** Allocate each string **/
		strcpy(fileContents[curLine], line);
		++curLine;
	}

	if(fclose(fp))
		printf("Error closing file\n");

	/** Add sentinel **/
	fileContents[curLine] = '\0';

	return fileContents;
}

/** 
 * Frees an array of strings from memory 
 **/
void freeStringArray(char ** arr)
{
	int i;
	int size = 0;

	if(!arr || !arr[0])
		return;
	
	/** Count **/
	while(arr[size] != '\0') { ++size; }

	for(i = 0; i < size + 1; ++i)
	{
		free(arr[i]);
		arr[i] = NULL;
	}
	free(arr);
	arr = NULL;
}

/**
 * Counts the amount of digits in an integer
 **/
int getIntDigits (unsigned n) {
    if (n < 0) 
    	return getIntDigits ((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10) 
    	return 1;
    return 1 + getIntDigits (n / 10);
}