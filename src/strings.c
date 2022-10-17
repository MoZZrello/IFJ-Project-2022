//
// Created by Richard Harman on 17. 10. 2022.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"



int stringInit(string *s){
    s->info = (char*) malloc(8*sizeof(char));
    if(s->info == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        return 1;
    }
    s->info[0] = '\0';
    s->length = 0;
    s->allocSize = 8;
    return 0;
}

int addChar(string *s, char c){
    if (s->length +1 >= s->allocSize) {
        char *tmp = (char *) realloc(s->info, (s->length + 8)*sizeof(char));
        if (tmp == NULL) {
            fprintf(stderr, "Memory didn't allocate!\n");
            return 1;
        } else {
            s->info = tmp;
        }
        s->allocSize = s->length + 8;
    }

    s->info[s->length] = c;
    s->length++;
    s->info[s->length] = '\0';
    return 0;
}

void stringClear(string *s){
    s->info[0] = '\0';
    s->length = 0;
}

