/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2017 Assignment #2
 * Full Name        : JAMES MCDOWELL
 * Student Number   : S3540510
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Xiaodong Li
 **********************************************************************/
#ifndef VM_STOCK_H
#define VM_STOCK_H

#include "vm_coin.h"

/**
 * The default stock level that all new stock should start with and the value
 * to use when restting stock.
 **/
#define DEFAULT_STOCK_LEVEL 20

#define STOCK_DELIM "|"
/** For differentiating **/
#define STOCK_DELIM_CENT "."
#define STOCK_DELIM_COMB STOCK_DELIM STOCK_DELIM_CENT

/** Custom functions **/
void initStockList(VmSystem * system);
void freeList(List * list);
Node* createNode(char **tokenisedData);
void pushNode(List * list, Node * newNode);
Node* getNodeByPos(List * list, int pos);
Node* getNodeByID(List * list, char * id);
void freeNode(List * list, Node * target);
int getListLength(List * list);
void sortStock(List * list);
int cmpNode(Node * first, Node * second);
void printStock(List * stock);
int getMaxWidth(List * stock, int param);
void getNewID(List * stock, char * newId);

#endif
