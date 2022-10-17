//
// Created by Richard Harman on 2. 10. 2022.
//

#ifndef IFJ_PROJECT_2022_SCANNER_H
#define IFJ_PROJECT_2022_SCANNER_H

#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "scanner.h"
#include "strings.h"
#include "errors.h"
#include "stdbool.h"

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
    S_PHP_4,
    S_PHP_END,
    ERROR
} AutomatStates;

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
    } type;
    char *info;
    bool isKeyword;
    KeywordType kwt;
} Token;



#endif //IFJ_PROJECT_2022_SCANNER_H
