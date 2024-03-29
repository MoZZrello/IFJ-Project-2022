/*
 * Created by Terezia Hundakova, Jasmína Csalova and Richard Harman on 16. 11. 2022
 */

#include "parser.h"

Token token;
bool literal = false;
bool ret = false;
int brac_count = 0;

void prolog() {
    token = getTokenFromList();
    
    if (strcmp("|PHP|", getTypeName(token)) != 0) {
        callError(ERR_SYN);
    }
    else {
        token = getTokenFromList();
        if(strcmp(token.info, "declare") != 0) {
            callError(ERR_SYN);
        }
        else {
            token = getTokenFromList();
            if (strcmp(token.info, "(") != 0) {
                callError(ERR_SYN);
            }
            else {
                token = getTokenFromList();
                if(strcmp(token.info, "strict_types") != 0) {
                    callError(ERR_SYN);
                }
                else {
                    token = getTokenFromList();
                    if (strcmp(token.info, "=") != 0) {
                        callError(ERR_SYN);
                    }
                    else {
                        token = getTokenFromList();
                        if (strcmp(token.info, "1") != 0) {
                            callError(ERR_SYN);
                        }
                        else {
                            token = getTokenFromList();
                            if (strcmp(token.info, ")") != 0) {
                                callError(ERR_SYN);
                            }
                            else {
                                token = getTokenFromList();
                                if(strcmp(token.info, ";") != 0) {
                                    callError(ERR_SYN);
                                }
                                else {
                                    body();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void body() {
    token = getTokenFromList();

    if(token.type == IDENTIFIER){
        stmt();
    } else if (token.type == VAR_ID){
       stmt();
    } else if(token.type == NUMBER || token.type == STRING || token.type == DECIMAL_NUMBER || token.type == EXPONENT_NUMBER) {
        expression(&token, true);
        body();
    } else if(token.type == LEFT_BRACKET) {
        expression(&token, true);
        body();
    }else if (token.type == PHP_END){
        return;
    } else if (token.type == EOF_T) {
        return;
    } else {
        callError(ERR_SYN);
    }

    body();
}

void stmt() {
    if (strcmp(token.info, "function") == 0) {
        func();
    } else if (strcmp(token.info, "if") == 0) {
        token = getTokenFromList();
        if (token.type == LEFT_BRACKET) {
            expression(&token, false);
            token = getTokenFromList();
            if (token.type == LEFT_CURLY_BRACKET) {
                brac_count++;     
                stmt_list();
                if (token.type == RIGHT_CURLY_BRACKET) {
                    brac_count--;
                    else_stmt();
                } else {
                    callError(ERR_SYN);
                }
            } else {
                callError(ERR_SYN);
            }
        } else{
            callError(ERR_SYN);
        }

    } else if (strcmp(token.info, "while") == 0) {
        token = getTokenFromList();
        if (token.type == LEFT_BRACKET) {
            expression(&token, false);
            token = getTokenFromList();
            if (token.type == LEFT_CURLY_BRACKET) {
                brac_count++;
                stmt_list();
                if (token.type != RIGHT_CURLY_BRACKET) {
                    callError(ERR_SYN);
                }
            } else {
                callError(ERR_SYN);
            }
        } else {
            callError(ERR_SYN);
        }  
    } else if(token.type == IDENTIFIER) {
        if (strcmp(token.info, "return") == 0) {
            token = getTokenFromList();
            expression(&token, true);
        } else {
            token = getTokenFromList();
            if(token.type == LEFT_BRACKET) {
                expression(&token, true);
            }
        }
    } else if (strcmp(getTypeName(token), "|VAR_ID|") == 0) {
        token = getTokenFromList();
        if (token.type != ASSIGN) {
            expression(&token, true);
            if(token.type != SEMICOLON) {
                callError(ERR_SYN);
            }
        } else {
            token = getTokenFromList();
            expression(&token, true);
        }
    } else if(token.type == NUMBER || token.type == STRING || token.type == DECIMAL_NUMBER || token.type == EXPONENT_NUMBER) {
        expression(&token, true);
        if(token.type != SEMICOLON) {
            callError(ERR_SYN);
        }
    } else {
        emptyTokenList();
        callError(ERR_SYN);
    }
}

void stmt_list() {
    token = getTokenFromList();
    if(token.type != RIGHT_CURLY_BRACKET) {
        stmt();
        stmt_list();
    }
}

void else_stmt() {
    token = getTokenFromList();
    
    if(strcmp(token.info, "else") == 0) {
        token = getTokenFromList();
        if(token.type == LEFT_CURLY_BRACKET) {
            brac_count++;
            stmt_list();
            if (token.type != RIGHT_CURLY_BRACKET) {
                callError(ERR_SYN);
            }
            else {
                brac_count--;
            }
        }
    }
}

void func() {
    token = getTokenFromList();
    if(token.type != IDENTIFIER || token.isKeyword == true) {
        callError(ERR_SYN);
    } else {
        token = getTokenFromList();
        if(token.type != LEFT_BRACKET) {
            callError(ERR_SYN);
        } else {
            args();
            ret_type();
            if (ret) {
               token = getTokenFromList();
            }
            if(token.type != LEFT_CURLY_BRACKET) {
                callError(ERR_SYN);
            } else {
                brac_count++;
                stmt_list();
                if(token.type != RIGHT_CURLY_BRACKET) {
                    callError(ERR_SYN);
                }
                else {
                    brac_count--;
                }
            }

        }
    }

}

void args() {
    token = getTokenFromList();
    if (token.type == RIGHT_BRACKET){
    } else if (token.type == IDENTIFIER) {
        data_type();
        token = getTokenFromList();
        if (token.type == VAR_ID) {
            arg_def();
             
            if(literal) {
                token = getTokenFromList();
            }
            arg_list();
        }
    }
}

void arg_def() {
    token = getTokenFromList();
    if(token.type == ASSIGN) {
        literal = true;
        token = getTokenFromList();
        if(token.type == STRING){
            return;
        } else if (token.type == NUMBER) {
            return;
        } else {
            callError(ERR_SYN);
        }
    }
}

void arg_list() {
    if(token.type == COMMA) {
        token = getTokenFromList();
        data_type();
        token = getTokenFromList();
        if(token.type == VAR_ID) {
            token = getTokenFromList();
            arg_list();
        }
    } else if(token.type == RIGHT_BRACKET) {
        return;
    } else {
        callError(ERR_SYN);
    }
}

void ret_type() {
    token = getTokenFromList();
    if(token.type == DOUBLE_DOT) {
        ret = true;
        token = getTokenFromList();
        if(strcmp(getTypeName(token), "|IDENTIFIER|") == 0) {
            data_type();
        }   
    }
}

void data_type() {
    if(strcmp(token.info, "void") == 0) {
        return;
    } else if(strcmp(token.info, "int") == 0) {
        return;
    } else if(strcmp(token.info, "float") == 0) {
        return;
    } else if(strcmp(token.info, "string") == 0) {
        return;
    } else if(strcmp(token.info, "?int") == 0) {
        return;
    } else if(strcmp(token.info, "?float") == 0) {
        return;
    } else if(strcmp(token.info, "?string") == 0) {
        return;
    } else {
        callError(ERR_SYN);
    }
}

void antilog(ht_table_t *table){
    int *key = malloc(sizeof(int));
    //check malloc
    if(key == NULL){
        callError(ERR_INTERNAL);
    }
    key[0] = 0;

    sortSem(table, key);
    int freeEnd = *key;
    addBuiltInFuncs(table, key);

    semControl(table, freeEnd);
    gen_program(table, freeEnd);
    free(key);
}

ht_table_t* sortSem(ht_table_t *table, int *retKey){
    Token t;
    int index = 8, key = *retKey; char func[MAX_HT_SIZE], leftCount = 0;
    changeTokenListIndex(index);
    bool inFunction = false;
    element* data;
    data = malloc(sizeof(element));
    //check malloc
    if(data == NULL){
        callError(ERR_INTERNAL);
    }

    while((t=getTokenFromList()).type != EOF_T){
        if(t.isKeyword){
            switch(t.kwt){
                case ELSE_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_else();
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case FUNCTION_K:
                    inFunction = true;
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_func(&table);
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case IF_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_if_while(inFunction);
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case RETURN_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_return(inFunction);
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case WHILE_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_if_while(inFunction);
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case NULL_K:
                case FLOAT_K:
                case STRING_K:
                case VOID_K:
                case INT_K:
                case UNKNOWN_K:
                    continue;
            }
        } else if(t.type == VAR_ID){
            data = realloc(data, sizeof(element)*(key+1));
            sprintf( func,"%d", key);
            data[key] = sem_var(inFunction);
            ht_insert(table, func, &data[key]);
            key++;
            previousTokenListIndex();
        } else if(t.type == IDENTIFIER){
            data = realloc(data, sizeof(element)*(key+1));
            sprintf( func,"%d", key);
            data[key] = sem_identif();
            ht_insert(table, func, &data[key]);
            key++;
        } else if(t.type != SEMICOLON){
            if(inFunction){
                if(t.type == LEFT_CURLY_BRACKET){
                    leftCount++;
                } else if(t.type == RIGHT_CURLY_BRACKET){
                    leftCount--;
                }
                if(leftCount == 0){ // FUNCTION END
                    inFunction = false;
                }
            }
            data = realloc(data, sizeof(element)*(key+1));
            sprintf( func,"%d", key);
            data[key] = sem_else();
            ht_insert(table, func, &data[key]);
            key++;
        } else {
            continue;
        }
    }

    free(data);
    retKey[0] = key;
    return table;
}

ht_table_t* addBuiltInFuncs(ht_table_t *table, int *retKey){
    Token name_t, rettype_t;
    int key = *retKey;
    char func[MAX_HT_SIZE];

    name_t = (Token){.info="reads", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="?string", .type=IDENTIFIER, .isKeyword=true, .kwt=STRING_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=true});

    name_t = (Token){.info="readi", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="?int", .type=IDENTIFIER, .isKeyword=true, .kwt=INT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=true});

    name_t = (Token){.info="readf", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="?float", .type=IDENTIFIER, .isKeyword=true, .kwt=FLOAT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=true});

    name_t = (Token){.info="write", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="void", .type=IDENTIFIER, .isKeyword=true, .kwt=VOID_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    name_t = (Token){.info="floatval", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="float", .type=IDENTIFIER, .isKeyword=true, .kwt=FLOAT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    name_t = (Token){.info="intval", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="int", .type=IDENTIFIER, .isKeyword=true, .kwt=INT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    name_t = (Token){.info="strval", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="string", .type=IDENTIFIER, .isKeyword=true, .kwt=STRING_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    name_t = (Token){.info="strlen", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="int", .type=IDENTIFIER, .isKeyword=true, .kwt=INT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    name_t = (Token){.info="substring", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="?string", .type=IDENTIFIER, .isKeyword=true, .kwt=STRING_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=true});

    name_t = (Token){.info="ord", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="int", .type=IDENTIFIER, .isKeyword=true, .kwt=INT_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});


    name_t = (Token){.info="chr", .type=IDENTIFIER, .isKeyword=false};
    rettype_t = (Token){.info="string", .type=IDENTIFIER, .isKeyword=true, .kwt=STRING_K};
    sprintf(func, "%d", key++);
    ht_insert(table, func, &(element){.name=name_t, .ret_type=rettype_t, .argslist=NULL, .nullRet=false});

    retKey[0] = key;
    return table;
}

element sem_func(){
    int argsCount = 0;
    Token t = getTokenFromList();
    element e;
    e.name = t;
    e.argslist = malloc(sizeof(argList));
    //check malloc
    if(e.argslist == NULL){
        callError(ERR_INTERNAL);
    }
    e.argslist->list = malloc(sizeof(arg));
    //check malloc
    if(e.argslist->list == NULL){
        callError(ERR_INTERNAL);
    }

    e.argslist->len = 0;
    while((t=getTokenFromList()).type != RIGHT_BRACKET){
        if(t.type == COMMA || t.type == LEFT_BRACKET){
            continue;
        } else {
            e.argslist->list = realloc(e.argslist->list, sizeof(arg)*(argsCount+1));
            e.argslist->list[argsCount].type = t;
            t = getTokenFromList();
            e.argslist->list[argsCount].arg = t;
            e.argslist->len++;
        }
        argsCount++;
    }
    t = getTokenFromList();
    if(t.type == DOUBLE_DOT){
        t = getTokenFromList();
        e.ret_type = t;
        if(t.info[0] == '?'){
            e.nullRet = true;
        } else {
            e.nullRet = false;
        }
    } else {
        e.nullRet = false;
        e.ret_type = (Token){.info="void", .type=IDENTIFIER, .isKeyword=true, .kwt=VOID_K};
        previousTokenListIndex();
    }
    exp_sem_func(&e);
    check_args_name(e);
    return e;
}

element sem_return(bool in_fce){
    previousTokenListIndex();
    Token t = getTokenFromList();
    int argsCount = 0;
    element e;
    e.name = t;
    e.argslist = NULL;
    e.ret_type = getEmptyToken();
    t = getTokenFromList();
    if(t.type != SEMICOLON){
        e.argslist = malloc(sizeof(argList));
        //check malloc
        if(e.argslist == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->list = malloc(sizeof(arg));
        //check malloc
        if(e.argslist->list == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->len = 0;
        e.argslist->list = realloc(e.argslist->list, sizeof(arg)*(argsCount+1));
        e.argslist->list[argsCount].arg = t;
        e.argslist->len++;
        argsCount++;
        while((t=getTokenFromList()).type != SEMICOLON){
            e.argslist->list = realloc(e.argslist->list, sizeof(arg)*(argsCount+1));
            e.argslist->list[argsCount].arg = t;
            e.argslist->len++;
            argsCount++;
        }
    } else {
        e.argslist = malloc(sizeof(argList));
        //check malloc
        if(e.argslist == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->list = malloc(sizeof(arg));
        //check malloc
        if(e.argslist->list == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->len = 0;
        e.argslist->list = realloc(e.argslist->list, sizeof(arg)*(argsCount+1));
        e.argslist->list[argsCount].arg = t;
        e.argslist->len++;
    }
    e.expr = exp_sem_return(&e, in_fce); 
    
    return e;
}

element sem_if_while(bool in_fce){
    previousTokenListIndex();
    Token t = getTokenFromList();
    element e;
    e.name = t;
    e.argslist = NULL;
    e.ret_type = getEmptyToken();
    int argsCount = 0;
    t = getTokenFromList();
    t = getTokenFromList();
    if(t.type != RIGHT_BRACKET) {
        e.argslist = malloc(sizeof(argList));
        //check malloc
        if(e.argslist == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->list = malloc(sizeof(arg));
        //check malloc
        if(e.argslist->list == NULL){
            callError(ERR_INTERNAL);
        }
        previousTokenListIndex();
        while ((t = getTokenFromList()).type != RIGHT_BRACKET) {
            e.argslist->list = realloc(e.argslist->list, sizeof(arg) * (argsCount + 1));
            e.argslist->list[argsCount].arg = t;
            e.argslist->len = argsCount;
            argsCount++;
        }
    }
    e.expr = exp_sem_ifwhile(&e, in_fce); 
    return e;
}

element sem_else(){
    previousTokenListIndex();
    Token t = getTokenFromList();
    element e;
    e.name = t;
    e.argslist = NULL;
    e.ret_type = getEmptyToken();
    return e;
}

element sem_var(bool in_fce){
    previousTokenListIndex();
    Token t = getTokenFromList();
    element e;
    e.name = t;
    e.argslist = NULL;
    e.ret_type = getEmptyToken();
    int argsCount = 0;
    t = getTokenFromList();
    if(t.type == ASSIGN){
        e.argslist = malloc(sizeof(argList));
        //check malloc
        if(e.argslist == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->list = malloc(sizeof(arg));
        //check malloc
        if(e.argslist->list == NULL){
            callError(ERR_INTERNAL);
        }
        previousTokenListIndex();
        while ((t = getTokenFromList()).type != SEMICOLON) {
            e.argslist->list = realloc(e.argslist->list, sizeof(arg) * (argsCount + 1));
            e.argslist->list[argsCount].arg = t;
            e.argslist->len = argsCount;
            argsCount++;           
        }
    }
    e.expr = exp_sem_var(&e, in_fce);  
    return e;
}

element sem_identif(){
    previousTokenListIndex();
    Token t = getTokenFromList();
    element e;
    e.name = t;
    e.argslist = NULL;
    e.ret_type = getEmptyToken();
    int argsCount = 0;
    t = getTokenFromList();
    t = getTokenFromList();
    if(t.type != RIGHT_BRACKET){
        e.argslist = malloc(sizeof(argList));
        //check malloc
        if(e.argslist == NULL){
            callError(ERR_INTERNAL);
        }
        e.argslist->list = malloc(sizeof(arg));
        //check malloc
        if(e.argslist->list == NULL){
            callError(ERR_INTERNAL);
        }
        previousTokenListIndex();
        while ((t = getTokenFromList()).type != RIGHT_BRACKET) {
            if(t.type != COMMA){
                e.argslist->list = realloc(e.argslist->list, sizeof(arg) * (argsCount + 1));
                e.argslist->list[argsCount].arg = t;
                e.argslist->len = argsCount+1;
                argsCount++;
            }
        }
    }
    return e;
}

void check_args_name(element e){
    if(e.argslist == NULL){
        return;
    }

    for(int i = 0;i < e.argslist->len; i++){
        int j = e.argslist->len-1;
        while(j != 0){
            if(i != j){
                if(strcmp(e.argslist->list[i].arg.info, e.argslist->list[j].arg.info) == 0) {
                    callError(ERR_SEM_OTHER);
                }
            }
            j--;
        }
    }
}

void semControl(ht_table_t *table, int key){
    progdata data;
    char funcChar[75] = "reads;readi;readf;write;floatval;intval;strval;strlen;substring;ord;chr;";
    bool functions_defined = false;

    data.returned = false;
    data.inIF = 0;
    data.inFunction = false;
    data.inElse = 0;
    data.inWhile = 0;
    data.lastFuncKey = 0;
    data.funcCounter = 0;
    data.varCounter = 0;
    data.definedFunctions = malloc(sizeof(char) * (strlen(funcChar)+1));
    lastCalled *lc;
    int lcIndex = 0;
    lc = malloc(sizeof(lastCalled) * (lcIndex+1));
    if(data.definedFunctions == NULL){
        callError(ERR_INTERNAL);
    }
    strcpy(data.definedFunctions, funcChar);
    data.definedVars = malloc(sizeof(char)*2);
    if(data.definedVars == NULL){
        callError(ERR_INTERNAL);
    }
    strcpy(data.definedVars, "");

    char index[MAX_HT_SIZE], lastFuncIndex[MAX_HT_SIZE];
    element* e = NULL;

    for(int i=0; i < key; i++){
        sprintf(index, "%d", i);
        sprintf(lastFuncIndex, "%d", data.lastFuncKey);
        e = ht_get(table, index);
        if(e->name.type == VAR_ID){ // if it's variable
            data.varCounter++;
            int len = (int)strlen(data.definedVars) + (int)strlen(e->name.info) + data.varCounter + 1;

            data.definedVars = realloc(data.definedVars, sizeof(char) * len);
            if(data.definedVars == NULL){
                callError(ERR_INTERNAL);
            }

            if(e->argslist != NULL && e->argslist->list[1].arg.type == IDENTIFIER){
                if(strcmp(e->argslist->list[1].arg.info, "null") != 0){
                    check_var_calls(table, *e, i);
                }
            }

            if(e->argslist != NULL && e->argslist->list[0].arg.type == SEMICOLON){
                get_variable(table, e, e->name ,i);
            }

            strcat(data.definedVars, e->name.info);
            strcat(data.definedVars, ";");

        } else if(e->name.kwt == RETURN_K){ // if it's return
            if(data.inFunction == false){
                check_global_return(*e);
            } else {
                check_sem_return(*ht_get(table, lastFuncIndex), *e);
            }
            data.returned = true;

        } else if (e->ret_type.type != ERROR_T){ // if it's function definition
            functions_defined = true;
            data.funcCounter++;
            // Syntax error if function definition isn't in main body of program
            if(data.inElse || data.inWhile || data.inIF){
                callError(ERR_SYN);
            }

            int len = (int)strlen(data.definedFunctions) + (int)strlen(e->name.info) + data.funcCounter + 1;

            data.lastFuncKey = i;
            data.inFunction = true;

            check_defined_functions(data, e->name.info);

            data.definedFunctions = realloc(data.definedFunctions, sizeof(char) * len);
            if(data.definedFunctions == NULL){
                callError(ERR_INTERNAL);
            }
            strcat(data.definedFunctions, e->name.info);
            strcat(data.definedFunctions, ";");

        } else if(e->name.type == IDENTIFIER){ // if it's function call
            if(strcmp(e->name.info, "if") == 0){
                lc = realloc(lc, sizeof(lastCalled) * (lcIndex+2));
                lc[lcIndex++] = IF;
                data.inIF++;
            } else if(strcmp(e->name.info, "while") == 0){
                lc = realloc(lc, sizeof(lastCalled) * (lcIndex+2));
                lc[lcIndex++] = WHILE;
                data.inWhile++;
            } else if(strcmp(e->name.info, "else") == 0){
                lc = realloc(lc, sizeof(lastCalled) * (lcIndex+2));
                lc[lcIndex++] = ELSE;
                data.inElse++;
            } else if(strcmp(e->name.info, "null") == 0){
                continue;
            } else { // function calls
                see_call_defined(table, *e, i);
            }
        } else if(e->name.type == RIGHT_CURLY_BRACKET){
            if (data.inFunction){
                if(data.inIF > 0 && lc[lcIndex-1] == IF){
                    data.inIF--;
                    lcIndex--;
                    continue;
                } else if (data.inWhile > 0 && lc[lcIndex-1] == WHILE){
                    data.inWhile--;
                    lcIndex--;
                    continue;
                } else if (data.inElse > 0 && lc[lcIndex-1] == ELSE){
                    data.inElse--;
                    lcIndex--;
                    continue;
                } else {
                    if(data.returned){
                        data.returned = false;
                        data.inFunction = false;
                    } else if((*ht_get(table, lastFuncIndex)).ret_type.kwt != VOID_K){
                        callError(ERR_SEM_ARGS);
                    } else {
                        data.inFunction = false;
                    }
                }
            } else if (data.inIF > 0 && lc[lcIndex-1] == IF){
                data.inIF--;
                lcIndex--;
            } else if (data.inWhile > 0 && lc[lcIndex-1] == WHILE){
                data.inWhile--;
                lcIndex--;
            } else if (data.inElse > 0 && lc[lcIndex-1] == ELSE){
                data.inElse--;
                lcIndex--;
            } else {
                callError(ERR_SEM_RETURN);
            }
        } else {
            continue;
        }
    }
    free(data.definedFunctions);
    free(data.definedVars);
    if(lc != NULL){
        free(lc);
    }
}

void check_var_calls(ht_table_t *table, element e, int key){
    int argsCount = 0;
    element call;
    call.name = e.argslist->list[1].arg;
    call.argslist = NULL;
    call.ret_type = getEmptyToken();

    if(e.argslist->len > 3) {
        call.argslist = malloc(sizeof(argList));
        if (call.argslist == NULL) {
            callError(ERR_INTERNAL);
        }

        call.argslist->list = malloc(sizeof(arg));
        if (call.argslist->list == NULL) {
            callError(ERR_INTERNAL);
        }

        call.argslist->len = 0;

        for (int i = 3; i < e.argslist->len; i++) {
            call.argslist->list = realloc(call.argslist->list, sizeof(arg) * (argsCount + 1));
            call.argslist->list[argsCount].arg = e.argslist->list[i].arg;
            call.argslist->len++;
            argsCount++;
        }
    }

    see_call_defined(table, call, key);

    if(call.argslist != NULL){
        free(call.argslist->list);
        free(call.argslist);
    }
}

void check_sem_return(element func_e, element ret_e){
    if((func_e.ret_type.kwt == STRING_K && ret_e.expr.kwt == STRING_K) ||
       (func_e.ret_type.kwt == VOID_K && ret_e.expr.kwt == NULL_K) ||
       (func_e.ret_type.canBeNull && ret_e.expr.kwt == NULL_K)||
       (func_e.ret_type.canBeNull && ret_e.argslist->len == 1 && ret_e.argslist->list[0].arg.kwt == NULL_K)){
        return;
    } else if (func_e.ret_type.kwt == INT_K && ret_e.expr.kwt == INT_K){
        if(strchr(ret_e.argslist->list->arg.info, '.') == NULL){
            return;
        } else {
            callError(ERR_SEM_ARGS);
        }
    } else if (func_e.ret_type.kwt == FLOAT_K && ret_e.expr.kwt == FLOAT_K){
        if(strchr(ret_e.argslist->list->arg.info, '.') != NULL){
            return;
        } else {
            callError(ERR_SEM_ARGS);
        }
    } else {
        if(ret_e.argslist != NULL && func_e.ret_type.kwt != VOID_K){
            int partCount = 0;
            for(int i = 0; i < ret_e.argslist->len;i++){
                if(ret_e.argslist->list[i].arg.type == SEMICOLON){
                    break;
                }
                if(i == 0){
                    partCount++;
                }
                if(ret_e.argslist->list[i].arg.type == COMMA){
                    partCount++;
                }
            }
            if(partCount != 1){
                callError(ERR_SEM_RETURN);
            }
        } else if(ret_e.argslist != NULL && func_e.ret_type.kwt == VOID_K){
            int partCount = 0;
            for(int i = 0; i < ret_e.argslist->len;i++){
                if(ret_e.argslist->list[i].arg.type == SEMICOLON){
                    break;
                }
                if(i == 0){
                    partCount++;
                }
                if(ret_e.argslist->list[i].arg.type == COMMA){
                    partCount++;
                }
            }
            if(partCount != 0){
                callError(ERR_SEM_RETURN);
            }
        }
        callError(ERR_SEM_ARGS);
    }
}

void check_global_return(element ret_e){
    if(ret_e.argslist != NULL || (ret_e.argslist == NULL || ret_e.argslist->len == 1)){
        if(ret_e.expr.isKeyword && (ret_e.expr.kwt == INT_K || ret_e.expr.kwt == STRING_K || ret_e.expr.kwt == FLOAT_K || ret_e.expr.kwt == NULL_K)){
            return;
        } else {
            callError(ERR_SEM_RETURN);
        }
    } else {
        callError(ERR_SEM_RETURN);
    }
}

void check_defined_functions(progdata data, char* name){
    char* one_func;
    one_func = malloc(sizeof(char) * (int) strlen(data.definedFunctions));
    bool new = true;

    for(int i = 0; i < strlen(data.definedFunctions); i++){
        int one_func_index = 0;
        while(data.definedFunctions[i] != ';'){
            one_func[one_func_index++] = data.definedFunctions[i];
            i++;
        }
        one_func[one_func_index] = '\0';
        if(strcmp(one_func, name) == 0){
            new = false;
        }
    }
    free(one_func);
    if(new == false){
        callError(ERR_SEM_FUNC);
    }
}

void see_call_defined(ht_table_t *table, element call, int key){
    int i = 0;
    bool defined = false;
    char index[MAX_HT_SIZE];
    element* compare_e = NULL;
    element* definition_e = NULL;

    while(!0){
        sprintf(index, "%d", i);
        compare_e = ht_get(table, index);
        if(compare_e == NULL) break;

        if (compare_e->ret_type.type != ERROR_T){
            if(strcmp(compare_e->name.info, call.name.info) == 0){
                definition_e = compare_e;
                defined = true;
            }
        }
        i++;
    }
    if(defined){
        see_call_arguments(table, *definition_e, call, key);
    } else {
        // undefined function called
        callError(ERR_SEM_FUNC);
    }
}

void see_call_arguments(ht_table_t *table, element func, element call, int key){
    char funcChar[75] = "reads;readi;readf;write;floatval;intval;strval;strlen;substring;ord;chr;";
    char* one_func;
    one_func = malloc(sizeof(char) * (int) strlen(funcChar));
    bool new = true;

    for(int i = 0; i < strlen(funcChar); i++){
        int one_func_index = 0;
        while(funcChar[i] != ';'){
            one_func[one_func_index++] = funcChar[i];
            i++;
        }
        one_func[one_func_index] = '\0';
        if(strcmp(one_func, call.name.info) == 0){
            new = false;
        }
    }
    free(one_func);

    if(call.argslist == NULL){
        if(func.argslist == NULL || func.argslist->len == 0){
            return;
        } else {
            callError(ERR_SEM_ARGS);
        }
    }

    if(new == true){ // defined in program
        if(func.argslist->len != call.argslist->len){
            int count = 0;
            for(int i = 0; i < call.argslist->len; i++){
                if(call.argslist->list[i].arg.type != COMMA){
                    count++;
                }
            }
            if(count != func.argslist->len){
                callError(ERR_SEM_ARGS);
            }
        }

        int i = 0;
        int commaCount = 0;
        while(i < func.argslist->len){
            if(call.argslist->list[i+commaCount].arg.type == COMMA){
                commaCount++;
            }
            if(call.argslist->list[i+commaCount].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[i+commaCount].arg, key);
                if(var.isKeyword == false || (func.argslist->list[i].type.kwt != var.kwt)){
                    callError(ERR_SEM_ARGS);
                }
                i++;
                continue;
            }
            if((func.argslist->list[i].type.kwt == INT_K && call.argslist->list[i+commaCount].arg.type == NUMBER) ||
               (func.argslist->list[i].type.kwt == FLOAT_K &&
                    (call.argslist->list[i+commaCount].arg.type == DECIMAL_NUMBER ||
                     call.argslist->list[i+commaCount].arg.type == EXPONENT_NUMBER)) ||
               (func.argslist->list[i].type.kwt == STRING_K && call.argslist->list[i+commaCount].arg.type == STRING) ||
               (func.argslist->list[i].type.canBeNull && call.argslist->list[i+commaCount].arg.kwt == NULL_K)){
                i++;
                continue;
            } else {
                callError(ERR_SEM_ARGS);
            }
            i++;
        }
    } else { //predefined function
        if(strcmp(call.name.info, "reads") == 0 ||
           strcmp(call.name.info, "readi") == 0 ||
           strcmp(call.name.info, "readf") == 0){
            if(call.argslist != NULL){
                callError(ERR_SEM_ARGS);
            }
        } else if(strcmp(call.name.info, "strlen") == 0 ||
                  strcmp(call.name.info, "ord") == 0){
            if(call.argslist == NULL){
                callError(ERR_SEM_ARGS);
            }
            if(call.argslist->list[0].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[0].arg, key);
                if(var.isKeyword == false && var.kwt != STRING_K){
                    callError(ERR_SEM_ARGS);
                }
            } else if(call.argslist->len != 1 ||
               call.argslist->list[0].arg.type != STRING){
                callError(ERR_SEM_ARGS);
            }

        } else if(strcmp(call.name.info, "floatval") == 0 ||
                strcmp(call.name.info, "intval") == 0){
            if(call.argslist == NULL){
                callError(ERR_SEM_ARGS);
            }
            if(call.argslist->list[0].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[0].arg, key);
                if(var.isKeyword == false || (var.kwt != INT_K && var.kwt != FLOAT_K && var.kwt != NULL_K)){
                    callError(ERR_SEM_ARGS);
                }
            } else if(call.argslist->len != 1 ||
                (call.argslist->list[0].arg.type != NUMBER &&
                call.argslist->list[0].arg.type != EXPONENT_NUMBER &&
                call.argslist->list[0].arg.type != DECIMAL_NUMBER &&
                call.argslist->list[0].arg.kwt != NULL_K)){
                    callError(ERR_SEM_ARGS);
            }
        } else if(strcmp(call.name.info, "strval") == 0){
            if(call.argslist == NULL){
                callError(ERR_SEM_ARGS);
            }
            if(call.argslist->list[0].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[0].arg, key);
                if(var.isKeyword == false || (var.kwt != STRING_K && var.kwt != NULL_K)){
                    callError(ERR_SEM_ARGS);
                }
            } else if(call.argslist->len != 1 ||
                    (call.argslist->list[0].arg.type != STRING &&
                    call.argslist->list[0].arg.kwt != NULL_K)){
                callError(ERR_SEM_ARGS);
            }
        } else if(strcmp(call.name.info, "substring") == 0){
            bool first = false, second = false, third = false;
            if(call.argslist == NULL){
                callError(ERR_SEM_ARGS);
            }
            if(call.argslist->list[0].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[0].arg, key);
                if(var.isKeyword == false || var.kwt != STRING_K){
                    callError(ERR_SEM_ARGS);
                }
                first = true;
            }
            if(call.argslist->list[2].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[2].arg, key);
                if(var.isKeyword == false || var.kwt != INT_K){
                    callError(ERR_SEM_ARGS);
                }
                second = true;
            }
            if(call.argslist->list[4].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[4].arg, key);
                if(var.isKeyword == false || var.kwt != INT_K){
                    callError(ERR_SEM_ARGS);
                }
                third = true;
            }
            if(call.argslist->len != 5 ||
                     ((call.argslist->list[0].arg.type != STRING  && first == false) ||
                     (call.argslist->list[2].arg.type != NUMBER && second == false ) ||
                     (call.argslist->list[4].arg.type != NUMBER && third == false))){
                callError(ERR_SEM_ARGS);
            }
        } else if(strcmp(call.name.info, "chr") == 0){
            if(call.argslist == NULL){
                callError(ERR_SEM_ARGS);
            }if(call.argslist->list[0].arg.type == VAR_ID){
                Token var = get_variable(table, &call, call.argslist->list[0].arg, key);
                if(var.isKeyword == false || var.kwt != INT_K){
                    callError(ERR_SEM_ARGS);
                }
            } else if(call.argslist->len != 1 ||
               call.argslist->list[0].arg.type != NUMBER){
                callError(ERR_SEM_ARGS);
            }
        } else if(strcmp(call.name.info, "write") == 0){
            if(call.argslist != NULL){
                for(int i = 0; i < call.argslist->len; i++){
                    if(call.argslist->list[i].arg.type == VAR_ID){
                        get_variable(table, &call, call.argslist->list[i].arg, key);
                    }
                }
            }
        }
    }
}

Token get_variable(ht_table_t *table, element* e, Token var, int key){
    int i = 0, curly = 0;
    bool inFunction = false;
    char index[MAX_HT_SIZE];
    element* compare_e = NULL;
    element* definition_e = NULL;
    Token func = getEmptyToken();
    Token main = getEmptyToken();

    while(!0){
        sprintf(index, "%d", i++);
        compare_e = ht_get(table, index);
        if((i-1) == key) break;

        if (compare_e->ret_type.type != ERROR_T){
            inFunction = true;
            if(compare_e->argslist != NULL){
                for(int j = 0; j < compare_e->argslist->len; j++){
                    if(strcmp(compare_e->argslist->list[j].arg.info, var.info) == 0){
                        func = compare_e->argslist->list[j].type;
                    }
                }
            }
        } else if (compare_e->name.type == VAR_ID){
            if(strcmp(compare_e->name.info, var.info) == 0){
                if(inFunction) {
                    func = compare_e->expr;
                } else {
                    main = compare_e->expr;
                }
            }
        } else if (compare_e->name.type == LEFT_CURLY_BRACKET){
            curly++;
        } else if (compare_e->name.type == RIGHT_CURLY_BRACKET){
            curly--;
            if(curly == 0){
                inFunction = false;
                func = getEmptyToken();
            }
        }
    }
    if(inFunction){
        if(func.type == ERROR_T){
            callError(ERR_SEM_VAR);
        }
        return func;
    } else {
        if(main.type == ERROR_T){
            callError(ERR_SEM_VAR);
        }
        return main;
    }
}