#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "list.h"
#include "errors.h"
#include "symtable.h"

typedef enum{
	VALUE, // promena 0
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
	

	// nonterminal
	NONTERM, // pravidlo r 17
	G, // 18
	L, // 19
	E, // 20
	X, // 21
	ENDSTACK, // 22
	ERR // 23
} expr_symb;

typedef enum{
	R_ADD, //E + E
	R_SUB, // E - E
	R_MUL, // E * E
	R_DIV, // E / E
	R_G, // E > E
	R_GE, // E >= E
	R_L, // E < E
	R_LE, // E <= E
	R_EQ, // E == E
	R_NEQ, // E != E
	R_KONK // E . E	
} rules;

typedef struct stack_item{
	Token token;
	expr_symb symbol;
	struct stack_item *next;
} stack_item_t;

struct stack_t {
	stack_item_t *head;
	size_t stack_size;
};

///linked list pre premenne
typedef enum{
	D_NUM, //0
	D_DECM_NUM, //1 
	D_EXP_NUMB, //2
	D_STRING, //3
	D_NULL, //4
	D_NON //5
} d_list_types;

struct variables {
   char *data;
   d_list_types type;
   struct variables *next;
};

//linked list pre funkciu
struct functions {
   char *fce_name;
   d_list_types return_type;
   bool can_be_null;
   struct functions *next;
};

void newStack(struct stack_t *theStack);
void push(struct stack_t *theStack, expr_symb symb, Token value);
void pop(struct stack_t *theStack);
expr_symb top(struct stack_t *theStack);
void clear (struct stack_t *theStack);
void destroyStack(struct stack_t **theStack);
void expression(Token *token, bool var);
expr_symb token_to_index(int token);
expr_symb skip_term(struct stack_t *stack);

void equal(struct stack_t *stack, expr_symb symb, Token *token);
void less(struct stack_t *stack, expr_symb symb, Token *token);
void greater(struct stack_t *stack);

void printstack(struct stack_t *stack);

Token exp_sem_var(element *e, bool in_func);
void exp_sem_func(element *e);
Token exp_sem_return(element *e, bool in_func);
Token exp_sem_ifwhile(element *e, bool in_func);

void insert_first(char *data, d_list_types d_type);
void deleteFirst();
struct variables* find(char *key);
struct variables* delete(char *key);
void printList(); 
d_list_types token_to_d_type(int d_type);
d_list_types kw_to_d_type(int kw_type);
d_list_types find_in_list(char *fce_name);
KeywordType d_type_to_kw(int d_type);
char *d_type_to_info(int d_type);

void insert_first_fce(char *data, d_list_types d_type, bool null_ret);
struct functions* find_fce(char *key);
void print_list_fce();

int get_position(char *key);
