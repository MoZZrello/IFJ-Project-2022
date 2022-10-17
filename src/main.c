//
// Created by Richard Harman on 4. 10. 2022.
//

#include "scanner.c"
#include "scanner.h"

int main(){
    printf("Starting program...\n");
    while(1){
        string str;
        stringInit(&str);
        Token t = getToken(str);

        if(t.type == ERROR_T){
            fprintf(stderr, "Lexical analysis error! Wrong lexeme structure.\n");
            return ERR_LEX;
        }
        if(t.type == EOF_T){
            free(str.data);
            break;
        }

        printf("%s %s %s\n",t.info ,getTypeName(t) , getKeywordTypeName(t));
        tokenFree();
    }
    return PROG_OK;
}