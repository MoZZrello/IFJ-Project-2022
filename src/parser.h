//
// Created by Richard Harman on 16. 11. 2022.
//

#ifndef IFJ_PROJECT_2022_PARSER_H
#define IFJ_PROJECT_2022_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "strings.h"
#include "expr.h"
#include "errors.h"

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

string str;
Token token;
extern bool end_file = false;
bool sem_find = false;
bool brac_find = false;
bool literal = false;
bool ret = false;
int brac_count = 0;

#endif //IFJ_PROJECT_2022_PARSER_H
