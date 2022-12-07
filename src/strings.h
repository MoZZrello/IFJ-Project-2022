/*
 * Created by Richard Harman on 17.10.2022
 */

#ifndef IFJ_PROJECT_2022_STRINGS_H
#define IFJ_PROJECT_2022_STRINGS_H

/**
 *@brief Enum types of keywords
 */
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

/**
 *@brief Structre to create strings
 */
typedef struct string_struct
{
    char* data;
    int length;
    int allocSize;
} string;

/**
 *@brief A function to initialize a string
 *@param string *s -> string to be initialized
 */
int stringInit(string *s);

/**
 *@brief A function to add a character to a string
 *@param string *s -> string
 *@param char c -> character to be added
 */
int addChar(string *s, char c);

/**
 *@brief A function to set an empty string
 *@param string *s -> string
 */
void stringClear(string *s);

/**
 *@brief A function to free a string
 *@param string *s -> string
 */
void stringFree(string *s);

/**
 *@brief A function to pick a keyword type of token
 *@param string *s -> string
 */
KeywordType keywordCheck(string *s);

/**
 *@brief A function to see if a token type can be null
 *@param string *s -> string
 */
int checkNull(string *s);

#endif //IFJ_PROJECT_2022_STRINGS_H
