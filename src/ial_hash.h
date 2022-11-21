/*
 * Hlavičkový súbor pre tabuľku s rozptýlenými položkami.
 * Tento súbor neupravujte.
 */

#ifndef IAL_HASHTABLE_H
#define IAL_HASHTABLE_H

#include <stdbool.h>
#include "scanner.h"

/*
 * Maximálna veľkosť poľa pre implementáciu tabuľky.
 * Funkcie pracujúce s tabuľkou uvažujú veľkosť HT_SIZE.
 */
#define MAX_HT_SIZE 1000

/*
 * Veľkosť tabuľky s ktorou pracujú implementované funkcie.
 * Pre účely testovania je vhodné mať možnosť meniť veľkosť tabuľky.
 * Pre správne fungovanie musí byť veľkosť prvočíslom.
 */
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
    bool nullRet;
} element;

// Prvok tabuľky
typedef struct ht_item {
  char key;            // kľúč prvku
  element* e;         //ukazovateľ na to čo to je
} ht_item_t;

// Tabuľka o reálnej veľkosti MAX_HT_SIZE
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

int get_hash(char *key);
void ht_init(ht_table_t *table);
ht_item_t *ht_search(ht_table_t *table, char *key);
void ht_insert(ht_table_t *table, char *key, element *data);
element *ht_get(ht_table_t *table, char *key);
void ht_delete_all(ht_table_t *table);


#endif
