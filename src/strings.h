//
// Created by Richard Harman on 17. 10. 2022.
//

#ifndef IFJ_PROJECT_2022_STRINGS_H
#define IFJ_PROJECT_2022_STRINGS_H

#include <scanner.h>

typedef struct string_struct
{
    char* data;
    int length;
    int allocSize;
} string;

int stringInit(string *s);
int addChar(string *s, char c);
void stringClear(string *s);
KeywordType keywordCheck(string *s);

#endif //IFJ_PROJECT_2022_STRINGS_H
