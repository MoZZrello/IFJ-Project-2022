#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "list.h"

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
	D_DOT,
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
void expression(Token *token);
expr_symb token_to_index(int token);
expr_symb skip_term(struct stack_t *stack);

void equal(struct stack_t *stack, expr_symb symb, Token *token);
void less(struct stack_t *stack, expr_symb symb);
void greater(struct stack_t *stack);