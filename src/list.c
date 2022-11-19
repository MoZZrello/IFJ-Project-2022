//
// Created by Richard Harman on 17. 11. 2022.
//

#include "list.h"

allTokens *prog_list;

void List_Init( List *list) {
    list->firstElement = NULL;
    list->activeElement = NULL;
}

void initTokenList(){
    prog_list = malloc(sizeof(allTokens));
}

void addTokenToList(Token t){
    prog_list->list = realloc(prog_list->list, sizeof(Token)*(prog_list->len+1));
    prog_list->list[prog_list->len] = t;
    prog_list->len++;
}

void deleteLastTokenFromList(){
    prog_list->list[prog_list->len] = (Token){.type=EOML};
    prog_list->len--;
}

void emptyTokenList(){
    for(int i=0; i < prog_list->len; i++){
        printf("%s\n", getTypeName(prog_list->list[i]));
    }
}
