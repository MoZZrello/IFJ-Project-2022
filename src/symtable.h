/*
 * Created by Marek Spirka and Jasmina Csalova on 17.11.2022
 *
 * Inspired by the second project from the IAL 2022 course
 */

#ifndef IAL_HASHTABLE_H
#define IAL_HASHTABLE_H

#include <stdbool.h>
#include "scanner.h"

#define MAX_HT_SIZE 65536

extern int HT_SIZE;

typedef struct arg{
    Token arg;
    Token type;
} arg;

typedef struct argList{
    arg* list;
    int len;
} argList;

typedef struct element{
    Token name;
    argList* argslist;
    Token ret_type;
    Token expr;
    bool nullRet;
} element;


typedef struct ht_item {
  char key;
  element e;
} ht_item_t;


typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

int get_hash(char *key);
void ht_init(ht_table_t *table);
ht_item_t *ht_search(ht_table_t *table, char *key);
void ht_insert(ht_table_t *table, char *key, element *data);
element *ht_get(ht_table_t *table, char *key);
void ht_delete_all(ht_table_t *table);

#endif
