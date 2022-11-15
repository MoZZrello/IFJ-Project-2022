//
// Created by Richard Harman on 17. 10. 2022.
//

#ifndef IFJ_PROJECT_2022_STRINGS_H
#define IFJ_PROJECT_2022_STRINGS_H

typedef enum {
    ELSE_K,
    FLOAT_K,
    FUNCTION_K,
    IF_K,
    INT_K,
    NULL_K,
    RETURN_K,
    STRING_K,
    VOID_K,
    WHILE_K,
    UNKNOWN_K,
} KeywordType;

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
