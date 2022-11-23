#include "parser.h"

Token token;
bool sem_find = false;
bool brac_find = false;
bool literal = false;
bool ret = false;
int brac_count = 0;

void prolog() {
    token = getTokenFromList();
    //inicializacia stacku ?? pre expr
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
    printTokenList();
}


//todo vymysliet ako end poslat do funkcie
void body() {
    token = getTokenFromList();
    //printf("%s %ds %s\n", token.info, token.type, getTypeName(token));

    if(strcmp(getTypeName(token), "|IDENTIFIER|") == 0){
        stmt();
    } else if (strcmp( getTypeName(token), "|VAR_ID|") == 0){
       stmt();
    } else if (strcmp( getTypeName(token), "|PHP END|") == 0){
        printf("end\n");
        return;
    } else if (strcmp( getTypeName(token), "|EOF|") == 0) {
        printf("end\n");
        end_file = true;
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
        if (strcmp(token.info, "(") == 0) {
            brac_find = true;  
            expr_skip();
            brac_find = false;
            token = getTokenFromList();
            if (strcmp(token.info, "{") == 0) {
                brac_count++;
                
                stmt_list();
                
                //token = getToken(str);
                if (strcmp(token.info, "}") == 0) {
                    //printf("%s %s sme pred else\n", token.info,getTypeName(token));
                    //return;
                    brac_count--;
                    else_stmt();
                } else {
                    printf("error if }\n");
                    callError(ERR_SYN);
                }
            } else {
                printf("error if {\n");
                callError(ERR_SYN);
            }
        } else{
            printf("error if\n");
            callError(ERR_SYN);
        }

    } else if (strcmp(token.info, "while") == 0) {
        token = getTokenFromList();
        if (strcmp(token.info, "(") == 0) {
            brac_find = true;  
            expr_skip();
            brac_find = false;
            token = getTokenFromList();
            if (strcmp(token.info, "{") == 0) {
                brac_count++;
                stmt_list();
                //token = getToken(str);
                if (strcmp(token.info, "}") != 0) {
                    printf("error while }\n");
                    callError(ERR_SYN);
                } else {
                    brac_find--;
                    printf("presli sme cez while\n");
                }
            } else {
                printf("error while {\n");
                callError(ERR_SYN);
            }
        } else {
            printf("error while\n");
            callError(ERR_SYN);
        }  
    } else if(strcmp(getTypeName(token), "|IDENTIFIER|") == 0) { //todo urobit expression
        if (strcmp(token.info, "return") == 0) {
            sem_find = true;
            expr_skip();
            sem_find = false;
            printf("mame return\n");
        } else {
            printf("mame volanie funkcie\n");
            //zatial skip parametre funkcie
            token = getTokenFromList();
            if(strcmp(token.info, "(") == 0) {
                sem_find = true;
                expr_skip();
                sem_find = false;
            }
        }
    } else if (strcmp(getTypeName(token), "|VAR_ID|") == 0) { //todo urobit premenne  //tu sme skoncili
        token = getTokenFromList();
        if (strcmp(token.info, "=") != 0) {
            printf("error =\n");
            callError(ERR_SYN);
        } else {
            sem_find = true;
            expr_skip();
            sem_find = false;
            printf("var ok\n");
        }
    } else {
        emptyTokenList();
        printf("error stmt %s\n", getTypeName(token));
        callError(ERR_SYN);
    }
}


void end() {
    //todo kontrolovat eol/eof/?> alebo error
}  

//vyresit epsilon
void stmt_list() {
    printf("%s %s v stmt\n", token.info, getTypeName(token));
    token = getTokenFromList();
    if(strcmp(token.info, "}") != 0) {
        stmt();
        stmt_list();
    } else {
        brac_find--;
    }
    

}

//todo ked else je epsilon
void else_stmt() {
    token = getTokenFromList();
    
    if(strcmp(token.info, "else") == 0) {
        token = getTokenFromList();
        if(strcmp(token.info, "{") == 0) {
            brac_count++;
            stmt_list();
            //token = getToken(str);
            if (strcmp(token.info, "}") != 0) {
                printf("error else }\n");
                callError(ERR_SYN);
            }
            else {
                brac_count--;
                printf("else okej\n");   
            }

        }
    }
}

