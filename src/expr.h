//
// Created by Terézia Hundáková, Jasmína Csalová on 1. 12. 2022
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "list.h"
#include "errors.h"
#include "symtable.h"

//structure for movement in table
typedef enum{
	VALUE, // 0
	PLS, // + 1
	MNS, // - 2
	MUL, // * 3
	DIV, // / 4
	EQ, // === 5
	NEQ, // !== 6
	GTHE, // >= 7 
	LTHE, // <= 8
	GTH, // > 9
	LTH, // < 10 
	COM, // , 11
	BRACE_L, // ( 12
	BRACE_R, // ) 13		
	KONK, // konkatenacia . 14
	FUNC, // 15
	END, // $ 16
	
	NONTERM, // nonterminal 17
	G, // 18
	L, // 19
	E, // 20
	X, // 21
	ENDSTACK, // 22
	ERR // 23
} expr_symb;

//stack for evaluated tokens
typedef struct stack_item{
	Token token;
	expr_symb symbol;
	struct stack_item *next;
} stack_item_t;

struct stack_t {
	stack_item_t *head;
	size_t stack_size;
};

///list of types for semantics control, list of available types
typedef enum{
	D_NUM, //0
	D_DECM_NUM, //1 
	D_EXP_NUMB, //2
	D_STRING, //3
	D_NULL, //4
	D_NON //5
} d_list_types;

//linked list for storing declared variables
struct variables {
   char *data;
   d_list_types type;
   struct variables *next;
};

//linked list for storing declared functions
struct functions {
   char *fce_name;
   d_list_types return_type;
   bool can_be_null;
   struct functions *next;
};

//functions to work with stack
void newStack(struct stack_t *theStack);
void push(struct stack_t *theStack, expr_symb symb, Token value);
void pop(struct stack_t *theStack);
expr_symb top(struct stack_t *theStack);
void clear (struct stack_t *theStack);
void destroyStack(struct stack_t **theStack);

//main function for expression, which is called in parser.c 
//var indicates if we are in if/while (false - we are looking for ")" as end) or in body of the program (true - we are looking for ";" as end) 
void expression(Token *token, bool var);

//change token to our enum expr_symb type
expr_symb token_to_index(int token);

//functions to work with expressions on stack, called function depends on return type from table
void equal(struct stack_t *stack, expr_symb symb, Token *token);
void less(struct stack_t *stack, expr_symb symb, Token *token);
void greater(struct stack_t *stack);

//functions to control semantics, var e stores list of all arguments from expression 
Token exp_sem_var(element *e, bool in_func);
Token exp_sem_return(element *e, bool in_func);
Token exp_sem_ifwhile(element *e, bool in_func);

//function to store function when declared
void exp_sem_func(element *e);

//functions to work with list of arguments
void insert_first(char *data, d_list_types d_type);
void deleteFirst();
struct variables* find(char *key);
struct variables* delete(char *key);
int get_position(char *key);

//functions to work with list of functions
void insert_first_fce(char *data, d_list_types d_type, bool null_ret);
struct functions* find_fce(char *key);


//functions to convert token.type to enum d_list_types 
d_list_types token_to_d_type(int d_type);
d_list_types kw_to_d_type(int kw_type);

//if function is not declared in list yet, we check if it will be declared later
//it is used only when we are in the body of function declaration 
d_list_types find_in_list(char *fce_name);

//convert to type, which we can store in token structure and then return it back into parser
KeywordType d_type_to_kw(int d_type);
char *d_type_to_info(int d_type);




