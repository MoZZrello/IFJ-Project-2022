#include "parser.h"

string str;
Token token;
bool sem_find = false;
bool brac_find = false;
bool literal = false;
bool ret = false;
int brac_count = 0;

void prolog(Token *t) {
    initTokenList();
    token = *t;
    //printf("%s\n", getTypeName(*token));
    //printf("%s\n", token->info);
    //todo inicializovat hashtable

    //inicializacia stacku ?? pre expr
    
    //TODO strict types vymenit!!!!!!!!
    if (strcmp("|PHP|", getTypeName(token)) != 0) {
        callError(ERR_SYN);
    }
    else {
        token = getToken(str);
        if(strcmp(token.info, "declare") != 0) {
            callError(ERR_SYN);
        }
        else {
            token = getToken(str);
            if (strcmp(token.info, "(") != 0) {
                callError(ERR_SYN);
            }
            else {
                token = getToken(str);
                if(strcmp(token.info, "strict_types") != 0) {
                    callError(ERR_SYN);
                }
                else {
                    token = getToken(str);
                    if (strcmp(token.info, "=") != 0) {
                        callError(ERR_SYN);
                    }
                    else {
                        token = getToken(str);
                        if (strcmp(token.info, "1") != 0) {
                            callError(ERR_SYN);
                        }
                        else {
                            token = getToken(str);
                            if (strcmp(token.info, ")") != 0) {
                                callError(ERR_SYN);
                            }
                            else {
                                token = getToken(str);
                                if(strcmp(token.info, ";") != 0) {
                                    callError(ERR_SYN);
                                }
                                else {
                                    //printf("toto je %s %s\n",token.info, getTypeName(token));
                                    body();
                                    /*free(token->info);
                                    //free(token->type);
                                    free(token);*/
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    emptyTokenList();
}


//todo vymysliet ako end poslat do funkcie
void body() {
    token = getToken(str);
    addTokenToList(token);
    //printf("%s %ds %s\n", token.info, token.type, getTypeName(token));

    if(!strcmp( getTypeName(token), "|IDENTIFIER|")){
        stmt();
    } else if (!strcmp( getTypeName(token), "|VAR_ID|")) {
       stmt();
    } else if (!strcmp( getTypeName(token), "|PHP END|")) {
        printf("end\n");
        return;
    } else if (!strcmp( getTypeName(token), "|EOF|")) {
        printf("end\n");
        end_file = true;
        return;
    } else {
        callError(ERR_SYN);
    }

    body();
}

void stmt() {
    if (!strcmp(token.info, "function")) {
        func();
    } else if (!strcmp(token.info, "if")) {
        token = getToken(str);
        addTokenToList(token);
        if (!strcmp(token.info, "(")) { 
            brac_find = true;  
            expr_skip();
            brac_find = false;
            token = getToken(str);
            addTokenToList(token);
            if (!strcmp(token.info, "{")) {
                brac_count++;
                
                stmt_list();
                
                //token = getToken(str);
                if (!strcmp(token.info, "}")) {
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

    } else if (!strcmp(token.info, "while")) {
        token = getToken(str);
        addTokenToList(token);
        if (!strcmp(token.info, "(")) { 
            brac_find = true;  
            expr_skip();
            brac_find = false;
            token = getToken(str);
            addTokenToList(token);
            if (!strcmp(token.info, "{")) {
                brac_count++;
                stmt_list();
                //token = getToken(str);
                if (strcmp(token.info, "}")) {
                    printf("error while }\n");
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
    }

    else if(!strcmp(getTypeName(token), "|IDENTIFIER|")) { //todo urobit expression 
        if (!strcmp(token.info, "return")) {
            sem_find = true;
            expr_skip();
            sem_find = false;
            printf("mame return\n");
        } else {
            printf("mame volanie funkcie\n");
            //zatial skip parametre funkcie
            token = getToken(str);
            addTokenToList(token);
            if(!strcmp(token.info, "(")) {
                sem_find = true;
                expr_skip();
                sem_find = false;
            }
        }
    } else if (!strcmp(getTypeName(token), "|VAR_ID|")) { //todo urobit premenne  //tu sme skoncili
        token = getToken(str);
        addTokenToList(token);
        if (strcmp(token.info, "=") != 0) {
            printf("error =\n");
        } else {
            sem_find = true;
            expr_skip();
            sem_find = false;
            printf("var ok\n");
        }
    } else {
        printf("error stmt\n");
        callError(ERR_SYN);
    }
}


void end() {
    //todo kontrolovat eol/eof/?> alebo error
}  

//vyresit epsilon
void stmt_list() {
    printf("%s %s v stmt\n", token.info, getTypeName(token));
    token = getToken(str);
    addTokenToList(token);
    if(strcmp(token.info, "}") != 0) {
        stmt();
        stmt_list();
    } else {
        brac_find--;
    }
    

}

//todo ked else je epsilon
void else_stmt() {
    token = getToken(str);
    addTokenToList(token);
    
    if(!strcmp(token.info, "else")) {
        token = getToken(str);
        addTokenToList(token);
        if(!strcmp(token.info, "{")) {
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
    token = getToken(str);
    addTokenToList(token);
    if(strcmp(getTypeName(token), "|IDENTIFIER|") != 0) {  //identifier nemoze byt zakazany nazov
        printf("chyba func id\n");
        callError(ERR_SYN);
    } else {
        token = getToken(str);
        addTokenToList(token);
        if(strcmp(token.info, "(") != 0) {
            printf("funkcia ( error\n");
            callError(ERR_SYN);
        } else {
            /*brac_find = true;
            expr_skip();
            brac_find = false;*/
            args();

            ret_type();
            if (ret) {
               token = getToken(str);
               addTokenToList(token);
            }
            if(strcmp(token.info, "{") != 0) {
                printf("error v fun {\n");
                callError(ERR_SYN);
            } else {
                brac_count++;
                stmt_list();
                //printf("%s %s pred\n", token.info, getTypeName(token));
                //token = getToken(str);
                /*if(brac_count != 0) {
                    token = getToken(str);
                }*/
                printf("%s %s v func\n", token.info, getTypeName(token));
                if(strcmp(token.info, "}") != 0) {
                    printf("error func }\n");
                    callError(ERR_SYN);
                }
                else {
                    brac_count--;
                    printf("func je okej\n");
                    callError(ERR_SYN);
                }
            }

        }
    }

}

//todo vyriesit ked neobsauje parametre
void args() {
    token = getToken(str);
    addTokenToList(token);
    if (!strcmp(token.info, ")")){
        printf("dalej nic nie je\n");
    } else if (!strcmp(getTypeName(token), "|IDENTIFIER|")) {
        data_type();
        token = getToken(str);
        addTokenToList(token);
        if (!strcmp(getTypeName(token), "|VAR_ID|")) {
            arg_def();
             
            if(literal) {
                token = getToken(str);
                addTokenToList(token);
            }
            arg_list();
        }
    }
}

//todo arg_def = literal?
void arg_def() {
    token = getToken(str);
    addTokenToList(token);
    if(!strcmp(token.info, "=")) {
        literal = true;
        token = getToken(str);
        addTokenToList(token);
        if(!strcmp(getTypeName(token), "|STRING|")) {
            return;
        } else if (!strcmp(getTypeName(token), "|NUMBER|")) {
            return;
        } else {
            printf("eror arg def\n");
            callError(ERR_SYN);
        }
    }
    //printf("%s %s\n",token.info ,getTypeName(token));
}

void arg_list() {
    if(!strcmp(token.info, ",")) {
        token = getToken(str);
        addTokenToList(token);
        data_type();
        token = getToken(str);
        addTokenToList(token);
        if(!strcmp(getTypeName(token), "|VAR_ID|")) {
            token = getToken(str);
            addTokenToList(token);
            arg_list();
        }
    } else if(!strcmp(token.info, ")")) {
        printf("dalej nic nie je\n");
    } else {
        printf("error arg list\n");
        callError(ERR_SYN);
    }
}

void ret_type() {
    token = getToken(str);
    addTokenToList(token);
    if(!strcmp(token.info,":")) {
        ret = true;
        token = getToken(str);
        addTokenToList(token);
        if(!strcmp(getTypeName(token), "|IDENTIFIER|")) {
            data_type();
        }
       
    }
}

//todo urobit ked su pred tym typom otazniky
//moze byt navratovy typ funkcie null?
void data_type() {
    if(!strcmp(token.info, "void")) {
        printf("mame void\n");
    } else if(!strcmp(token.info, "int")) {
        printf("mame int\n");
    } else if(!strcmp(token.info, "float")) {
        printf("mame float\n");
    } else if(!strcmp(token.info, "string")) {
        printf("mame string\n");
    } else {
        printf("Datovy typ error\n");
        callError(ERR_SYN);
    }
}

void expr_skip() {
    printf("tuuu %s %s %d\n", token.info, getTypeName(token), token.type); 
    token = getToken(str);
    addTokenToList(token);
    if (sem_find) {
        /*while (strcmp(token.info, ";") != 0) {
            //expression(getTypeName(token));
            //expression(token.type);
            token = getToken(str);
            
        }*/
    } else if (brac_find) {
       /*while (strcmp(token.info, ")") != 0) {
            expression(getTypeName(token));
            token = getToken(str);
        } */
        //while a if - vraciam ked koniec zatorky
        expression(&token);
    }
}