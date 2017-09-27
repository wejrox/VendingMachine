/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
/** For defined constants **/
#include <limits.h>
/** For Parsing **/
#include <errno.h>

typedef enum boolean
{
    FALSE = 0,
    TRUE
} Boolean;

#define BOLDWHITE   "\033[1m\033[37m"
#define RESET   "\033[0m"

#define NEW_LINE_SPACE 1
#define NULL_SPACE 1

/**
 * This is used to compensate for the extra character spaces taken up by
 * the '\n' and '\0' when input is read through fgets().
 **/
#define EXTRA_SPACES (NEW_LINE_SPACE + NULL_SPACE)

#define EMPTY_STRING ""

/**
 * Call this function whenever you detect buffer overflow.
 **/
void readRestOfLine();
int getInput(char * output, int size);
Boolean parseLong(const char * str, long * val);
Boolean parseUInt(const char * str, unsigned int * val);
char** GetFileContents(const char * fname);
void freeStringArray(char ** arr);
int getIntDigits (unsigned n);

#endif
