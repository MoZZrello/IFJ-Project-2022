//
// Created by Terézia Hundáková, Jasmína Csalová on 1. 12. 2022
//
#include "strings.h"
#include "errors.h"
#include "expr.h"

#define SIZE 18
bool rdc = false;
bool fce = false;
bool sign = false;
struct variables *head_var = NULL;
struct variables *current = NULL;
struct functions *head_var_fce = NULL;
struct functions *current_fce = NULL;

int table [SIZE][SIZE] = {
				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|BRACER_L|BRACE_R|KONK|FUNC|END*/
	/* VALUE*/	  {X, L, L, L, L, L, L, L, L, L, L, L, L, L, L, X, L},
	/* PLUS*/		  {G, E, L, L, L, L, L, L, L, L, L, X, L, L, G, G, L},
	/* MINUS*/	  {G, L, E, L, L, L, L, L, L, L, L, X, L, L, G, G, L},
	/* MULTI*/	  {G, L, L, E, L, L, L, L, L, L, L, X, L, L, G, G, L},
	/* DIV */		  {G, L, L, L, E, L, L, L, L, L, L, X, L, L, G, G, L},
	/* EQ */		  {G, L, L, L, L, G, G, G, G, G, G, X, L, G, L, G, L},
	/* NEQ */		  {G, L, L, L, L, G, G, G, G, G, G, X, L, G, L, G, L},
	/* GTHE */	  {G, L, L, L, L, G, G, G, G, G, G, X, L, L, L, G, L},
	/* LTHE */	  {G, L, L, L, L, G, G, G, G, G, G, X, L, L, L, G, L},
	/* GTH */		  {G, L, L, L, L, G, G, G, G, G, G, X, L, L, L, G, L},
	/* LTH */		  {G, L, L, L, L, G, G, G, G, G, G, X, L, L, L, G, L},
	/* COM */		  {G, X, X, X, X, X, X, X, X, X, X, E, L, L, L, G, L},
	/* BRACE_L */	{G, L, L, L, L, L, L, L, L, L, L, X, E, X, L, G, L},
	/* BRACE_R */	{G, L, L, L, L, L, L, L, L, L, L, L, L, E, G, G, L},
	/* KONK */		{G, L, L, L, L, G, G, G, G, G, G, G, L, G, E, G, L},
  /* FUNC */    {X, L, L, L, L, L, L, L, L, L, L, L, L, L, L, X, L},
	/* END */   	{G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G, X}
  
};

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
    int brac_count = 0;
    struct stack_t stack;
    newStack(&stack);

    expr_symb symb_a;
    expr_symb symb_b;

    push(&stack, ENDSTACK, *token);
    push(&stack, END, *token);

    struct stack_t tmp_stack;
    newStack(&tmp_stack);
    if(var == true) {
      while (token->type != SEMICOLON || brac_count != 0) {
        if(token->type == GREATER || token->type == GREATER_EQUAL || token->type == LESS || token->type == LESS_EQUAL || token->type == ASSIGN || token->type == EQUAL || token->type == NOT_EQUAL) {
          callError(ERR_SYN);
        }
        if(token->isKeyword  && token->kwt == NULL_K) {
          symb_a = VALUE;
        }
        else {
          symb_a = token_to_index(token->type);
        }
        symb_b = top(&stack);

        if(sign == true && (symb_b == PLS || symb_b == MNS || symb_b == MUL || symb_b == DIV)) {
          if(symb_a != VALUE) {
            callError(ERR_SYN);
          }
        }
        else if(symb_b == BRACE_R && (symb_a == VALUE || symb_a == BRACE_L)) {
          callError(ERR_SYN);
        }
        else if(symb_b == VALUE && symb_a == BRACE_L) {
          callError(ERR_SYN);
        }
        else if(symb_b == COMMA && symb_a != VALUE) {
          callError(ERR_SYN);
        }
        else if(symb_b == FUNC && symb_a != BRACE_L) {
          callError(ERR_SYN);
        }

        if(symb_b == NONTERM) {
          expr_symb symb;
          while (1) {
            symb = top(&stack);
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


        if(symb_b > END) {
          pop(&stack);
          callError(ERR_SYN);
        }

        switch (table[symb_a][symb_b]) {  
          case E:
              equal(&stack, symb_a, &(*token));
              break;
          case L:
              less(&stack, symb_a, &(*token));
              break;
          case G:
              greater(&stack);
              rdc = true;
              break;
          case X:
              clear(&stack);
              callError(ERR_SYN);
              break;
          default:
              clear(&stack);
              callError(ERR_SYN);
              break;
        }

        if(token->type == LEFT_BRACKET && rdc == false) {
          brac_count++;
        }
        else if(token->type == RIGHT_BRACKET && rdc == false) {
          brac_count--;
        }

        if(!rdc) {
          *token = getTokenFromList();
          if(token->type == SEMICOLON && sign == true) {
            if(symb_b == PLS || symb_b == MNS || symb_b == MUL || symb_b == DIV) {
              callError(ERR_SYN);
            }
          }
        }
        else {
          rdc = false;
        }
        sign = false;
      } 
    }
    else if(var == false) {
      while (token->type != RIGHT_BRACKET || brac_count != 1) {
        if(token->isKeyword  && token->kwt == NULL_K) {
          symb_a = VALUE;
        }
        else {
          symb_a = token_to_index(token->type);
        }
        symb_b = top(&stack);

        if(sign == true && (symb_b == PLS || symb_b == MNS || symb_b == MUL || symb_b == DIV)) {
          if(symb_a != VALUE) {
            callError(ERR_SYN);
          }
        }
        else if(symb_b == BRACE_R && (symb_a == VALUE || symb_a == BRACE_L)) {
          callError(ERR_SYN);
        }
        else if(symb_b == VALUE && symb_a == BRACE_L) {
          callError(ERR_SYN);
        }
        else if(symb_b == COMMA && symb_a != VALUE) {
          callError(ERR_SYN);
        }

        if(symb_b == NONTERM) {
          expr_symb symb;
          while (1) {
            symb = top(&stack);
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

        if(symb_b > END) {
          pop(&stack);
          callError(ERR_SYN);
        }
        switch (table[symb_a][symb_b]) {
          case E:
              equal(&stack, symb_a, &(*token));
              break;
          case L:
              less(&stack, symb_a, &(*token));
              break;
          case G:
              greater(&stack);
              rdc = true;
              break;
          case X:
              clear(&stack);
              callError(ERR_SYN);
              break;
          default:
              clear(&stack);
              callError(ERR_SYN);
              break;
        }

        if(token->type == LEFT_BRACKET) {
          brac_count++;
        }
        else if(token->type == RIGHT_BRACKET && rdc == false) {
          brac_count--;
        }

        if(!rdc) {
          *token = getTokenFromList();
          if(token->type == RIGHT_BRACKET && sign == true) {
            if(symb_b == PLS || symb_b == MNS || symb_b == MUL || symb_b == DIV) {
              callError(ERR_SYN);
            }
          }
        }
        else {
          rdc = false;
        }
        sign = false;
      } 
    }
  clear(&stack);
  clear(&tmp_stack);  
  
}

expr_symb token_to_index(int token) {
    switch(token) {
      case PLUS:
          sign = true;
          return  PLS;
      case MINUS:
          sign = true;
          return MNS;
      case RIGHT_BRACKET:
          return BRACE_R;
      case LEFT_BRACKET:
          return BRACE_L;
      case MULTIPLY:
          sign = true;
          return MUL;
      case DIVIDE:
          sign = true;
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
      case IDENTIFIER: 
          return FUNC;   
      case VAR_ID:
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
  push(stack, symb, *token);
}

void less(struct stack_t *stack, expr_symb symb, Token *t) {
  struct stack_t *curr_ptr = stack;
  stack_item_t *prev_ptr = NULL;
  Token token;
  switch(t->type) {
    case NUMBER:
    case DECIMAL_NUMBER:
    case EXPONENT_NUMBER:
      break;
    case VAR_ID:
    case IDENTIFIER:
      token = *t;
      break;  
    case  STRING:
      break;
    default:
      break;
  }
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
      callError(ERR_INTERNAL);
    }

    tmp_item->symbol = tmp_sym;
    prev_ptr->next = curr_ptr->head;
    curr_ptr->head = prev_ptr;
    push(stack, symb, token);
    return;
  }

  clear(stack);
  callError(ERR_SYN);
}

void greater(struct stack_t *stack) {
  struct stack_t *curr_ptr = stack;
  struct stack_t tmp_stack;
  newStack(&tmp_stack);

  while(top(stack) != ENDSTACK) {
    if(top(curr_ptr) != L) {
      push(&tmp_stack,top(curr_ptr), curr_ptr->head->token);
      curr_ptr->head = curr_ptr->head->next;
    }
    else {
      curr_ptr->head = curr_ptr->head->next;
      switch (top(&tmp_stack)) {
        case FUNC:
        case VALUE: {
            Token tmp_token = tmp_stack.head->token;
            pop(&tmp_stack);
            if(empty(&tmp_stack)) {
              clear(&tmp_stack);
              clear(stack);
              callError(ERR_SYN);
            }
            push(curr_ptr, NONTERM, tmp_token);
            break;
          }
        default:
          clear(&tmp_stack);
          clear(stack);
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

/////////////////////////////////////////////////////////////////////////////
//semantika
void insert_first(char *data, d_list_types d_type) {
   struct variables *link = (struct variables*) malloc(sizeof(struct variables));
	
   link->data = data;
   link->type = d_type;
	
   link->next = head_var;
	
   head_var = link;
}

void deleteFirst() {
   struct variables *tmp;
   if(head_var == NULL) {
    callError(ERR_INTERNAL);
    return;
   }

   tmp = head_var;
   head_var = head_var->next;
}

struct variables* find(char *key) {
   struct variables* current = head_var;

   if(head_var == NULL) {
      return NULL;
   }

   while(strcmp(current->data, key)) {
      if(current->next == NULL) {
         return NULL;
      } else {
         current = current->next;
      }
   }      
   return current;
}

struct variables* delete(char *key) {
   struct variables* current = head_var;
   struct variables* previous = NULL;
	
   if(head_var == NULL) {
      return NULL;
   }

   while(strcmp(current->data, key)) {

      if(current->next == NULL) {
         return NULL;
      } else {
         previous = current;
         current = current->next;
      }
   }

   if(current == head_var) {
      head_var = head_var->next;
   } else {
      previous->next = current->next;
   }    
	
   return current;
}

int get_position(char *key) {
  struct variables* current = head_var;
  int count = 0;

  while(strcmp(current->data, key) != 0) {
    if(current->next == NULL) {
         return 0;
      } else {
         current = current->next;
         count++;
      }
  }

  return count;
}

void insert_first_fce(char *data, d_list_types d_type, bool null_ret) {
   struct functions *link = (struct functions*) malloc(sizeof(struct functions));
	
   link->fce_name = data;
   link->return_type = d_type;
   link->can_be_null = null_ret;
	
   link->next = head_var_fce;
	
   head_var_fce = link;
}

struct functions* find_fce(char *key) {
   struct functions* current_fce = head_var_fce;

   if(head_var_fce == NULL) {
      return NULL;
   }

   while(strcmp(current_fce->fce_name, key)) {
	
      if(current_fce->next == NULL) {
         return NULL;
      } else {
         current_fce = current_fce->next;
      }
   }      
	
   return current_fce;
}

d_list_types token_to_d_type(int d_type) {
  switch (d_type) {
  case STRING:
    return D_STRING;
  case NUMBER:
    return D_NUM;
  case DECIMAL_NUMBER:
    return D_DECM_NUM;
  case EXPONENT_NUMBER:
    return D_EXP_NUMB;
  }
}

Token exp_sem_var(element *e, bool in_func) {
  struct variables* curr;
  struct functions* curr_fce;
  struct variables* tmp_var;
  d_list_types arg_type = D_NON;
  int brc_count;
  bool fce_call = false;
  int start_index = 0;
  int key_index = 0;
  Token t;

  if(e->argslist == NULL) {
    if(find(e->name.info) == NULL) {
      callError(ERR_SEM_VAR);
    }
    else {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = NULL_K;
      t.info = "null";
      return t;
    }
    
  }

  if(in_func == false && fce == true) {
    while (head_var->type != D_NON) {
      deleteFirst();
    }
    deleteFirst();
    fce = false;
  }

  for(int i = 1; i < e->argslist->len + 1; i++) {
    curr = find(e->name.info);
    if((e->argslist->list[i].arg.type == VAR_ID) && (fce_call == false)) {
      if(find(e->argslist->list[i].arg.info) == NULL) {
        callError(ERR_SEM_VAR);
      }
      else {
        if(in_func == true) {
          start_index = get_position("start");
          key_index = get_position(e->argslist->list[i].arg.info);
          if(start_index < key_index) {
            callError(ERR_SEM_VAR);
          }
        }
        tmp_var = find(e->argslist->list[i].arg.info); //najdem si typ premennej
        if(arg_type == D_NON || arg_type == D_NULL) {
          arg_type = tmp_var->type;
        }
        else {
          if(arg_type == D_STRING && arg_type != tmp_var->type) {
            if(tmp_var->type != D_NULL) {
              callError(ERR_SEM_COMPAT);
            }
            
          }
          else if(tmp_var->type == D_NUM || tmp_var->type == D_EXP_NUMB ||  tmp_var->type == D_DECM_NUM) {
            if((arg_type == D_NUM) && (tmp_var->type == D_EXP_NUMB || tmp_var->type == D_DECM_NUM)) {
              arg_type = D_DECM_NUM;
            }
        
          }
        }
      }
    } 
    else if(e->argslist->list[i].arg.type == STRING && fce_call == false) {
      if(arg_type == D_NON || arg_type == D_NULL) {
          arg_type = token_to_d_type(e->argslist->list[i].arg.type);
        }
        else {
          if(arg_type != token_to_d_type(e->argslist->list[i].arg.type) ) {
            callError(ERR_SEM_COMPAT);
          }
        }
    }
    else if((e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER) && (fce_call == false)) {
      if(arg_type == D_NON || arg_type == D_NULL) {
        arg_type = token_to_d_type(e->argslist->list[i].arg.type);
      }
      else if((arg_type == D_NUM) && (e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER)) {
        arg_type = D_DECM_NUM;
      }
        
    }
    else if((e->argslist->list[i].arg.type == DOT) && (arg_type == D_NUM || arg_type == D_EXP_NUMB || arg_type == D_DECM_NUM)) {
      callError(ERR_SEM_COMPAT);
    }
    else if((e->argslist->list[i].arg.type == PLUS || e->argslist->list[i].arg.type == MINUS || e->argslist->list[i].arg.type == MULTIPLY || e->argslist->list[i].arg.type == DIVIDE) && (arg_type == D_STRING)) {
      callError(ERR_SEM_COMPAT);
    }
    else if(e->argslist->list[i].arg.type == IDENTIFIER) {
      if(strcmp(e->argslist->list[i].arg.info, "reads") == 0) {
        arg_type = D_STRING;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "readi") == 0) {
        arg_type = D_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "readf") == 0) {
        arg_type = D_DECM_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "floatval") == 0) {
        arg_type = D_DECM_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "intval") == 0) {
        arg_type = D_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "strval") == 0) {
        arg_type = D_STRING;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "strlen") == 0) {
        arg_type = D_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "substring") == 0) {
        arg_type = D_STRING;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "ord") == 0) {
        arg_type = D_NUM;
        fce_call = true;
      }
      else if(strcmp(e->argslist->list[i].arg.info, "chr") == 0) {
        arg_type = D_STRING;
        fce_call = true;
      }
      else if(e->argslist->list[i].arg.kwt == NULL_K) {
        if(arg_type == D_NON) {
          arg_type = D_NULL;
        }
      }
      else {
        if(find_fce(e->argslist->list[i].arg.info) != NULL) {
          curr_fce = find_fce(e->argslist->list[i].arg.info);
          arg_type = curr_fce->return_type;
          fce_call = true;
        }
        else {
          int index = getListIndex();
          arg_type = find_in_list(&(*e->argslist->list[i].arg.info));
          changeTokenListIndex(index);

          if(arg_type == D_NON) {
            callError(ERR_SEM_FUNC);
          }
          fce_call = true;
        } 
      }
    }
    else if(e->argslist->list[i].arg.type == RIGHT_BRACKET) {
      if(fce_call == true && brc_count == 1) {
        fce_call = false;
      }
      brc_count--;
    }
    else if(e->argslist->list[i].arg.type == LEFT_BRACKET) {
      brc_count++;
    }
  }

  if(find(e->name.info) != NULL && in_func == false) {
    delete(e->name.info); //ak uz existuje zmazem a prepisem novym
    insert_first(e->name.info, arg_type);
  }
  else {
    insert_first(e->name.info, arg_type);
  }

  t.isKeyword = true;
  t.type = IDENTIFIER;
  t.kwt = d_type_to_kw(arg_type);
  t.info = d_type_to_info(arg_type);
  
  arg_type = D_NON;

  return t;
}

