//
// Created by Richard Harman on 16. 11. 2022.
//
#include "errors.h"

void callError(int err_num){
    switch(err_num){
        case 1:
            fprintf(stderr, "Lexical error found.\n");
            break;
        case 2:
            fprintf(stderr, "Syntax error found.\n");
            break;
        case 3:
            fprintf(stderr, "Semantics error found. Undefined function or function redefinition.\n");
            break;
        case 4:
            fprintf(stderr, "Semantics error found. Wrong number of arguments or incompatible type.\n");
            break;
        case 5:
            fprintf(stderr, "Semantics error found. Undefined variable.\n");
            break;
        case 6:
            fprintf(stderr, "Semantics error found. Function return incompatible type or missing variable.\n");
            break;
        case 7:
            fprintf(stderr, "Semantics error found. Incompatible types in expression.\n");
            break;
        case 8:
            fprintf(stderr, "Other semantics error found.\n");
            break;
        case 99:
            fprintf(stderr, "Internal error.\n");
            break;
        default:
            fprintf(stderr, "Unknown error.\n");
            break;
    }
    //printf("ERROR %d\n", err_num);
    exit(err_num);
}