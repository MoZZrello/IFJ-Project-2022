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
        if(t.type == EOF_T){
            free(str.info);
            break;
        }
        printf("%s %s \n",t.info ,getTypeName(t));
        tokenFree();
    }
    return 0;
}