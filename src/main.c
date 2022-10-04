//
// Created by Richard Harman on 4. 10. 2022.
//

#include "scanner.h"
#include "scanner.c"

int main(){
    printf("Starting program...\n");
    while(1){
        Token t = getToken();
        if(t.type == EOF_T){
            break;
        }
        printf("%s %s \n", t.info, getTypeName(t));
        free(tmp_token->info);
        free(tmp_token);
    }
    return 0;
}