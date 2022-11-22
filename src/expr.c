#include "strings.h"
#include "errors.h"
#include "expr.h"

#define SIZE 17
string str;

int table [SIZE][SIZE] = {
				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|FUNC|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/	  {X, G, G, G, G, G, G, G, G, G, G, G, X, G, G, G},
	/* PLUS*/		  {L, G, G, L, L, G, G, G, G, G, G, G, L, G, G, G},
	/* MINUS*/	  {L, G, G, L, L, G, G, G, G, G, G, G, L, G, G, G},
	/* MULTI*/	  {L, G, G, G, G, G, G, G, G, G, G, G, L, G, G, G},
	/* DIV */		  {L, G, G, G, G, G, G, G, G, G, G, G, L, G, G, G},
	/* EQ */		  {L, L, L, L, L, G, G, L, L, L, L, G, L, G, L, G},
	/* NEQ */		  {L, L, L, L, L, G, G, L, L, L, L, G, L, G, L, G},
	/* GTHE */	  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* LTHE */	  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* GTH */		  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* LTH */		  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* COM */		  {L, L, L, L, L, L, L, L, L, L, L, E, L, E, L, X},
	/* BRACE_L */	{L, L, L, L, L, L, L, L, L, L, L, E, L, E, L, X},
	/* BRACE_R */	{X, G, G, G, G, G, G, G, G, G, G, G, X, G, G, G},
	/* KONK */		{L, G, G, L, L, G, G, G, G, G, G, G, L, G, G, G},
	/* END */   	{L, L, L, L, L, L, L, L, L, L, L, X, L, X, L, X}
};

struct stack_t stack;

void newStack(struct stack_t *theStack) {
  //theStack = malloc(sizeof *theStack);
  //if (theStack) {
    theStack->head = NULL;
    theStack->stack_size = 0;
  //}
}

/**
 * Push a value onto the stack
 */
void push(struct stack_t *theStack, expr_symb symb, Token value) {
  stack_item_t *entry = malloc(sizeof(struct stack_item)); 
  if (entry) {
    entry->symbol = symb;
    entry->token =  value;
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stack_size++;
  }
  else {
    clear(theStack);
  }
}

/**
 * Get the value at the top of the stack
 */
expr_symb top(struct stack_t *theStack) {
  if (theStack && theStack->head)
    return theStack->head->symbol;
  else
    return ERR;
}

/**
 * Pop the top element from the stack; this deletes both 
 * the stack entry and the string it points to
 */
void pop(struct stack_t *theStack) {
  if (theStack->head != NULL) {
    stack_item_t *tmp = theStack->head;
    theStack->head = tmp->next;
    free(tmp);
    theStack->stack_size--;
  }
}

int empty(struct stack_t *theStack) {
  if(theStack -> head != NULL) //??stack top 
    return 1;
  else
    return 0;
}

/**
 * Clear all elements from the stack
 */
void clear (struct stack_t *theStack) {
  while (theStack->head != NULL)
    pop(theStack);
}

/**
 * Destroy a stack instance
 */
void destroyStack(struct stack_t **theStack) {
  clear(*theStack);
  free(*theStack);
  *theStack = NULL;
}

//todo vymysliet kde pytat token 
void expression(Token *token) {

    newStack(&stack);

    expr_symb symb_a;
    expr_symb symb_b;
    expr_symb tbl_val;

    push(&stack, ENDSTACK, *token);
    push(&stack, END, *token);

    while ((token->type != SEMICOLON) && (token->type != RIGHT_BRACKET)) {//kym nenarazim na ) alebo ; 
      symb_a = token_to_index(token->type);
      symb_b = skip_term(&stack);
      //symb_b = top(&stack);

      if(symb_b > END) {
        pop(&stack);
        printf("error\n");
      }
       printf("%d %d\n", symb_a, symb_b);
      //tbl_val = table[symb_b][symb_a];
      switch (table[symb_b][symb_a]) {
        case E:
            equal(&stack, symb_a, &(*token));
        case L:
            less(&stack, symb_a);
        case G:
            greater(&stack);
        case X:
            clear(&stack);
            //todo error prazdne miesto z tabulky
            printf("error prazdna tabulka\n");
        default:
            clear(&stack);
            //todo error
            printf("error neexistujuca hodnota\n");
            break;
      }

    }
    
}

expr_symb token_to_index(int token) {
    switch(token) {
      case PLUS:
          return  PLS;
      case MINUS:
          return MNS;
      case RIGHT_BRACKET:
          return BRACE_R;
      case LEFT_BRACKET:
          return BRACE_L;
      case MULTIPLY:
          return MUL;
      case DIVIDE:
          return DIV;
      case EQUAL:
          return EQ;
      case NOT_EQUAL:
          return NEQ;
      case GREATER:
          return GTH;
      case LESS:
          return LTH;
      case GREATER_EQUAL:
          return GTHE;
      case LESS_EQUAL:
          return LTHE;
      case COMMA:
          return COM;
      case DOT:
          return KONK;
      case DOUBLE_DOT:
          return D_DOT;
      case VAR_ID:
      case IDENTIFIER:
      case STRING:
      case NUMBER:
      case DECIMAL_NUMBER:
      case EXPONENT_NUMBER:
          return VALUE;
      default:
          return ERR;
    }
}

expr_symb skip_term(struct stack_t *stack) {
    struct stack_t tmp_stack;
    newStack(&tmp_stack);
    expr_symb symb;

    while (1) {
      symb = top(stack);
      if(symb < NONTERM) {
        while(!empty(&tmp_stack)) {
          push(stack, top(&tmp_stack), tmp_stack.head->token);
          pop(&tmp_stack);
        }
        break;
      }
       else {
        push(&tmp_stack, symb, stack->head->token);
        pop(stack); //todo vyriesit tento seg fault
        printf("tu soom\n");
      }
    }
    
    return symb;   
}

void equal(struct stack_t *stack, expr_symb symb, Token *token) {
  *token = getTokenFromList();
  push(stack, symb, *token);
}

void less(struct stack_t *stack, expr_symb symb) {

}

void greater(struct stack_t *stack) {

}