#include "strings.h"
#include "errors.h"
#include "expr.h"

#define SIZE 17
bool rdc = false;

int table [SIZE][SIZE] = {
				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/	  {X, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
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

//struct stack_t stack;

void newStack(struct stack_t *theStack) {
    theStack->head = NULL;
    theStack->stack_size = 0;
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
    callError(ERR_INTERNAL);
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
  if(theStack -> head != NULL) 
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

void expression(Token *token, bool var) {
    struct stack_t stack;
    newStack(&stack);

    expr_symb symb_a;
    expr_symb symb_b;

    push(&stack, ENDSTACK, *token);
    push(&stack, END, *token);

    struct stack_t tmp_stack;
    newStack(&tmp_stack);
    if(var == true) {
      while (token->type != SEMICOLON) {
        //printf("token nam chodi %d\n", token->type);
        symb_a = token_to_index(token->type);
        symb_b = top(&stack);

        if(symb_b == NONTERM) {
          expr_symb symb;
          while (1) {
            symb = top(&stack);
                    //printf(" top je %d\n", symb);
            if(symb < NONTERM) {
              while(empty(&tmp_stack)) {
                push(&stack, top(&tmp_stack), tmp_stack.head->token);
                pop(&tmp_stack);
              }
              break;
            }
            else {
              push(&tmp_stack, symb, stack.head->token);
              pop(&stack);
            }
          }
          symb_b = symb;
        }

        //printf("b je %d a je %d\n", symb_b, symb_a);

        if(symb_b > END) {
          pop(&stack);
          callError(ERR_SYN);
        }

        //printf("vrateny znk z tabulkky je %d\n", table[symb_b][symb_a]);
        switch (table[symb_b][symb_a]) {
          case E:
              equal(&stack, symb_a, &(*token));
              break;
          case L:
              less(&stack, symb_a, &(*token));
              break;
          case G:
              //printf("top zasobnika je %d\n", top(&stack));
              greater(&stack);
              rdc = true;
              break;
          case X:
              clear(&stack);
              //todo error prazdne miesto z tabulky
              //printf("error prazdna tabulka\n");
              callError(ERR_SYN);
              break;
          default:
              clear(&stack);
              //todo error
              //printf("error neexistujuca hodnota\n");
              callError(ERR_SYN);
              break;
        }

        if(!rdc) {
          *token = getTokenFromList();
        }
        else {
          rdc = false;
        }
      } 
            //printf("vyskocim z podmienke\n");
    }
    else if(var == false) {
      while (token->type != RIGHT_BRACKET) {
        symb_a = token_to_index(token->type);
        symb_b = top(&stack);

        if(symb_b == NONTERM) {
          expr_symb symb;
          while (1) {
            symb = top(&stack);
                    //printf(" top je %d\n", symb);
            if(symb < NONTERM) {
              while(empty(&tmp_stack)) {
                push(&stack, top(&tmp_stack), tmp_stack.head->token);
                pop(&tmp_stack);
              }
              break;
            }
            else {
              push(&tmp_stack, symb, stack.head->token);
              pop(&stack);
            }
          }
          symb_b = symb;
        }

       // printf("b je %d a je %d\n", symb_b, symb_a);

        if(symb_b > END) {
          pop(&stack);
          //printf("error\n");
          callError(ERR_SYN);
        }

        //printf("vrateny znk z tabulkky je %d\n", table[symb_b][symb_a]);
        switch (table[symb_b][symb_a]) {
          case E:
              equal(&stack, symb_a, &(*token));
              break;
          case L:
              less(&stack, symb_a, &(*token));
              break;
          case G:
              //printf("top zasobnika je %d\n", top(&stack));
              greater(&stack);
              rdc = true;
              break;
          case X:
              clear(&stack);
              //todo error prazdne miesto z tabulky
              //printf("error prazdna tabulka\n");
              callError(ERR_SYN);
              break;
          default:
              clear(&stack);
              //todo error
              //printf("error neexistujuca hodnota\n");
              callError(ERR_SYN);
              break;
        }

        if(!rdc) {
          *token = getTokenFromList();
        }
        else {
          rdc = false;
        }
      } 
    }
  clear(&stack);
  clear(&tmp_stack);  
  //destroyStack(*(*stack));
  //destroyStack(*(&tmp_stack));
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

void equal(struct stack_t *stack, expr_symb symb, Token *token) {
  //*token = getTokenFromList(); //??? ma tu byt
  push(stack, symb, *token);
}

void less(struct stack_t *stack, expr_symb symb, Token *t) {
  struct stack_t *curr_ptr = stack;
  stack_item_t *prev_ptr = NULL;
  Token token;
  //printf("%d token je\n", t->type);
  switch(t->type) {
    case NUMBER:
    case DECIMAL_NUMBER:
    case EXPONENT_NUMBER:
      //todo pridat do hash 
      break;
    case VAR_ID:
    case IDENTIFIER:
      token = *t;
      break;  
    case  STRING:
      //todo pridat do hash 
      break;
    default:
      break;
  }
  //printf("idem pushnut %d\n", symb);
  if (top(curr_ptr) <= ENDSTACK) {
    push(stack, L, token);
    push(stack, symb, token);
    return;
  }
  else {
    stack_item_t *tmp_item;
    expr_symb tmp_sym = L;
    tmp_item = malloc(sizeof(struct stack_t));
    if(tmp_item == NULL) {
      clear(stack);
      //todo error
      //printf("chyba pri alokacii\n");
      callError(ERR_INTERNAL);
    }

    tmp_item->symbol = tmp_sym;
    prev_ptr->next = curr_ptr->head;
    curr_ptr->head = prev_ptr;
    push(stack, symb, token);
    return;
  }

  clear(stack);
  //todo error
  //printf("chyba pri less\n");
  callError(ERR_SYN);
}

void greater(struct stack_t *stack) {
  struct stack_t *curr_ptr = stack;
  struct stack_t tmp_stack;
  newStack(&tmp_stack);

  //printf("vrchol stacku je %d\n", stack->head->symbol);
  while(top(stack) != ENDSTACK) {
    if(top(curr_ptr) != L) {
      push(&tmp_stack,top(curr_ptr), curr_ptr->head->token);
      curr_ptr->head = curr_ptr->head->next;
    }
    else {
      curr_ptr->head = curr_ptr->head->next;
      switch (top(&tmp_stack)) {
        //EXPR → literal 
        case VALUE: {
            Token tmp_token = tmp_stack.head->token;
            pop(&tmp_stack);
            if(empty(&tmp_stack)) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error rule EXPR → literal\n");
              callError(ERR_SYN);
            }
            push(curr_ptr, NONTERM, tmp_token);
            break;
          }
        //EXPR → ( EXPR ) 
        case BRACE_L:
            if(top(&tmp_stack) != BRACE_L) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error v E -> (E)\n");
              callError(ERR_SYN);
            }
            pop(&tmp_stack);
            if(top(&tmp_stack) != NONTERM) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error v E -> (E)\n");
              callError(ERR_SYN);
            }
            Token tmp_token = tmp_stack.head->token;
            pop(&tmp_stack);
            if(top(&tmp_stack) != BRACE_L) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error v E -> (E)\n");
              callError(ERR_SYN);
            }
            pop(&tmp_stack);
            if(empty(&tmp_stack)) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error v E -> (E)\n");
              callError(ERR_SYN);
            }
            push(curr_ptr, NONTERM, tmp_token);
            break;
          //// E-> E op E
        case NONTERM: 
            printf("tu som v grt\n");
            if(top(&tmp_stack) != NONTERM) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("error v operandoch\n");
              callError(ERR_SYN);
            }
            Token token = tmp_stack.head->token;
            pop(&tmp_stack);
            if(top(&tmp_stack) > COM) {
              if(top(&tmp_stack) != KONK) {
                clear(&tmp_stack);
                clear(stack);
                //todo error
                //printf("error v operandoch\n");
                callError(ERR_SYN);
              }
            }
            rules op;
            switch (top(&tmp_stack)) {
              case PLS:
                op = R_ADD;
                break;
              case MNS:
                op = R_SUB;
                break;
              case MUL:
                op = R_MUL;
                break;
              case DIV:
                op = R_DIV;
              case EQ:
                op = R_EQ;
                break;
              case NEQ:
                op = R_NEQ;
                break;
              case GTHE:
                op = R_GE;
                break;
              case LTHE:
                op = R_LE;
                break;
              case GTH:
                op = R_G;
                break;
              case LTH:
                op = R_L;
                break;
              case KONK:
                op = R_KONK;
                break;
              default:
                callError(ERR_SYN);
                break;
            }

            pop(&tmp_stack);
            if(top(&tmp_stack) != NONTERM) {
              clear(&tmp_stack);
              clear(stack);
              //todo erooor
              //printf("error v operandoch\n");
              callError(ERR_SYN);
            }

            Token token2 = tmp_stack.head->token;
            pop(&tmp_stack);

            if(empty(&tmp_stack)) {
              clear(&tmp_stack);
              clear(stack);
              //todo error
              //printf("stack error\n");
              callError(ERR_SYN);
            }

            //???
            //printf("tu v E op E je %d %d\n", token.type, token2.type);
            //printf("som aj tu?\n");
            break;
        default:
          clear(&tmp_stack);
          clear(stack);
          //todo error
          //printf("error v rules\n");
          callError(ERR_SYN);
          break;
      }
      return;
    }

  }
   clear(&tmp_stack);
   clear(stack);
   callError(ERR_SYN);
}