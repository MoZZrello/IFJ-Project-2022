/*
 * Created by Richard Harman on 17.11.2022
*/

#ifndef IFJ_PROJECT_2022_LIST_H
#define IFJ_PROJECT_2022_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "symtable.h"

/**
 *@brief Structre for token list
 */
typedef struct allTokens{
    Token* list;
    int index;
    int len;
} allTokens;

/**
 *@brief Function to create a token list
 */
void createTokenList();

/**
 *@brief Function to initialize a token list
 */
void initTokenList();

/**
 *@brief Function to add a token to a token list
 *@param Token t -> token to be added
 */
void addTokenToList(Token t);

/**
 *@brief Function to delete last token from a token list
 */
void deleteLastTokenFromList();

/**
 *@brief Function to delete all tokens from a token list
 */
void emptyTokenList();

/**
 *@brief Function to print all tokens from a token list
 */
void printTokenList();

/**
 *@brief Function to get current index from a token list
 */
int getListIndex();

/**
 *@brief Function to change token list index
 */
void changeTokenListIndex(int index);

/**
 *@brief Function to get token list index one behind
 */
void previousTokenListIndex();

/**
 *@brief Function to get a current token from list
 */
Token getTokenFromList();

/**
 *@brief Function to return an empty token
 */
Token getEmptyToken();


#endif //IFJ_PROJECT_2022_LIST_H
