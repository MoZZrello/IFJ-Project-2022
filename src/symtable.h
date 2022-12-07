/*
 * Created by Marek Spirka and Jasmina Csalova on 17.11.2022
 * Inspired by the second project from the IAL 2022 course
 */

#ifndef IAL_HASHTABLE_H
#define IAL_HASHTABLE_H

#include <stdbool.h>
#include "scanner.h"

#define MAX_HT_SIZE 65536

extern int HT_SIZE;

/**
 * Struct for argument
 */
typedef struct arg{
    Token arg;
    Token type;
} arg;

/**
 * @brief Struct for argumentList
 */
typedef struct argList{
    arg* list;
    int len;
} argList;

/**
 * @brief Struct for element
 */
typedef struct element{
    Token name;
    argList* argslist;
    Token ret_type;
    Token expr;
    bool nullRet;
} element;

/**
 * @brief Struct for item in table
 */
typedef struct ht_item {
  char key;
  element e;
} ht_item_t;

typedef ht_item_t *ht_table_t[MAX_HT_SIZE];
/**
 * @brief Function for creating a hash key for the element that we store in the table
 * @param *key - pointer to the key from which we will create a hash
 */
int get_hash(char *key);
/**
 * @brief Function to create and initialize the sym table
 * @param *table - pointer to the table
 */
void ht_init(ht_table_t *table);
/**
 * @brief Function that uses a hash key to search for an element in the table
 * @param *table - pointer to the table
 * @param *key - pointer to key that we want to find
 */
ht_item_t *ht_search(ht_table_t *table, char *key);
/**
 * @brief Function that stores an element in a table
 * @param *table - pointer to the table
 * @param *key - pointer to the key with which we save the element in the table
 * @param *data - pointer to data that we want to insert
 */
void ht_insert(ht_table_t *table, char *key, element *data);
/**
 * @brief Function to get an element from the table
 * @param *table - pointer to the table
 * @param *key - pointer to the key with which we search in the table
 */
element *ht_get(ht_table_t *table, char *key);
/**
 * @brief Function to delete the whole table
 * @param *table - pointer to the table
 */
void ht_delete_all(ht_table_t *table);

#endif
