//
// Created by Richard Harman on 4. 10. 2022.
//

#include "parser.h"

int main(){
    printf("Starting program...\n");

    ht_table_t *table;
    table = malloc(sizeof(ht_table_t));
    ht_init(table);

    createTokenList();

    //prolog();
    antilog(table);
    //neulog();

    if(end_file) {
        printf("som tu\n");
    }

    free(table);
    return PROG_OK;
}