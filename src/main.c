//
// Created by Richard Harman on 4. 10. 2022.
//

#include "parser.h"

int main(){
    printf("Starting program...\n");
    createTokenList();
    //printTokenList();

    prolog();

    if(end_file) {
        printf("som tu\n");
    }
    return PROG_OK;
}