void func() {
    token = getTokenFromList();
    if(strcmp(getTypeName(token), "|IDENTIFIER|") != 0) {  //identifier nemoze byt zakazany nazov
        printf("chyba func id\n");
        callError(ERR_SYN);
    } else {
        token = getTokenFromList();
        if(strcmp(token.info, "(") != 0) {
            printf("funkcia ( error\n");
            callError(ERR_SYN);
        } else {
            args();
            ret_type();
            if (ret) {
               token = getTokenFromList();
            }
            if(strcmp(token.info, "{") != 0) {
                printf("error v fun {\n");
                callError(ERR_SYN);
            } else {
                brac_count++;
                stmt_list();
                printf("%s %s v func\n", token.info, getTypeName(token));
                if(strcmp(token.info, "}") != 0) {
                    printf("error func }\n");
                    callError(ERR_SYN);
                }
                else {
                    brac_count--;
                    printf("func je okej\n");
                }
            }

        }
    }

}

//todo vyriesit ked neobsauje parametre
void args() {
    token = getTokenFromList();
    if (strcmp(token.info, ")") == 0){
        printf("dalej nic nie je\n");
    } else if (strcmp(getTypeName(token), "|IDENTIFIER|") == 0) {
        data_type();
        token = getTokenFromList();
        if (!strcmp(getTypeName(token), "|VAR_ID|")) {
            arg_def();
             
            if(literal) {
                token = getTokenFromList();
            }
            arg_list();
        }
    }
}

//todo arg_def = literal?
void arg_def() {
    token = getTokenFromList();
    if(strcmp(token.info, "=") == 0) {
        literal = true;
        token = getTokenFromList();
        if(strcmp(getTypeName(token), "|STRING|") == 0){
            return;
        } else if (strcmp(getTypeName(token), "|NUMBER|") == 0) {
            return;
        } else {
            printf("eror arg def\n");
            callError(ERR_SYN);
        }
    }
    //printf("%s %s\n",token.info ,getTypeName(token));
}

void arg_list() {
    if(strcmp(token.info, ",") == 0) {
        token = getTokenFromList();
        data_type();
        token = getTokenFromList();
        if(strcmp(getTypeName(token), "|VAR_ID|") == 0) {
            token = getTokenFromList();
            arg_list();
        }
    } else if(strcmp(token.info, ")") == 0) {
        printf("dalej nic nie je\n");
    } else {
        printf("error arg list\n");
        callError(ERR_SYN);
    }
}

void ret_type() {
    token = getTokenFromList();
    if(strcmp(token.info,":") == 0) {
        ret = true;
        token = getTokenFromList();
        if(strcmp(getTypeName(token), "|IDENTIFIER|") == 0) {
            data_type();
        }
       
    }
}

//todo urobit ked su pred tym typom otazniky
//moze byt navratovy typ funkcie null?
void data_type() {
    if(strcmp(token.info, "void") == 0) {
        printf("mame void\n");
    } else if(strcmp(token.info, "int") == 0) {
        printf("mame int\n");
    } else if(strcmp(token.info, "float") == 0) {
        printf("mame float\n");
    } else if(strcmp(token.info, "string") == 0) {
        printf("mame string\n");
    } else {
        printf("Datovy typ error\n");
        callError(ERR_SYN);
    }
}

void expr_skip() {
    printf("tuuu %s %s %d\n", token.info, getTypeName(token), token.type);
    token = getTokenFromList();
    if (sem_find) {
        while (strcmp(token.info, ";") != 0) {
            //expression(getTypeName(token));
            //expression(token.type);
            //token = getTokenFromList();
        }
    } else if (brac_find) {
      while (strcmp(token.info, ")") != 0) {
            //expression(getTypeName(token));
            //token = getTokenFromList();
            expression(&token);
        } 
        //while a if - vraciam ked koniec zatorky
        
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

    free(key);
}

ht_table_t* sortSem(ht_table_t *table, int *retKey){
    Token t;
    int index = 8, key = *retKey; char func[MAX_HT_SIZE];
    changeTokenListIndex(index);
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
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_func();
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case IF_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_if_while();
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case RETURN_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_return();
                    ht_insert(table, func, &data[key]);
                    key++;
                    break;
                case WHILE_K:
                    data = realloc(data, sizeof(element)*(key+1));
                    sprintf( func,"%d", key);
                    data[key] = sem_if_while();
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
            data[key] = sem_var();
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
    return e;
}

element sem_return(){
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
    return e;
}

element sem_if_while(){
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

element sem_var(){
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
            e.argslist->list = realloc(e.argslist->list, sizeof(arg) * (argsCount + 1));
            e.argslist->list[argsCount].arg = t;
            e.argslist->len = argsCount+1;
            argsCount++;
        }
    }
    return e;
}

