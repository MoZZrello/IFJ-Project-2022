/*
 * Created by Richard Harman on 02.10.2022
 */

#ifndef IFJ_PROJECT_2022_SCANNER_H
#define IFJ_PROJECT_2022_SCANNER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "strings.h"
#include "errors.h"

/**
 *@brief Enum types of automat
 */
typedef enum {
    Start,
    S_EOF,
    S_Dot,
    S_DoubleDot,
    S_Left_Bracket,
    S_Right_Bracket,
    S_Left_Curly_Bracket,
    S_Right_Curly_Bracket,
    S_Semicolon,
    S_Plus,
    S_Comma,
    S_Multiply,
    S_Minus,
    S_Question_Mark,
    S_Number,
    S_Exponent_0,
    S_Exponent_1,
    S_Exponent_Sign,
    S_Decimal_0,
    S_Decimal_1,
    S_Divide,
    S_Comment_SL,
    S_Comment_ML,
    S_Comment_ML_End,
    S_Dollar,
    S_Var,
    S_Exclamation,
    S_Not_Equal_0,
    S_Not_Equal_1,
    S_Assign,
    S_Equal_0,
    S_Equal_1,
    S_Identifier,
    S_Identifier_Q,
    S_Greater,
    S_Greater_Equal,
    S_Less,
    S_Less_Equal,
    S_String_0,
    S_String_1,
    S_Escape_0,
    S_Escape_1,
    S_Octal_0,
    S_Octal_1,
    S_Octal_2,
    S_Hex_0,
    S_Hex_1,
    S_Hex_2,
    S_PHP_0,
    S_PHP_1,
    S_PHP_2,
    S_PHP_3,
    S_PHP_Comment,
    S_PHP_4,
    S_PHP_END,
    ERROR
} AutomatStates;

/**
 *@brief Structre to save tokens and their information
 */
typedef struct {
    enum {
        VAR_ID,
        IDENTIFIER,
        STRING,
        EOF_T,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        LEFT_CURLY_BRACKET,
        RIGHT_CURLY_BRACKET,
        SEMICOLON,
        PLUS,
        MINUS,
        COMMA,
        MULTIPLY,
        DOT,
        DOUBLE_DOT,
        QUESTION_MARK,
        DIVIDE,
        NUMBER,
        DECIMAL_NUMBER,
        EXPONENT_NUMBER,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        ASSIGN,
        EQUAL,
        NOT_EQUAL,
        COMMENT,
        PHP,
        PHP_END,
        ERROR_T,
        EOML,
    } type;
    char *info;
    bool isKeyword;
    KeywordType kwt;
    bool canBeNull;
} Token;

/**
 *@brief A function that specifies next step of automat
 *@param AutomatStates input -> last step of automat
 *@param char c -> new character from STDIN
 *@param bool *php_comment ->
 */
AutomatStates nextState(AutomatStates input, char c, bool *php_comment);

/**
 *@brief A function to create a whole token from data
 *@param AutomatStates final_state -> last step of automat
 *@param string *str -> string for easier work with characters
 */
Token returnTokenCreator(AutomatStates final_state, string* str);

/**
 *@brief A function to pass created token to token list
 *@param string str -> string for easier work with characters
 */
Token getToken(string str);

/**
 *@brief A function to get name of token type as a string
 *@param Token t -> token
 */
char* getTypeName(Token t);

/**
 *@brief A function to get name of token keyword type as a string
 *@param Token t -> token
 */
char* getKeywordTypeName(Token t);

/**
 *@brief A function to free all tokens
 */
void tokenFree();

#endif //IFJ_PROJECT_2022_SCANNER_H
