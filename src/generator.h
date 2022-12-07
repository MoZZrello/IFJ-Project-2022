/*
 * Created by Marek Spirka and Richard Harman on 27.11.2022
 */

#ifndef SCANNER_C_GENERATOR_H
#define SCANNER_C_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"
#include "list.h"

/**
 *@brief Structre to save return types of functions
 */
typedef struct{
    int return_type;
    bool canBeNull;
} RetType;

/**
 *@brief functions that print commands in the IFJcode22 language
 *@param name -> command name
 *@param arg1 -> first arg of command
 *@param arg2 -> first arg of command
 *@param arg3 -> first arg of command
 */
void PRINT_LANE_THREE_ARG(char* name, char* arg1, char* arg2, char* arg3);
void PRINT_LANE_TWO_ARG(char* name, char* arg1, char* arg2);
void PRINT_LANE_ONE_ARG(char* name, char* arg1);
void PRINT_LANE_ZERO_ARG(char* name);

/**
 *@brief The function calls the functions that compose the entire IFJcode22 code
 *@param table -> place where the entire code from input is stored
 *@param no_build_in_func -> ID of element in Table
 */
void gen_program(ht_table_t *table, int no_build_in_func);
/**
 *@brief The function prints the program header and a reference to main
 */
void start_program();
/**
 *@brief A function that prints an user-defined function
 *@param ht_table_t *table -> pointer to place where the entire code from input is stored
 */
void function_gen(ht_table_t *table);
/**
 *@brief A function that prints an user-defined function
 *@param element* e -> pointer on element from table
 *@param ht_table_t *table -> pointer to table
 */
RetType def_func_start(element* e, ht_table_t *table);
/**
 *@brief A function defines and assigns values to the variable arguments that are passed to the function
 *@param element* e -> pointer on element from table
 */
void func_arg_print(element* e);
/**
 * @brief A function that determines the return value of a function
 * @param element* e -> pointer on element from table
 * @param RetType ret_type -> return function type
 */
void func_return(element* e, RetType ret_type);
/**
 *@brief A function that prints the function body
 *@param element* e -> pointer on element from table
 */
void func_main_print(ht_table_t *table, element* e, RetType ret_type, int *key);
/**
 *@brief A function that declares the arguments to be passed to the function
 *@param element call -> the arguments we want to push for the called function
 *@param ht_table_t *table -> pointer to place where the entire code from input is stored
 */
void gen_func_call(ht_table_t *table, element call);
/**
 *@brief A function that calls a function
 *@param element call -> name of the function
 */
void func_call(char* call );
/**
 *@brief A function that generates main
 *@param ht_table_t *table -> place where the entire code from input is stored
 *@param key -> ID of element in Table
 */
void gen_main(ht_table_t *table, int key);
/**
 *@brief A function that retypes argument to the required form LF@ / int@ / float@ / string@
 *@param Token arg - the input argument that we cast
 */
char *retype_string(Token arg);
/**
 *@brief A function that converts hexadecimal notation to octal notation
 *@param const char hex[] - Input argument in hexa form
 */
long long hexa_to_octal(const char hex[]);
/**
 *@brief When a built-in function is called, the function is printed at the beginning of the program,
 *each function is called only once
 *@param table -> place where the entire code from input is stored
 *@param no_build_in_func -> ID of element in Table
 */
void gen_built_in_functions(ht_table_t *table, int key);
/**
 * @brief A built-in function that reads strings from standard input
 * -string pushes on the stack
 */
void func_reads();
/**
 * @brief A built-in function that reads int from standard input
 * -int pushes on the stack
 */
void func_readi();
/**
 * @brief A built-in function that reads float from standard input
 * -float pushes on the stack
 */
void func_readf();
/**
 * @brief Built-in function for printing
 * The number of arguments must be pushed on the stack, and given arguments
 */
void func_write();
/**
 * @brief Built-in function to change INT to FLOAT
 * It is necessary to push INT number to the function via stack
 */
void func_floatval();
/**
 * @brief Built-in function to change FLOAT to INT
 * It is necessary to push FLOAT number to the function via stack
 */
void func_intval();
/**
 * @brief In our case, the function just returns a string or nill
 */
void func_strval();
/**
 * @brief Built-in function returns the length of the argument that was pushed on stack
 */
void func_strlen();
/**
 * @brief Built-in function returns the substring of the string that was pushed on stack
 */
void func_substring();
/**
 * @brief Built-in function to change first CHAR of STRINF to INT
 * It is necessary to push STRING to the function via stack
 */
void func_ord();
/**
 * @brief Built-in function to change INT to CHAR
 * It is necessary to push INT to the function via stack
 */
void func_chr();

/**
 *@brief A function that assignes function call to a variable
 *@param element* e -> pointer on element from table
 */
void func_call_asign(element *e);

/**
 *@brief A function to print and assign an expression to a variable
 *@param element* e -> pointer on element from table
 */
void var_expr_gen(element *e);

/**
 *@brief A function that prints the return expression
 *@param element* e -> pointer on element from table
 */
void return_expr(element *e);

/**
 *@brief A function that generates IF
 *@param ht_table_t* t -> pointer to hashtable
 *@param element* e -> pointer on element from table
 */
void gen_if(ht_table_t * t, element *e);

/**
 *@brief A function that generates WHILE
 *@param ht_table_t* t -> pointer to hashtable
 *@param element* e -> pointer on element from table
 */
void gen_while(ht_table_t *t, element *e);

/**
 *@brief Checks if a variable has already been defined in code
 *@param char* var_list -> string of all variables already defined
 *@param char* var -> name of variable found
 */
bool is_var_new(char* var_list, char* var);

#endif //SCANNER_C_GENERATOR_H