void semControl(ht_table_t *table, int key){
    progdata data;
    char funcChar[50] = "reads;readi;readf;write;strlen;substring;ord;cbr;";

    data.returned = false;
    data.inIF = false;
    data.inFunction = false;
    data.inElse = false;
    data.inWhile = false;
    data.lastFuncKey = 0;
    data.funcCounter = 0;
    data.varCounter = 0;
    data.definedFunctions = malloc(sizeof(char) * (strlen(funcChar)+1));
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
            strcat(data.definedVars, e->name.info);
            strcat(data.definedVars, ";");

        } else if(e->name.kwt == RETURN_K){ // if it's return
            check_sem_return(*ht_get(table, lastFuncIndex), *e);
            data.returned = true;

        } else if (e->ret_type.type != ERROR_T){ // if it's function
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
                data.inIF = true;
            } else if(strcmp(e->name.info, "while") == 0){
                data.inWhile = true;
            } else if(strcmp(e->name.info, "else") == 0){
                data.inElse = true;
            } else { // function calls
                see_call_defined(table, *e);
            }
        } else if(e->name.type == RIGHT_CURLY_BRACKET){
            if (data.inFunction){
                if(data.inIF){
                    data.inIF = false;
                    continue;
                } else if (data.inWhile){
                    data.inWhile = false;
                } else if (data.inElse){
                    data.inElse = false;
                } else {
                    if(data.returned){
                        data.returned = false;
                        data.inFunction = false;
                    } else if((*ht_get(table, lastFuncIndex)).ret_type.kwt != VOID_K){
                        callError(ERR_SEM_RETURN);
                    } else {
                        data.inFunction = false;
                    }
                }
            } else if (data.inIF){
                data.inIF = false;
            } else if (data.inWhile){
                data.inWhile = false;
            } else if (data.inElse){
                data.inElse = false;
            } else {
                callError(ERR_SEM_RETURN);
            }
        } else {
            continue;
        }
    }
    free(data.definedFunctions);
    free(data.definedVars);
}

void check_sem_return(element func_e, element ret_e){
    if(func_e.ret_type.kwt == STRING_K && ret_e.argslist->list[0].arg.type == STRING ||
       func_e.ret_type.kwt == VOID_K && ret_e.argslist->list[0].arg.type == SEMICOLON){
        return;
    } else if (func_e.ret_type.kwt == INT_K &&
                (ret_e.argslist->list[0].arg.type == NUMBER || ret_e.argslist->list[0].arg.type == EXPONENT_NUMBER)){
        if(strchr(ret_e.argslist->list->arg.info, '.') == NULL){
            return;
        } else {
            callError(ERR_SEM_ARGS);
        }
    } else if (func_e.ret_type.kwt == FLOAT_K &&
                (ret_e.argslist->list[0].arg.type == DECIMAL_NUMBER || ret_e.argslist->list[0].arg.type == EXPONENT_NUMBER)){
        if(strchr(ret_e.argslist->list->arg.info, '.') != NULL){
            return;
        } else {
            callError(ERR_SEM_ARGS);
        }
    } else {
        callError(ERR_SEM_ARGS);
    }
}

void check_defined_functions(progdata data, char* name){
    if(strstr(data.definedFunctions, name) != NULL){
        callError(ERR_SEM_FUNC);
    }
}

void see_call_defined(ht_table_t *table, element call){
    int i = 0;
    bool defined = false;
    char index[MAX_HT_SIZE];
    element* compare_e = NULL;
    element* definition_e = NULL;

    while(!0){
        sprintf(index, "%d", i++);
        compare_e = ht_get(table, index);
        if(compare_e == NULL) break;

        if (compare_e->ret_type.type != ERROR_T){
            if(strcmp(compare_e->name.info, call.name.info) == 0){
                definition_e = compare_e;
                defined = true;
            }
        }
    }
    if(defined){
        see_call_arguments(*definition_e, call);
    } else {
        // undefined function called
        callError(ERR_SEM_FUNC);
    }
}

void see_call_arguments(element func, element call){
    char funcChar[50] = "reads;readi;readf;write;strlen;substring;ord;cbr;";
    if(strstr(funcChar, call.name.info) == NULL){ // defined in program
        if(func.argslist->len != call.argslist->len){
            callError(ERR_SEM_ARGS);
        }

        int i = 0;
        while(i < func.argslist->len){
            if((func.argslist->list[i].type.kwt == INT_K && call.argslist->list[i].arg.type == NUMBER) ||
               (func.argslist->list[i].type.kwt == FLOAT_K && call.argslist->list[i].arg.type == DECIMAL_NUMBER) ||
               (func.argslist->list[i].type.kwt == STRING_K && call.argslist->list[i].arg.type == STRING)){
                i++;
                continue;
            } else {
                callError(ERR_SEM_ARGS);
            }
            i++;
        }
    } else { //predefined function

    }
    //printf("%s|%s\n", func.name.info, call.name.info);
}