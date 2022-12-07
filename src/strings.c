/*
 * Created by Richard Harman on 17.10.2022
 */

#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "strings.h"
#include "errors.h"



int stringInit(string *s){
    s->data = (char*) malloc(8 * sizeof(char));
    if(s->data == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        return ERR_INTERNAL;
    }
    s->data[0] = '\0';
    s->length = 0;
    s->allocSize = 8;
    return PROG_OK;
}

void stringFree(string *s){
    free(s->data);
}

int addChar(string *s, char c){
    if (s->length +1 >= s->allocSize) {
        char *tmp = (char *) realloc(s->data, (s->length + 8) * sizeof(char));
        if (tmp == NULL) {
            fprintf(stderr, "Memory didn't allocate!\n");
            return ERR_INTERNAL;
        } else {
            s->data = tmp;
        }
        s->allocSize = s->length + 8;
    }

    s->data[s->length] = c;
    s->length++;
    s->data[s->length] = '\0';
    return PROG_OK;
}

void stringClear(string *s){
    s->data[0] = '\0';
    s->length = 0;
}

KeywordType keywordCheck(string *s){
    if (strcmp(s->data, "else") == 0){
        return ELSE_K;
    } else if (strcmp(s->data, "float") == 0) {
        return FLOAT_K;
    } else if (strcmp(s->data, "?float") == 0) {
        return FLOAT_K;
    } else if (strcmp(s->data, "function") == 0) {
        return FUNCTION_K;
    } else if (strcmp(s->data, "if") == 0) {
        return IF_K;
    } else if (strcmp(s->data, "int") == 0) {
        return INT_K;
    } else if (strcmp(s->data, "?int") == 0) {
        return INT_K;
    } else if (strcmp(s->data, "null") == 0) {
        return NULL_K;
    } else if (strcmp(s->data, "return") == 0) {
        return RETURN_K;
    } else if (strcmp(s->data, "string") == 0) {
        return STRING_K;
    } else if (strcmp(s->data, "?string") == 0){
        return STRING_K;
    } else if (strcmp(s->data, "void") == 0) {
        return VOID_K;
    } else if (strcmp(s->data, "while") == 0) {
        return WHILE_K;
    } else {
        return UNKNOWN_K;
    }
}

int checkNull(string *s){
    if (strstr(s->data, "?") == NULL){
        return 0;
    } else {
        return 1;
    }
}