#include <stdio.h>

#define SIZE 20

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
	KONK, // konkatenace . 14
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
} expr_type;

typedef struct stack_entry{
	char *data;
	struct stack_entry *next;
};

struct stack_t {
	struct stack_entry *head;
	size_t stack_size;
};

struct stack_t *newStack(void);
char *copyString(char *str);
void push(struct stack_t *theStack, char *value);
char *top(struct stack_t *theStack);
void pop(struct stack_t *theStack);
void clear (struct stack_t *theStack);
void destroyStack(struct stack_t **theStack);
void expression(char *neviem);
void init();
