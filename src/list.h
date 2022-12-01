//
// Created by Richard Harman on 17. 11. 2022.
//

#ifndef IFJ_PROJECT_2022_LIST_H
#define IFJ_PROJECT_2022_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "symtable.h"

typedef struct allTokens{
    Token* list;
    int index;
    int len;
} allTokens;

void createTokenList();
void initTokenList();
void addTokenToList(Token t);
void deleteLastTokenFromList();
void emptyTokenList();
void printTokenList();
int getListIndex();
void changeTokenListIndex(int index);
void previousTokenListIndex();
Token getTokenFromList();
Token getEmptyToken();


#endif //IFJ_PROJECT_2022_LIST_H