Token exp_sem_ifwhile(element *e, bool in_func) {
  struct variables* curr;
  struct functions* curr_fce;

  struct variables* tmp_var;
  d_list_types arg_type = D_NON;

  Token t;

  if(e->argslist == NULL) {
    if(find(e->name.info) == NULL) {
      callError(ERR_SEM_VAR);
    }
    else {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = NULL_K;
      t.info = "null";
      return t;
    }
    
  }
  
  for(int i = 0; i < e->argslist->len + 1; i++) {
    if(e->argslist->list[i].arg.type == VAR_ID) {
      if(find(e->argslist->list[i].arg.info) == NULL) {
        callError(ERR_SEM_VAR);
      }
    }
  }

  t.isKeyword = true;
  t.type = IDENTIFIER;
  t.kwt = INT_K;
  t.info = "int";

  return t;
}

Token exp_sem_return(element *e, bool in_func) {
  struct variables* curr;
  struct variables* tmp_var;
  d_list_types ret_t; 
  d_list_types arg_type = D_NON;
  Token t;

  if(e->argslist == NULL) {
    if(find(e->name.info) == NULL) {
      callError(ERR_SEM_VAR);
    }
    else {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = NULL_K;
      t.info = "null";
      return t;
    }
    
  }

  if(in_func == true) {
    if(head_var_fce->return_type == D_NULL && e->argslist->list[0].arg.type != SEMICOLON) {
      callError(ERR_SEM_RETURN );
    }
  }
  
  for(int i = 0; i < e->argslist->len; i++) {
    if(in_func == true) {
      if(e->argslist->list[i].arg.type == VAR_ID) {
        if(find(e->argslist->list[i].arg.info) == NULL) {
          callError(ERR_SEM_VAR);
        }
        else {
          curr = find(e->argslist->list[i].arg.info);
          if(head_var_fce->return_type == D_DECM_NUM || head_var_fce->return_type == D_NUM) {
            switch(curr->type) {
              case D_NUM:
                break;
              case D_DECM_NUM:
                break;
              case D_EXP_NUMB:
                break;
              case D_NULL:
                if(head_var_fce->can_be_null == false) {
                  callError(ERR_SEM_ARGS);
                }
                break;
              case D_STRING:
                  callError(ERR_SEM_ARGS);
              break;
            }
          }
          else if(head_var_fce->return_type == D_STRING) {
            switch(curr->type) {
              case D_STRING:
                break;
              case D_NULL:
                if(head_var_fce->can_be_null == false) {
                  callError(ERR_SEM_ARGS);
                }
                break;
              default:
                  callError(ERR_SEM_ARGS);
              break;
            }
          }
        }

      }
      else if(e->argslist->list[i].arg.type == STRING || e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER) {
        ret_t = token_to_d_type(e->argslist->list[i].arg.type);
        if(head_var_fce->return_type == D_DECM_NUM || head_var_fce->return_type == D_NUM) {
          switch(ret_t) {
            case D_NUM:
              break;
            case D_DECM_NUM:
              break;
            case D_EXP_NUMB:
              break;
            case D_STRING:
              callError(ERR_SEM_ARGS);
              break;
          }
        }
        else if(head_var_fce->return_type == D_STRING) {
           switch(ret_t) {
              case D_STRING:
                break;
              case D_NULL:
                if(head_var_fce->can_be_null == false) {
                  callError(ERR_SEM_ARGS);
                }
                break;
              default:
                  callError(ERR_SEM_ARGS);
              break;
            }
        }
      }
      else if((e->argslist->list[i].arg.type == DOT) && (head_var_fce->return_type == D_NUM || head_var_fce->return_type == D_EXP_NUMB || head_var_fce->return_type == D_DECM_NUM)) {
        callError(ERR_SEM_COMPAT);
      }
      else if((e->argslist->list[i].arg.type == PLUS || e->argslist->list[i].arg.type == MINUS || e->argslist->list[i].arg.type == MULTIPLY || e->argslist->list[i].arg.type == DIVIDE) && (head_var_fce->return_type == D_STRING)) {
        callError(ERR_SEM_COMPAT);
      }
      else if(e->argslist->list[i].arg.type == IDENTIFIER && e->argslist->len == 1) {
        ret_t = kw_to_d_type(e->argslist->list[i].arg.kwt);
        if(ret_t == D_NULL && head_var_fce->can_be_null != true) {
          callError(ERR_SEM_ARGS);
        }
      } 
    }

    else {
      if((e->argslist->list[i].arg.type == VAR_ID)) {
        if(find(e->argslist->list[i].arg.info) == NULL) {
          callError(ERR_SEM_VAR);
        }
        else {
          tmp_var = find(e->argslist->list[i].arg.info); //najdem si typ premennej
          if(arg_type == D_NON || arg_type == D_NULL) {
            arg_type = tmp_var->type;
          }
          else {
            if(arg_type == D_STRING && (arg_type != tmp_var->type)) {
              callError(ERR_SEM_COMPAT);
            }
            else if(tmp_var->type == D_NUM || tmp_var->type == D_EXP_NUMB ||  tmp_var->type == D_DECM_NUM) {
              if((arg_type == D_NUM) && (tmp_var->type == D_EXP_NUMB || tmp_var->type == D_DECM_NUM)) {
                arg_type = D_DECM_NUM;
              }
          
            }
          }
        }
      } 
      else if(e->argslist->list[i].arg.type == STRING) {
        if(arg_type == D_NON || arg_type == D_NULL) {
            arg_type = token_to_d_type(e->argslist->list[i].arg.type);
          }
          else {
            if(arg_type != token_to_d_type(e->argslist->list[i].arg.type) ) {
              callError(ERR_SEM_COMPAT);
            }
          }
      }
      else if((e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER)) {
        if(arg_type == D_NON || arg_type == D_NULL) {
          arg_type = token_to_d_type(e->argslist->list[i].arg.type);
        }
        else if((arg_type == D_NUM) && (e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER)) {
          arg_type = D_DECM_NUM;
        }
          
      }
      else if((e->argslist->list[i].arg.type == DOT) && (arg_type == D_NUM || arg_type == D_EXP_NUMB || arg_type == D_DECM_NUM)) {
        callError(ERR_SEM_COMPAT);
      }
      else if((e->argslist->list[i].arg.type == PLUS || e->argslist->list[i].arg.type == MINUS || e->argslist->list[i].arg.type == MULTIPLY || e->argslist->list[i].arg.type == DIVIDE) && (arg_type == D_STRING)) {
        callError(ERR_SEM_COMPAT);
      }
      else if(e->argslist->list[i].arg.type == IDENTIFIER) {
        if(e->argslist->list[i].arg.kwt == NULL_K) {
          if(arg_type == D_NON) {
            arg_type = D_NULL;
          }
        }
      }
    }
  }

  if(in_func == true) {
     if(e->argslist->list[0].arg.type == SEMICOLON) {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = NULL_K;
      t.info = "null";
    }
    else {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = d_type_to_kw(head_var_fce->return_type);
      t.info = d_type_to_info(head_var_fce->return_type);
    }
    
  }
  else {
    if(e->argslist->list[0].arg.type == SEMICOLON) {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = NULL_K;
      t.info = "null";
    }
    else {
      t.isKeyword = true;
      t.type = IDENTIFIER;
      t.kwt = INT_K;
      t.info = "int";
    }
  }

  return t;
}

