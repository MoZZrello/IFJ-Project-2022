//
// Created by Richard Harman on 16. 11. 2022.
//

#ifndef IFJ_PROJECT_2022_PARSER_H
#define IFJ_PROJECT_2022_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "expr.h"
#include "errors.h"
#include "list.h"

static bool end_file = false;

void prolog();
void body();
void stmt();
void end();
void expr_skip();
void else_stmt();
void stmt_list();
void func();
void ret_type();
void data_type();
void args();
void arg_def();
void arg_list();

void antilog(ht_table_t *table);
element* sortSem(element* elementList, int* retKey);
element sem_func();
element sem_return();
element sem_if_while();
element sem_else();
element sem_var();
void check_sem_return(element func_e, element ret_e);

#endif //IFJ_PROJECT_2022_PARSER_H