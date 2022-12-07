/*
 * Created by Marek Spirka and Jasmina Csalova on 17.11.2022
 * Inspired by the second project from the IAL 2022 course
 */

#include "symtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

int get_hash(char *key) {
    unsigned result;

    for ( result = 0; *key!= '\0'; key++)
        result = *key + 31 *  result;
    return  result % MAX_HT_SIZE;
}

void ht_init(ht_table_t *table) {
    for (int i = 0; i < MAX_HT_SIZE; i++){
        (*table)[i] = NULL;
    }
}

ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table == NULL) {
        return NULL;
    }

    int index = get_hash(key);
    ht_item_t *search = (*table)[index];

    while(search != NULL){
        if(search->key == *key){
            return search;
        }else{
            callError(ERR_INTERNAL);
        }
    }
    return NULL;
}

void ht_insert(ht_table_t *table, char *key, element *data) {
    if (table == NULL) {
        return;
    }
    ht_item_t *item_exist = ht_search(table, key);

    if(item_exist!= NULL){
        item_exist->e = *data;
    }
    else{
        int index = get_hash(key);

        ht_item_t *item = malloc(sizeof (ht_item_t));
        if(item == NULL){
            callError(ERR_INTERNAL);
        }
        memcpy(&item->e, data, sizeof(element));
        memcpy(&item->key, key, sizeof(*key));
        (*table)[index] = item;
    }
}

element *ht_get(ht_table_t *table, char *key) {
    ht_item_t *item = ht_search(table, key);

    if(item != NULL){
        return &item->e;
    } else {
        return NULL;
    }
}

void ht_delete_all(ht_table_t *table) {
    ht_item_t *active;

    if (table != NULL){
        for(int i = 0; i < HT_SIZE; i++){
            if(((*table)[i]) != NULL){
                active = (*table)[i];
                free(active);
                (*table)[i] = NULL;
            }
        }
    } else {
      return;
    }
}