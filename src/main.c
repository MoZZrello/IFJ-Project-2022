//
// Created by Richard Harman on 4. 10. 2022.
//

#include "scanner.h"
#include "parser.h"

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
        

        //printf("%s %s %s %s\n",t.info ,getTypeName(t) , getKeywordTypeName(t), str.data);
        
        prolog(&t);
        
        if(end_file) {
            printf("som tu\n");
            free(str.data);
            break;
        }

        tokenFree();
    }
    return PROG_OK;
}