/*
 * Created by Richard Harman on 17.11.2022
 */

#include "list.h"

allTokens *prog_list;

void initTokenList(){
    prog_list = malloc(sizeof(allTokens));
    if(prog_list == NULL){
        callError(ERR_INTERNAL);
    }
    prog_list->list = NULL;
    prog_list->index = 0;
    prog_list->len = 0;
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

}

void printTokenList(){
    for(int i=0; i < prog_list->len; i++){
        printf("%s\n", prog_list->list[i].info);
    }
}

void createTokenList(){
    initTokenList();
    string str;
    int i = getchar();

    if(i != '<'){
        callError(ERR_SYN);
    }
    ungetc(i, stdin);

    while(1){
        stringInit(&str);
        Token t = getToken(str);
        if(t.type == ERROR_T){
            callError(ERR_LEX);
        }
        addTokenToList(t);
        if(t.type == EOF_T){
            break;
        }
        tokenFree();
    }
    stringFree(&str);
}

void changeTokenListIndex(int index){
    prog_list->index = index;
}

int getListIndex(){
    return prog_list->index;
}

Token getTokenFromList(){
    if( prog_list->index < prog_list->len-1){
        return prog_list->list[prog_list->index++];
    } else {
        return prog_list->list[prog_list->index];
    }
}

void previousTokenListIndex(){
    if(prog_list->index > 0){
        prog_list->index--;
    }
}

Token getEmptyToken(){
    return (Token){.info="",.type=ERROR_T,.kwt=UNKNOWN_K, .isKeyword=false};
}