void exp_sem_func(element *e) {
  d_list_types fce_param_type;
  fce = true;
  if (e->ret_type.kwt == VOID_K) {
    insert_first_fce(e->name.info, D_NULL, e->nullRet);
  }
  else {
    insert_first_fce(e->name.info, kw_to_d_type(e->ret_type.kwt), e->nullRet);
  }
  
  insert_first("start", D_NON);
  if(e->argslist != NULL) {
      for (int i = 0; i < e->argslist->len; i++) {
          if (e->argslist->list[i].type.isKeyword) {
              fce_param_type = kw_to_d_type(e->argslist->list[i].type.kwt);
          }
          insert_first(e->argslist->list[i].arg.info, fce_param_type);
      }
  }
}

d_list_types kw_to_d_type(int kw_type) {
  switch (kw_type) {
      case FLOAT_K:
        return D_DECM_NUM;
      case STRING_K:
        return D_STRING;
      case INT_K:
        return D_NUM;
      case NULL_K:
        return D_NULL;
      }
}

d_list_types find_in_list(char *fce_name) {
  Token t;
  d_list_types type;
  while((t=getTokenFromList()).type != EOF_T) {
    if(t.isKeyword && t.kwt == FUNCTION_K) {
      t = getTokenFromList();
      if(strcmp(t.info, fce_name) == 0) {
        while(t.type != DOUBLE_DOT) {
          t = getTokenFromList();
        }
        t = getTokenFromList();
        type = kw_to_d_type(t.kwt);
        return type;
        break;
      }
    }
  }
  return D_NON;
}

KeywordType d_type_to_kw(int d_type) {
  switch(d_type) {
    case D_NUM:
      return INT_K;
	  case D_DECM_NUM:
    case D_EXP_NUMB:
      return FLOAT_K; 
	  case D_STRING:
      return STRING_K;
	  case D_NON:
      return UNKNOWN_K;
    case D_NULL:
      return NULL_K;
  }
}

char *d_type_to_info(int d_type) {
  switch(d_type) {
    case D_NUM:
      return "int";
	  case D_DECM_NUM:
    case D_EXP_NUMB:
      return "float"; 
	  case D_STRING:
      return "string";
    case D_NULL:
      return "null";
  }
}