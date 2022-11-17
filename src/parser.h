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
#include "list.h"

static bool end_file = false;

void prolog(Token *t);
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

#endif //IFJ_PROJECT_2022_PARSER_H