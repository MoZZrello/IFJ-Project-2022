/*
 * Created by Terezia Hundakova, Jasmína Csalova and Richard Harman on 16. 11. 2022
 */

#ifndef IFJ_PROJECT_2022_PARSER_H
#define IFJ_PROJECT_2022_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "expr.h"
#include "errors.h"
#include "list.h"
#include "generator.h"

static bool end_file = false;

/**
 *@brief Structre to keep program data for semantic control
 */
typedef struct progdata{
    int lastFuncKey;
    bool inFunction;
    int inIF;
    int inWhile;
    int inElse;
    bool returned;
    char* definedFunctions;
    int funcCounter;
    char* definedVars;
    int varCounter;
} progdata;

/**
 *@brief Enum types of last called function type
 */
typedef enum{
    IF,
    WHILE,
    ELSE,
} lastCalled;

/**
 *@brief Functions for syntax control based on LL grammar
 */
void prolog();
void body();
void stmt();
void end();
void else_stmt();
void stmt_list();
void func();
void ret_type();
void data_type();
void args();
void arg_def();
void arg_list();

/**
 *@brief Functions to create elements and pass them to hashtable
 */
void antilog(ht_table_t *table);
ht_table_t* sortSem(ht_table_t *table, int *retKey);
ht_table_t* addBuiltInFuncs(ht_table_t *table, int *retKey);
element sem_func();
element sem_return(bool in_fce);
element sem_if_while(bool in_fce);
element sem_else();
element sem_var(bool in_fce);
element sem_identif();

/**
 *@brief Functions for semantics control based on LL grammar
 */
void check_var_calls(ht_table_t *table, element e, int key);
void check_args_name(element e);
void semControl(ht_table_t *table, int key);
void check_sem_return(element func_e, element ret_e);
void check_global_return(element ret_e);
void check_defined_functions(progdata data, char* name);
void see_call_defined(ht_table_t *table, element call, int key);
void see_call_arguments(ht_table_t *table, element func, element call, int key);

/**
 *@brief Function to get variables current type
 */
Token get_variable(ht_table_t *table, element* e, Token var, int key);


#endif //IFJ_PROJECT_2022_PARSER_H