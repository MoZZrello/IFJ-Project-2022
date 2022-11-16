//
// Created by Richard Harman on 16. 11. 2022.
//
#include "errors.h"

void callError(int err_num){
    switch(err_num){
        case 1:
            fprintf(stderr, "Lexical error found.\n");
        case 2:
            fprintf(stderr, "Syntax error found.\n");
        case 3:
            fprintf(stderr, "Semantics error found. Undefined function or function redefinition.\n");
        case 4:
            fprintf(stderr, "Semantics error found. Wrong number of arguments or incompatible type.\n");
        case 5:
            fprintf(stderr, "Semantics error found. Undefined variable.\n");
        case 6:
            fprintf(stderr, "Semantics error found. Function return incompatible type or missing variable.\n");
        case 7:
            fprintf(stderr, "Semantics error found. Incompatible types in expression.\n");
        case 8:
            fprintf(stderr, "Other semantics error found.\n");
        case 99:
            fprintf(stderr, "Internal error.\n");
        default:
            fprintf(stderr, "Unknown error.\n");
    }

    exit(err_num);
}