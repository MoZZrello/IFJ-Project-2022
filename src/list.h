//
// Created by Richard Harman on 17. 11. 2022.
//

#ifndef IFJ_PROJECT_2022_LIST_H
#define IFJ_PROJECT_2022_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "errors.h"

typedef struct token_list {
    Token* program_tokens_list;
    int index;
    int length;
} t_list;

#endif //IFJ_PROJECT_2022_LIST_H
