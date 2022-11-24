#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "list.h"
#include "error.h"

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
	END, // $ 15

	// nonterminal
	NONTERM, // pravidlo r 16
	G, // 17
	L, // 18
	E, // 19
	X, // 20
	ENDSTACK, // 21
	ERR // 22
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