/*
 * Created by Richard Harman on 04.10.2022
 */

#include "parser.h"
#include "generator.h"

int main(){
    ht_table_t *table;
    table = malloc(sizeof(ht_table_t));
    if(table == NULL){
        callError(ERR_INTERNAL);
    }

    ht_init(table);

    createTokenList();

    prolog();
    antilog(table);

    int i = 0;
    while(!0){
        char func[MAX_HT_SIZE];
        sprintf(func, "%d", i++);
        element* e = ht_get(table, func);
        if(e == NULL){
            break;
        }
        //printf("%s\n", e->name.info);
        if(e->argslist != NULL){
            free(e->argslist->list);
            free(e->argslist);
        }
    }

    ht_delete_all(table);

    free(table);
    return PROG_OK;
}