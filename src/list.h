//
// Created by Richard Harman on 17. 11. 2022.
//

#ifndef IFJ_PROJECT_2022_LIST_H
#define IFJ_PROJECT_2022_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "errors.h"


typedef struct token_element {
    Token* program_tokens_list;
    int index;
    int length;
    struct token_element *nextElement;
} *t_element;

typedef struct {
    t_element firstElement;
    t_element activeElement;
}List;

typedef struct allTokens{
    Token* list;
    int index;
    int len;
} allTokens;

void Init_List( List *);
void Insert(List *, Token ); //Token tam je fitkivne len
void Delete(List *);

void createTokenList();
void initTokenList();
void addTokenToList(Token t);
void deleteLastTokenFromList();
void emptyTokenList();
void printTokenList();
void changeTokenListIndex(int index);
Token getTokenFromList();


#endif //IFJ_PROJECT_2022_LIST_H
