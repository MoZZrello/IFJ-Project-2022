#include "strings.h"
#include "errors.h"
#include "expr.h"

#define SIZE 17
bool rdc = false;
struct variables *head_var = NULL;
struct variables *current = NULL;
bool in_fce = false;
d_list_types return_type;

// TODO ZVRATENA TABUKA
int table [SIZE][SIZE] = {
				/* VALUE|PLUS|MINUS|MULTI|DIV|EQ|NEQ|GTHE|LTHE|GTH|LTH|COM|BRACER_L|BRACE_R|KONK|END*/
	/* VALUE*/	  {X, G, G, G, G, G, G, G, G, G, G, G, G, G, G, G},
	/* PLUS*/		  {L, E, E, E, E, G, G, G, G, G, G, G, L, G, L, G},
	/* MINUS*/	  {L, E, E, E, E, G, G, G, G, G, G, G, L, G, L, G},
	/* MULTI*/	  {L, E, E, E, E, G, G, G, G, G, G, G, L, G, G, G},
	/* DIV */		  {L, E, E, E, E, G, G, G, G, G, G, G, L, G, G, G},
	/* EQ */		  {L, L, L, L, L, G, G, L, L, L, L, G, L, G, L, G},
	/* NEQ */		  {L, L, L, L, L, G, G, L, L, L, L, G, L, G, L, G},
	/* GTHE */	  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* LTHE */	  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* GTH */		  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* LTH */		  {L, L, L, L, L, G, G, G, G, G, G, G, L, G, L, G},
	/* COM */		  {L, L, L, L, L, L, L, L, L, L, L, E, L, E, L, X},
	/* BRACE_L */	{L, L, L, L, L, L, L, L, L, L, L, E, L, E, L, X},
	/* BRACE_R */	{X, G, G, G, G, G, G, G, G, G, G, G, X, G, G, G},
	/* KONK */		{L, G, G, L, L, G, G, G, G, G, G, G, L, G, E, G},
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

        //printf("b je %d a je %d\n", symb_b, symb_a);

        if(symb_b > END) {
          pop(&stack);
          //printf("error\n");
          callError(ERR_SYN);
        }

        printf("vrateny znk z tabulkky je %d\n", table[symb_b][symb_a]);
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
  printstack(&stack); 
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
      //printf("%d top tmp\n", top(&tmp_stack));
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
            //printf("tu som v grt\n");
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
            push(curr_ptr, NONTERM, token2);
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

void printstack(struct stack_t *stack){
	expr_symb temp = top(stack);
	while(temp != ENDSTACK){
		switch(temp){
			case NONTERM:
				printf("E\n");
				break;
			case BRACE_L:
				printf("(\n");
				break;
			case L:
				printf("<\n");
				break;
			case MUL:
				printf("*\n");
				break;
			case BRACE_R:
				printf(")\n");
				break;
			case VALUE:
				printf("i\n");
				break;
			default:
				break;
			// case FUNC:
			// 	printf("FUNC\n");
			// 	break;
		}
		pop(stack);
		temp = top(stack);
	}
	printf("$\n");
}

/////////////////////////////////////////////////////////////////////////////
//semantika
void insert_first(char *data, d_list_types d_type) {
   //create a link
   struct variables *link = (struct variables*) malloc(sizeof(struct variables));
	
   //link->key = key;
   link->data = data;
   link->type = d_type;
	
   //point it to old first variables
   link->next = head_var;
	
   //point first to new first variables
   head_var = link;
}

//delete first item
struct variables* deleteFirst() {

   //save reference to first link
   struct variables *tempLink = head_var;
	
   //mark next to first link as first 
   head_var = head_var->next;
	
   //return the deleted link
   return tempLink;
}

//is list empty
bool isEmpty() {
   return head_var == NULL;
}

int length() {
   int length = 0;
   struct variables *current;
	
   for(current = head_var; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given key
struct variables* find(char *key) {
   //start from the first link
   struct variables* current = head_var;

   //if list is empty
   if(head_var == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->data, key)) {
	
      //if it is last variables
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct variables* delete(char *key) {

   //start from the first link
   struct variables* current = head_var;
   struct variables* previous = NULL;
	
   //if list is empty
   if(head_var == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->data, key)) {

      //if it is last variables
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head_var) {
      //change first to point to next link
      head_var = head_var->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

void printList() {
   struct variables *ptr = head_var;
	
   //start from the beginning
   while(ptr != NULL) {
      printf("v liste je %s %d\n", ptr->data, ptr->type);
      ptr = ptr->next;
   }
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
  //riesit tu aj error ak neexistujuci typ?
  //add null ako mozny typ
  }
}

void exp_sem_var(element *e) {
  //printf("dlzka je %d\n", e->argslist->len);
  struct variables* curr;
  struct variables* tmp_var;
  //ak je premenna uz deklarovana netreba ju znovu ukladat (zaial kym neriesim vysledok)
  if(find(e->name.info) == NULL) {
    insert_first(e->name.info, token_to_d_type(e->argslist->list[1].arg.type));
  }
  else {
    //skontrolovat ci nie je premennej priradeny iny typ nez predtym, ak ano, ulozim novy
    curr = find(e->name.info);
    //arglist vacsi ako 1 -> vtedy nic nemenim lebo vacsi vyraz -> kontrolujem dalej a vysledok ulozim neskor
    if(e->argslist->len == 1) {
      //kotrola ci idem priradzovat $y = $a alebo $y = "abcd"
      if(e->argslist->list[1].arg.type == VAR_ID) {
        if(find(e->argslist->list[1].arg.info) == NULL) {
          printf("error, nedeklarovana premenna\n");
        }
        else {
          tmp_var = find(e->argslist->list[1].arg.info);
          delete(e->name.info); //povodny zmazem
          insert_first(e->name.info, tmp_var->type); //ulozim novy
        }
      }

      if (curr->type != token_to_d_type(e->argslist->list[1].arg.type)) {
        delete(e->name.info); //povodny zmazem
        insert_first(e->name.info, token_to_d_type(e->argslist->list[1].arg.type)); //ulozim novy
      }
    }
  }
  
 // printList();
  //printf("stmt je %d\n", e->argslist->list[1].arg.type);

  for(int i = 1; i < e->argslist->len + 1; i++) {
    //printf("%s %s\n", e->name.info, e->argslist->list[i].arg.info);
    curr = find(e->name.info);
    //printf("info je %s %d\n", e->argslist->list[i].arg.info, e->argslist->list[i].arg.type);
    if(e->argslist->list[i].arg.type == VAR_ID) {
      if(find(e->argslist->list[i].arg.info) == NULL) {
        printf("error, nedeklarovana premenna\n");
      }
      else {
        tmp_var = find(e->argslist->list[i].arg.info);
        if(curr->type != tmp_var->type) {
          printf("error nekompatibilne datove typy\n");
        }
        
      }
    }
    else if(e->argslist->list[i].arg.type == STRING || e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER) {
      if (token_to_d_type(e->argslist->list[i].arg.type) != curr->type) {
        printf("error nekompatibilna hodnota s datovym typom\n");
      }
    }
    else if((e->argslist->list[i].arg.type == DOT) && (curr->type != D_STRING)) {
      printf("error, zle pouzitie operatora konk\n");
    }
    else if((e->argslist->list[i].arg.type == PLUS || e->argslist->list[i].arg.type == MINUS) && (curr->type == D_STRING)) {
      printf("error, zle pouzitie operatora plus, minus\n");
    }
    else if((e->argslist->list[i].arg.type == MULTIPLY || e->argslist->list[i].arg.type == DIVIDE) && (curr->type == D_STRING)) {
      //todo priradit a ocislovat prioritu, ze pojde prve
      printf("error, zle pouzitie operatora mul, div\n");
    }
    //todo
    //ak je < ==! == > atd =>error
    //zatvorky + ich priorita

  }
  //printf("%s %d\n", e->name.info, e->argslist->list->arg.type);

}

void exp_sem_ifwhile(element *e) {
  struct variables* curr;
  struct variables* tmp_var;
  
  for(int i = 0; i < e->argslist->len + 1; i++) {
    printf("%s %d\n", e->name.info, e->argslist->list[i].arg.type);
    if(e->argslist->list[i].arg.type == VAR_ID) {
      if(find(e->argslist->list[i].arg.info) == NULL) {
        printf("error, nedeklarovana premenna\n");
      }
      else {
        curr = find(e->argslist->list[i].arg.info);
      }
    }
    else if(e->argslist->list[i].arg.type == STRING || e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER) {
      if (token_to_d_type(e->argslist->list[i].arg.type) != curr->type) {
        printf("error nekompatibilna hodnota s datovym typom\n");
      }
    }
  }
}

void exp_sem_return(element *e) {
  struct variables* curr;
  for(int i = 0; i < e->argslist->len; i++) {
    printf("som tuuu %s %s\n", e->name.info, e->argslist->list[i].arg.info);
    //if(e->argslist->list[i].arg.type == VAR_ID) {
      /*if(find(e->argslist->list[i].arg.info) == NULL) {
        printf("error, nedeklarovana premenna pri return\n");
      }*/
      /*else {
        //skontrolujem ci vraciam dobry typ
         curr = find(e->argslist->list[i].arg.info);
         if(curr->type != return_type) {
          printf("error, zly navratovy typ\n");
         }

      }*/
    /*}
    else if(e->argslist->list[i].arg.type == STRING || e->argslist->list[i].arg.type == NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER || e->argslist->list[i].arg.type == DECIMAL_NUMBER) {
      if (token_to_d_type(e->argslist->list[i].arg.type) != return_type) {
        printf("error nekompatibilna hodnota s datovym typom v return\n");
      }
    }*/
  }
}

/*void  expr_sem_identif(element *e) {
  printf("tu som\n");

}*/

void exp_sem_func(element *e) {
  in_fce = true;
  d_list_types fce_param_type;
  return_type = kw_to_d_type(e->ret_type.kwt);
  for(int i = 0; i < e->argslist->len; i++) {
    printf("%d %d\n", e->argslist->list[i].arg.type, e->argslist->list[i].type.type);
    if(e->argslist->list[i].type.isKeyword) {
      fce_param_type = kw_to_d_type(e->argslist->list[i].type.kwt);
    }
    insert_first(e->argslist->list[i].arg.info, fce_param_type);
  }
  printList();
}

d_list_types kw_to_d_type(int kw_type) {
  switch (kw_type) {
      case FLOAT_K:
        return D_DECM_NUM;
      case STRING_K:
        return D_STRING;
      case INT_K:
        return D_NUM;
      }
}

