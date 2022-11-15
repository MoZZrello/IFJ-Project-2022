#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"
#include "errors.h"
#include "expr.h"

int table [SIZE][SIZE] = {
    /*          +      -     *      /    //    (     )     <     >    <=    >=     ==    ~=    ..     #    ID   INT   NUM   STR    $    */
    /* +  */ {  G , G , L , L , L , L , G , G , G , G , G , G , G , G , L , L , L , L , L , G   },
    /* -  */ {  G , G , L , L , L , L , G , G , G , G , G , G , G , G , L , L , L , L , X , G   },
    /* *  */ {  G , G , G , G , G , L , G , G , G , G , G , G , G , G , L , L , L , L , X , G   },
    /* / */  {  G , G , G , G , G , L , G , G , G , G , G , G , G , G , L , L , L , L , X , G   },
    /* // */ {  G , G , G , G , G , L , G , G , G , G , G , G , G , G , L , L , L , L , X , G   },
    /*  ( */ {  L , L , L , L , L , L , E , L , L , L , L , L , L , L , L , L , L , L , L , X   },
    /*  ) */ {  G , G , G , G , G , X , G , G , G , G , G , G , G , G , G , L , L , L , L , G   },
    /*  < */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /*  > */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /* <= */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /* >= */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /* == */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /* ~= */ {  L , L , L , L , L , L , G , X , X , X , X , X , X , L , G , L , L , L , L , G   },
    /* .. */ {  L , L , L , L , L , L , G , G , G , G , G , G , G , G , L , L , L , L , L , G   },
    /*  # */ {  G , G , G , G , G , G , G , G , G , G , G , G , G , G , G , L , L , L , L , G   },
    /* ID  */{  G , G , G , G , G , G , G , G , G , G , G , G , G , G , G , X , X , X , X , G   },
    /* INT */{  G , G , G , G , G , G , G , G , G , G , G , G , G , G , G , X , X , X , X , G   },
    /* NUM */{  G , G , G , G , G , G , G , G , G , G , G , G , G , G , G , X , X , X , X , G   },
    /* STR */{  X , X , X , X , X , G , G , G , G , G , G , G , G , G , G , X , X , X , X , G   },
    /*  $  */{  L , L , L , L , L , L , X , L , L , L , L , L , L , L , L , L , L , L , L , X   }
};

struct stack_t *nas_stack;

struct stack_t *newStack(void)
{
  struct stack_t *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stack_size = 0;
  }
  return stack;
}

/**
 * Make a copy of the string to be stored (assumes  
 * strdup() or similar functionality is not
 * available
 */
char *copyString(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

/**
 * Push a value onto the stack
 */
void push(struct stack_t *theStack, char *value)
{
  struct stack_entry *entry = malloc(sizeof *entry); 
  if (entry)
  {
    entry->data = copyString(value);
    entry->next = theStack->head;
    theStack->head = entry;
    theStack->stack_size++;
  }
  else
  {
    // handle error here
  }
}

/**
 * Get the value at the top of the stack
 */
char *top(struct stack_t *theStack)
{
  if (theStack && theStack->head)
    return theStack->head->data;
  else
    return NULL;
}

/**
 * Pop the top element from the stack; this deletes both 
 * the stack entry and the string it points to
 */
void pop(struct stack_t *theStack)
{
  if (theStack->head != NULL)
  {
    struct stack_entry *tmp = theStack->head;
    theStack->head = theStack->head->next;
    free(tmp->data);
    free(tmp);
    theStack->stack_size--;
  }
}

int empty(struct stack_t *stack) {
  if(stack-> head != NULL) //??stack top 
    return 1;
  else
    return 0;
}

/**
 * Clear all elements from the stack
 */
void clear (struct stack_t *theStack)
{
  while (theStack->head != NULL)
    pop(theStack);
}

/**
 * Destroy a stack instance
 */
void destroyStack(struct stack_t **theStack)
{
  clear(*theStack);
  free(*theStack);
  *theStack = NULL;
}

void expression(char * neviem) {
    push(&(*nas_stack), &(*neviem));

    for(int i = 0; i < nas_stack->stack_size; i--) {
        printf("obsah stacku je %s\n", nas_stack->head->data);
    }
}

void init() {
    nas_stack = newStack();
}

expr_type token_to_expr(char *token) {
    if (!strcmp(token,"|LEFT BRACKET|"))
      return BRACE_L;
    else if (!strcmp(token,"|RIGHT BRACKET|"))
      return BRACE_R;
    else if(!strcmp(token, "|PLUS|"))
      return PLS;
    else if (!strcmp(token, "|MINUS|"))
      return MNS;
    else if(!strcmp(token, "|COMMA|"))
      return COM;
    else if(!strcmp(token, "|MULTIPLY|"))
      return MUL;
    else if (!strcmp(token, "|DOT|"))
      return KONK;
    else if (!strcmp(token, "|DOUBLE DOT|"))
      return D_DOT;
    else if(!strcmp(token, "|DIVIDE|"))
      return DIV;
    else if(!strcmp(token, "|GREATER|"))
      return GTH;
    else if(!strcmp(token, "|GREATER EQUAL|"))
      return GTHE;
    else if(!strcmp(token, "|LESS|"))
      return LTH;
    else if(!strcmp(token, "|LESS EQUAL|"))
      return LTH;
    else if(!strcmp(token, "|EQUAL|"))
      return EQ;
    else if(!strcmp(token, "|NOT EQUAL|"))
      return NEQ;
    else if(!strcmp(token, "|VAR_ID|") || !strcmp(token, "|IDENTIFIER|") || !strcmp(token, "|STRING|") || !strcmp(token, "|NUMBER|") || !strcmp(token, "|DECIMAL NUMBER|") || !strcmp(token, "|EXPONENT NUMBER|"))
      return VALUE;
    else
      return ERR;
}

expr_type skip_non_term(struct stack_t *stack) {
  /*stack_t tmp_stack;
  tmp_stack = newStack();

  expr_type b;
  while(1) {
    b = top(&stack);
    if(b < NONTERM) {
      while(!empty(&tmp_stack)) {
        push(stack, tmp_stack->head->data);
        pop(&tmp_stack)
      }
      break;
    }
    else {
      push(&tmp_stack, stack->head->data);
      pop(stack);
    }
  }*/
}

