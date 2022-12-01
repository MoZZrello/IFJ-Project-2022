//
// Created by Marek Spirka on 27. 11. 2022.
//

#ifndef SCANNER_C_GENERATOR_H
#define SCANNER_C_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ADD (var) (symb1) (symb2)
 * SUB (var) (symb1) (symb2)
 * MUL (var) (symb1) (symb2)
 * DIV (var) (symb1) (symb2)
 * IDIV (var) (symb1) (symb2)
 * LT/GT/EQ (var) (symb1) (symb2)
 * STRI2INT (var) (symb1) (symb2)
 * CONCAT (var) (symb1) (symb2)
 * GETCHAR (var) (symb1) (symb2)
 * SETCHAR (var) (symb1) (symb2)
 * JUMPIFEQ (label) (symb1) (symb2)
 * JUMPIFNEQ (label) (symb1) (symb2)
 */
void PRINT_LANE_THREE_ARG(char* name, char* arg1, char* arg2, char* arg3);
/*
 * MOVE (var) (symb)
 * INT2FLOAT (var) (symb)
 * FLOAT2INT (var) (symb)
 * INT2CHAR  (var) (symb)
 * READ (var) (type)
 * STRLEN (var) (symb)
 * TYPE (var) (symb)
 */
void PRINT_LANE_TWO_ARG(char* name, char* arg1, char* arg2);
/*
 * DEFVAR (var)
 * CALL (label)
 * PUSH (symb)
 * POPS (var)
 * WRITE (symb)
 * LABEL (label)
 * JUMP (label)
 * EXIT (symb)
 * DPRINT (symb)
 */
void PRINT_LANE_ONE_ARG(char* name, char* arg1);
/*
 * CREATEFRAME
 * PUSHFRAME
 * POPFRAME
 * RETURN
 * CLEARS
 * LTS/GTS/EQS
 * ANDS/ORS/NOTS
 * INT2FLOATS/FLOAT2INTS/INT2CHARS/STRI2INTS
 * BREAK
 */
void PRINT_LANE_ZERO_ARG(char* name);

void start_program();

//Vstavane funckie
void func_reads();      //done I guess
void func_readi();      //done I guess
void func_readf();      //done I guess
void func_write();      //funguje- skontrolovane (este skontrolovat)
void func_floatval();   //funguje--- skontrolovane
void func_intval();     //funguje--- skontrolovane
void func_strval();     //funguje--- skontrolovane (ide len string->string a null->string@)
void func_strlen();     //funguje--- skontrolovane
void func_substring();  //funguje--- skontrolovane
void func_ord();        //funguje--- skontrolovane
void func_chr();        //funguje--- dorobit chybovy stav 58!!!!

#endif //SCANNER_C_GENERATOR_H