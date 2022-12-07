//
// Created by Marek Spirka on 27. 11. 2022.
//
#include "generator.h"

int counter = 0;
char cycle_name[30] = "$while_cycle_";
char cycle_end[30] = "$while_end_";
char while_main[20] = "LF@WHILE_STMT\0";
char if_main[11] = "LF@IF_STMT\0";
char else_label[20] = "$else_";

/**
 *@brief functions that print commands in the IFJcode22 language
 *@param name -> command name
 *@param arg1 -> first arg of command
 *@param arg2 -> first arg of command
 *@param arg3 -> first arg of command
 */
void PRINT_LANE_THREE_ARG(char* name, char* arg1, char* arg2, char* arg3) {
    printf("%s %s %s %s \n", name, arg1, arg2, arg3);
}
void PRINT_LANE_TWO_ARG(char* name, char* arg1, char* arg2) {
    printf("%s %s %s \n", name, arg1, arg2);
}
void PRINT_LANE_ONE_ARG(char* name, char* arg1) {
    printf("%s %s \n", name, arg1);
}
void PRINT_LANE_ZERO_ARG(char* name) {
    printf("%s \n", name);
}

/**
 *@brief The function calls the functions that compose the entire IFJcode22 code
 *@param table -> place where the entire code from input is stored
 *@param no_build_in_func -> ID of element in Table
 */
void gen_program(ht_table_t *table, int no_build_in_func){
    counter = 0;
    start_program();
    gen_built_in_functions(table, no_build_in_func);
    function_gen(table);
    gen_main(table, no_build_in_func);
}

/**
 *@brief The function prints the program header and a reference to main
 */
void start_program(){
    PRINT_LANE_ZERO_ARG(".IFJcode22");
    PRINT_LANE_ONE_ARG("JUMP", "$main");
}

/**
 *@brief When a built-in function is called, the function is printed at the beginning of the program,
 *each function is called only once
 *@param table -> place where the entire code from input is stored
 *@param no_build_in_func -> ID of element in Table
 */
void gen_built_in_functions(ht_table_t *table, int key){
    bool reads = true, readi = true, readf = true, write = true, floatval = true, intval = true, strval = true;
    bool strlen = true, substring = true, ord = true, chr = true;
    char index[MAX_HT_SIZE];

    for(int i=0; i < key; i++){
        element *e = NULL;
        sprintf(index,
                "%d", i);
        e = ht_get(table, index);
        if(e->name.type == IDENTIFIER) {
            if(e->argslist != NULL) {
                if(strcmp(e->name.info, "write") == 0){
                    if(write){
                        func_write();
                        write = false;
                    }
                }else if(strcmp(e->name.info, "floatval") == 0) {
                    if (floatval) {
                        func_floatval();
                        floatval = false;
                    }
                }else if(strcmp(e->name.info, "intval") == 0) {
                    if (intval) {
                        func_intval();
                        intval = false;
                    }
                }else if(strcmp(e->name.info, "strval") == 0) {
                    if (strval) {
                        func_strval();
                        strval = false;
                    }
                }else if(strcmp(e->name.info, "strlen") == 0) {
                    if (strlen) {
                        func_strlen();
                        strlen = false;
                    }
                }else if(strcmp(e->name.info, "substring") == 0) {
                    if (substring) {
                        func_substring();
                        substring = false;
                    }
                }else if(strcmp(e->name.info, "ord") == 0) {
                    if (ord) {
                        func_ord();
                        ord = false;
                    }
                }else if(strcmp(e->name.info, "chr") == 0) {
                    if (chr) {
                        func_chr();
                        chr = false;
                    }
                }
            }else{
                if (strcmp(e->name.info, "reads") == 0) {
                    if (reads) {
                        func_reads();
                        reads = false;
                    }
                } else if (strcmp(e->name.info, "readi") == 0) {
                    if (readi) {
                        func_readi();
                        readi = false;
                    }
                } else if (strcmp(e->name.info, "readf") == 0) {
                    if (readf) {
                        func_readf();
                        readf = false;
                    }
                }
            }
        } else if(e->name.type == VAR_ID && e->argslist != NULL){
            if(strcmp(e->argslist->list[1].arg.info, "readi") == 0){
                if(readi) {
                    func_readi();
                    readi = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "reads") == 0){
                if(reads) {
                    func_reads();
                    reads = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "readf") == 0){
                if(readf) {
                    func_readf();
                    readf = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "write") == 0){
                if(write) {
                    func_write();
                    write = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "floatval") == 0){
                if(floatval) {
                    func_floatval();
                    floatval = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "intval") == 0){
                if(intval) {
                    func_intval();
                    intval = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "strval") == 0){
                if(strval) {
                    func_strval();
                    strval = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "strlen") == 0){
                if(strlen) {
                    func_strlen();
                    strlen = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "substring") == 0){
                if(substring) {
                    func_substring();
                    substring = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "ord") == 0){
                if(ord) {
                    func_ord();
                    ord = false;
                }
            } else if(strcmp(e->argslist->list[1].arg.info, "chr") == 0){
                if(chr) {
                    func_chr();
                    chr = false;
                }
            }
        }
    }
}

//---------------------------------------FUNCTIONS-------------------------------------------------//
/**
 *@brief A function that prints an user-defined function
 *@param ht_table_t *table -> pointer to place where the entire code from input is stored
 */
void function_gen(ht_table_t *table){
    bool is_function = false; // zistenie ci mame funkciu ak ano printime ak nie nic sa nestane,
    bool is_end = false; //is_end ak sa dostaneme na koniec funkcie prestaneme print == }
    bool inElse = false, inWhile = false;
    element* e = NULL;
    char func[MAX_HT_SIZE];
    char else_end_label[20] = "$else_end_\0";
    RetType ret_type;
    int i = 0, elseCurly = 0, curly = 0, whileCurly = 0;

    while(!0){
        sprintf(func, "%d", i++);
        e = ht_get(table, func);
        if(e == NULL)break;

        if(e->ret_type.type != ERROR_T){
            ret_type = def_func_start(e, table);
            func_arg_print(e);//print arg of functions
            is_function = true;
            is_end = false;
            continue;
        } else if (e->name.type == LEFT_CURLY_BRACKET){
            if(is_function){
                curly++;
            }
            if(inElse){
                elseCurly++;
            }
            if(inWhile){
                whileCurly++;
            }
            continue;
            //telo funkcie skoncilo }
        } else if (e->name.type == RIGHT_CURLY_BRACKET){
            if(inElse) {
                elseCurly--;
                if (elseCurly == 0) {
                    PRINT_LANE_ONE_ARG("LABEL", else_end_label);
                    inElse = false;
                }
            }
            if(inWhile){
                whileCurly--;
                if(whileCurly == 0){
                    inWhile = false;
                    char tmp[MAX_HT_SIZE];
                    PRINT_LANE_ONE_ARG("JUMP", cycle_name);
                    PRINT_LANE_ONE_ARG("LABEL", cycle_end);
                }
            }
            if(is_function){
                curly--;
                if(curly == 0){
                    is_end = true;
                    is_function = false;
                }
            }

        } else if(e->name.kwt == ELSE_K && is_function){
            inElse = true;
            char tmp[HT_SIZE];
            sprintf(tmp, "%d", counter++);
            else_end_label[10] = '\0';
            strcat(else_end_label, tmp);
            PRINT_LANE_ONE_ARG("JUMP", else_end_label);
            PRINT_LANE_ONE_ARG("LABEL", else_label);
        } else if(e->name.kwt == WHILE_K && is_function){
            inWhile = true;
            gen_while(table, e);
        } else if(e->name.type == VAR_ID && is_function){
            if(e->argslist->list[1].arg.type == IDENTIFIER){
                func_call_asign(e);
            }else{
                var_expr_gen(e);
            }
        }
        //printujem telo funkcie
        if(is_function && is_end == false){
            func_main_print(table, e, ret_type, &i);
        }
    }
}
/**
 *@brief A function that prints an user-defined function
 *@param element* e -> pointer on element from table
 */
RetType def_func_start(element* e, ht_table_t* table ){
    RetType rt;
    if(e->argslist != NULL) {
        printf("\n");
        printf("#ZACALA NOVA FUNKCIA !!!!!\n");
        char final[500];
        snprintf(final, sizeof final, "$%s", e->name.info);
        PRINT_LANE_ONE_ARG("LABEL", final);
        PRINT_LANE_ZERO_ARG("CREATEFRAME");
        PRINT_LANE_ZERO_ARG("PUSHFRAME");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_STMT");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_LEFT_EXPR");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_RIGHT_EXPR");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_STMT");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_LEFT_EXPR");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_RIGHT_EXPR");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@INT2FLOATVAR");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@FUNC_RETURNED_ME_A_VAR_THANK_YOU_FUNC");;
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@IM_FUNCTION_AND_I_RETURN_THIS");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@ONLY_FOR_GREATER_EQUAL");
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@ONLY_FOR_LESS_EQUAL");

        char *vars = malloc(sizeof(char));
        char *print = NULL;
        int curly = 0, i = 0;
        bool inFunction = false;
        char func[MAX_HT_SIZE];
        element *search;
        strcpy(vars, "\0");

        while(!0){
            sprintf(func, "%d", i++);
            search = ht_get(table, func);
            if(search == NULL)break;

            if(search->name.type == IDENTIFIER && search->ret_type.type != ERROR_T){
                inFunction = true;
            } else if(search->name.type == LEFT_CURLY_BRACKET){
                curly++;
            } else if(search->name.type == RIGHT_CURLY_BRACKET){
                curly--;
                if(curly == 0){
                    inFunction = false;
                }
            } else if(search->name.type == VAR_ID && inFunction){
                if(is_var_new(vars, search->name.info)){
                    vars = realloc(vars, sizeof(char) * (strlen(vars) + strlen(search->name.info) + 2));
                    strcat(vars, search->name.info);
                    strcat(vars, ";");
                    print = retype_string(search->name);
                    PRINT_LANE_ONE_ARG("DEFVAR", print);
                }
            }
        }

        if(vars != NULL){
            free(vars);
        }
        if(print != NULL){
            free(print);
        }
    }

    if(e->ret_type.kwt == STRING_K){
        rt.return_type = 1; //func return is string
    }else if(e->ret_type.kwt == INT_K){
        rt.return_type = 2; //func return is int
    }else if(e->ret_type.kwt == FLOAT_K){
        rt.return_type = 3; //func return is float
    }else{
        rt.return_type = 0;
    }

    if(e->nullRet){
        rt.canBeNull = true;
    } else {
        rt.canBeNull = false;
    }
    return rt;
}
/**
 *@brief A function defines and assigns values to the variable arguments that are passed to the function
 *@param element* e -> pointer on element from table
 */
void func_arg_print(element* e){
    if(e->argslist != NULL) {
        char final_var [500];

        for (int i = 0; i < e->argslist->len; ++i) {
            if(e->argslist->list->arg.info[0] == '$') {
                memmove(e->argslist->list[i].arg.info, e->argslist->list[i].arg.info + 1,
                        strlen(e->argslist->list[i].arg.info));
            }
            snprintf(final_var, sizeof final_var, "LF@%s", e->argslist->list[i].arg.info);
            PRINT_LANE_ONE_ARG("DEFVAR", final_var);
            PRINT_LANE_ONE_ARG("POPS", final_var);
        }
    }
}
/**
 * TODO - dokoncit funkciu -- funkcia, ktora by mala pushovat pri returne
 */
void func_return(element* e, RetType ret_type){
    if(ret_type.return_type == 0){
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@return");
        PRINT_LANE_TWO_ARG("MOVE", "LF@return", "nil@nil");
        PRINT_LANE_ONE_ARG("PUSHS", "LF@return");
    }else if(ret_type.return_type == 1){
        return_expr(e);
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@return");
        PRINT_LANE_TWO_ARG("MOVE", "LF@return", "LF@IM_FUNCTION_AND_I_RETURN_THIS");
        PRINT_LANE_ONE_ARG("PUSHS", "LF@return");
    }else if(ret_type.return_type == 2){
        return_expr(e);
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@return");
        PRINT_LANE_TWO_ARG("MOVE", "LF@return", "LF@IM_FUNCTION_AND_I_RETURN_THIS");
        PRINT_LANE_ONE_ARG("PUSHS", "LF@return");
    }else if(ret_type.return_type == 3){
        return_expr(e);
        PRINT_LANE_ONE_ARG("DEFVAR", "LF@return");
        PRINT_LANE_TWO_ARG("MOVE", "LF@return", "LF@IM_FUNCTION_AND_I_RETURN_THIS");
        PRINT_LANE_ONE_ARG("PUSHS", "LF@return");
    }
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
    printf("\n");
}
/**
 *@brief A function that prints the function body
 *@param element* e -> pointer on element from table
 * TODO - make it :D tlacit by mala vsetko okrem volania inej funkcii
 */
void func_main_print(ht_table_t *table, element* e, RetType ret_type, int *key){
    if(e->name.isKeyword && e->name.kwt == RETURN_K){
        func_return(e, ret_type);
    } else if(e->name.isKeyword && e->name.kwt == IF_K){
        gen_if(table, e);
    } else if(e->name.isKeyword && e->name.kwt == ELSE_K){
        return;
    } else if(e->name.isKeyword && e->name.kwt == WHILE_K){
        return;
    } else if(e->name.type == IDENTIFIER){
        gen_func_call(table, *e); //ak sa z funkcie vola druha funkcia
    }
}
/**
 *@brief A function that declares the arguments to be passed to the function
 *@param element call -> the arguments we want to push for the called function
 *@param ht_table_t *table -> pointer to place where the entire code from input is stored
 */
void gen_func_call(ht_table_t *table, element call){
    char index[MAX_HT_SIZE];
    element* compare_e = NULL;
    char* print = NULL;

    int i = 0;
    while (!0){
        sprintf(index, "%d", i++);
        compare_e = ht_get(table, index);
        if(compare_e == NULL) break;

        if (compare_e->ret_type.type != ERROR_T) {
            if (call.argslist != NULL) {
                if (strcmp(compare_e->name.info, call.name.info) == 0) {
                    for (int j = 0; j < call.argslist->len; ++j) {
                        if (strcmp(call.argslist->list[j].arg.info, ",") != 0 && strcmp(call.name.info, "write") != 0 )  {
                            print = retype_string(call.argslist->list[j].arg);
                            PRINT_LANE_ONE_ARG("PUSHS", print);
                        }

                    }
                    //argumenty do write musime pushovat opacne na zasobnik a to od konca
                    if(strcmp(call.name.info, "write") == 0) {
                        for (int j = call.argslist->len - 1; j >= 0; j--) {
                            if (strcmp(call.argslist->list[j].arg.info, ",") != 0 )  {
                                print = retype_string(call.argslist->list[j].arg);
                                PRINT_LANE_ONE_ARG("PUSHS", print);
                            }

                        }
                        printf("PUSHS int@%d\n", call.argslist->len); //pushneme ta stack pocet argumentov, ktore vypisujeme
                    }
                    func_call(call.name.info);
                }
            }
        }
    }

    if (strcmp(call.name.info, "readi") == 0) {
        func_call(call.name.info);
    }else if (strcmp(call.name.info, "reads") == 0){
        func_call(call.name.info);
    }else if (strcmp(call.name.info, "readf") == 0){
        func_call(call.name.info);
    }
}
/**
 *@brief A function that calls a function
 *@param element call -> name of the function
 */
void func_call(char* call){
    char final[500];
    snprintf(final, sizeof final, "$%s", call);
    PRINT_LANE_ONE_ARG("CALL", final);
}

//---------------------------------------MAIN-----------------------------------------------------//

/**
 *@brief A function that generates main
 *@param ht_table_t *table -> place where the entire code from input is stored
 *@param key -> ID of element in Table
 */
void gen_main(ht_table_t *table, int key){
    bool inFunction = false, inElse = false, inWhile = false;
    int curly = 0, elseCurly = 0, whileCurly = 0;
    char index[MAX_HT_SIZE];
    char else_end_label[20] = "$else_end_";
    char *vars = malloc(sizeof(char));
    char *print = NULL;
    strcpy(vars, "\0");

    PRINT_LANE_ONE_ARG("LABEL", "$main");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@FUNC_RETURNED_ME_A_VAR_THANK_YOU_FUNC");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_STMT");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_LEFT_EXPR");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@IF_RIGHT_EXPR");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_STMT");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_LEFT_EXPR");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@WHILE_RIGHT_EXPR");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@INT2FLOATVAR");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@ONLY_FOR_GREATER_EQUAL");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@ONLY_FOR_LESS_EQUAL");

    for(int i = 0; i < key; i++){
        element* e = NULL;
        sprintf(index, "%d", i);
        e = ht_get(table, index);
        if(e->name.type == IDENTIFIER && e->ret_type.type != ERROR_T){
            inFunction = true;
        } else if(e->name.type == LEFT_CURLY_BRACKET){
            curly++;
        } else if(e->name.type == RIGHT_CURLY_BRACKET){
            curly--;
            if(curly == 0){
                inFunction = false;
            }
        } else if(e->name.type == VAR_ID && inFunction == false){
            if(is_var_new(vars, e->name.info)){
                vars = realloc(vars, sizeof(char) * (strlen(vars) + strlen(e->name.info) + 2));
                strcat(vars, e->name.info);
                strcat(vars, ";");
                print = retype_string(e->name);
                PRINT_LANE_ONE_ARG("DEFVAR", print);
            }
        }
    }

    if(vars != NULL){
        free(vars);
    }
    if(print != NULL){
        free(print);
    }

    inFunction = false;
    inWhile = false;
    inElse = false;
    curly = 0;

    for (int i = 0; i < key ; i++) {
        element* e = NULL;
        sprintf(index, "%d", i);
        e = ht_get(table, index);
        if(e == NULL){break;}
        if(e->name.type == IDENTIFIER){
            if (e->name.isKeyword && e->name.kwt == RETURN_K && inFunction == false){
                PRINT_LANE_ONE_ARG("JUMP", "$main_end");
            } else if(e->ret_type.type == ERROR_T && inFunction == false) {
                if(e->name.kwt == IF_K){
                    gen_if(table, e);
                } else if(e->name.kwt == ELSE_K){
                    char tmp[HT_SIZE];
                    sprintf(tmp, "%d", counter++);
                    else_end_label[10] = '\0';
                    strcat(else_end_label, tmp);
                    PRINT_LANE_ONE_ARG("JUMP", else_end_label);
                    PRINT_LANE_ONE_ARG("LABEL", else_label);
                    inElse = true;
                } else if(e->name.kwt == WHILE_K){
                    gen_while(table, e);
                    inWhile = true;
                }
                if(inFunction == false) {
                    gen_func_call(table, *e);
                }
            } else {
                inFunction = true;
            }
        } else if(e->name.type == LEFT_CURLY_BRACKET){
            curly++;
            if(inElse){
                elseCurly++;
            }
            if(inWhile){
                whileCurly++;
            }
        } else if(e->name.type == RIGHT_CURLY_BRACKET){
            curly--;
            if(curly == 0){
                inFunction = false;
            }
            if(inElse){
                elseCurly--;
                if(elseCurly == 0){
                    inElse = false;
                    PRINT_LANE_ONE_ARG("LABEL", else_end_label);
                }
            }
            if(inWhile){
                whileCurly--;
                if(whileCurly == 0){
                    inWhile = false;
                    PRINT_LANE_ONE_ARG("JUMP", cycle_name);
                    PRINT_LANE_ONE_ARG("LABEL", cycle_end);
                }
            }
        } else if (e->name.type == VAR_ID && inFunction == false){
            if(inFunction == false && e->argslist != NULL){
                if(e->argslist->list[1].arg.type == IDENTIFIER){
                    func_call_asign(e);
                } else {
                    var_expr_gen(e);
                }
            }
        }
    }

    PRINT_LANE_ONE_ARG("LABEL", "$main_end");
}

/**
 *@brief A function that retypes argument to the required form LF@ / int@ / float@ / string@
 *@param Token arg - the input argument that we cast
 */
char *retype_string(Token arg){
    char *final_arg = NULL;
    char tmp[MAX_HT_SIZE];
    if(arg.type == NUMBER){
        final_arg = malloc(sizeof (char) * (4 + (int)strlen(arg.info) + 1));
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }

        strcpy(final_arg, "int@");
        strcat(final_arg, arg.info);

    }else if(arg.type == DECIMAL_NUMBER){
        sprintf(tmp, "%a", strtod(arg.info, NULL));

        final_arg = malloc(sizeof (char) * (8 + (int)strlen(tmp) + 1));
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "float@");
        strcat(final_arg, tmp);

    }else if(arg.type == STRING){
        char cTmp[6];

        final_arg = malloc(sizeof (char) * 8);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }

        strcpy(final_arg, "string@");
        arg.info[strlen(arg.info)-1] = '\0';
        memmove(arg.info, arg.info+1, strlen(arg.info));

        int lenght_string =  (int) strlen(arg.info);
        for (int i = 0; i < lenght_string ; i++) {
            int character = (int) arg.info[i];
            if((character >= '\n' && character <= ' ') ||  character == '#' || character == '\\'){
                if(character == '\\'){
                    if(arg.info[i+1] == 'n'){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 5));
                        sprintf(cTmp, "\\0%d", '\n');
                        strcat(final_arg, cTmp);
                        i++;
                        continue;
                    } else if(arg.info[i+1] == '\\'){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 5));
                        sprintf(cTmp, "\\0%d", '\\');
                        strcat(final_arg, cTmp);
                        i++;
                        continue;
                    } else if(arg.info[i+1] == 't'){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 5));
                        sprintf(cTmp, "\\0%d", '\t');
                        strcat(final_arg, cTmp);
                        i++;
                        continue;
                    } else if(arg.info[i+1] == '$'){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 2));
                        sprintf(cTmp, "%c", '$');
                        strcat(final_arg, cTmp);
                        i++;
                        continue;
                    } else if(arg.info[i+1] == '"'){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 2));
                        sprintf(cTmp, "%c", '"');
                        strcat(final_arg, cTmp);
                        i++;
                        continue;
                    } else if(isdigit(arg.info[i+1]) && isdigit(arg.info[i+2]) && isdigit(arg.info[i+3])){
                        final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 5));
                        sprintf(cTmp, "\\%c%c%c",arg.info[i+1],arg.info[i+2],arg.info[i+3] );
                        strcat(final_arg, cTmp);
                        i += 3;
                        continue;
                    } else if(arg.info[i+1] == 'x'
                              && ((isdigit(arg.info[i+2] ) || ((arg.info[i+2] >= 'A') && (arg.info[i+2] <= 'F')) || ((arg.info[i+2] >= 'a') && (arg.info[i+2] <= 'f')))
                                  && (isdigit(arg.info[i+3] ) || ((arg.info[i+3] >= 'A') && (arg.info[i+3] <= 'F')) || ((arg.info[i+3] >= 'a') && (arg.info[i+3] <= 'f'))))){
                        sprintf(cTmp, "\\%c%c%c",arg.info[i+1],arg.info[i+2],arg.info[i+3] );
                        int long long number =  hexa_to_octal(cTmp);
                        strcat(final_arg, "\\");
                        if(number < 100){
                            strcat(final_arg, "0");
                        }
                        if(number < 10){
                            strcat(final_arg, "0");
                        }
                        char buffer[MAX_HT_SIZE];
                        sprintf(buffer, "%d", (int)number);
                        strcat(final_arg, buffer);
                        i += 3;
                        continue;
                    }
                }
                final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 9));
                sprintf(cTmp, "\\0%d", character);
                strcat(final_arg, cTmp);
            } else if (character >= 0 && character < '\n'){
                final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 6));
                sprintf(cTmp, "\\00%d", character);
                strcat(final_arg, cTmp);
            } else if (character != 0){
                final_arg = realloc(final_arg, sizeof(char) * ((int) strlen(final_arg) + 2));
                sprintf(cTmp, "%c", character);
                strcat(final_arg, cTmp);
            }
        }
    }else if(arg.type == VAR_ID){
        final_arg = malloc(sizeof (char) * (3 + (int)strlen(arg.info) + 1));
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "LF@");
        if(arg.info[0] == '$'){
            memmove(arg.info, arg.info+1, strlen(arg.info));
        }
        strcat(final_arg, arg.info);
    }else if(arg.kwt == NULL_K){
        final_arg = malloc(sizeof (char) * (3 + (int)strlen(arg.info) + 1));
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "nil@nil");

    }
    return final_arg;
}

/**
 *@brief A function that converts hexadecimal notation to octal notation
 *@param const char hex[] - Input argument in hexa form
 */
long long hexa_to_octal(const char hex[]){
    long long octal_number, bin_number, spot;
    int rem, val;

    octal_number = 0ll;
    bin_number = 0ll;
    spot= 0ll;

    for(int i=0; hex[i]!='\0'; i++){
        bin_number = bin_number * spot;

        switch(hex[i]){
            case '0':
                bin_number += 0;
                break;
            case '1':
                bin_number += 1;
                break;
            case '2':
                bin_number += 10;
                break;
            case '3':
                bin_number += 11;
                break;
            case '4':
                bin_number += 100;
                break;
            case '5':
                bin_number += 101;
                break;
            case '6':
                bin_number += 110;
                break;
            case '7':
                bin_number += 111;
                break;
            case '8':
                bin_number += 1000;
                break;
            case '9':
                bin_number += 1001;
                break;
            case 'a':
            case 'A':
                bin_number += 1010;
                break;
            case 'b':
            case 'B':
                bin_number += 1011;
                break;
            case 'c':
            case 'C':
                bin_number += 1100;
                break;
            case 'd':
            case 'D':
                bin_number += 1101;
                break;
            case 'e':
            case 'E':
                bin_number += 1110;
                break;
            case 'f':
            case 'F':
                bin_number += 1111;
                break;
            default:
                break;
        }
        spot = 10000;
    }
    spot = 1;

    while(bin_number> 0){
        rem = bin_number % 1000;
        switch(rem){
            case 0:
                val = 0;
                break;
            case 1:
                val = 1;
                break;
            case 10:
                val = 2;
                break;
            case 11:
                val = 3;
                break;
            case 100:
                val = 4;
                break;
            case 101:
                val = 5;
                break;
            case 110:
                val = 6;
                break;
            case 111:
                val = 7;
                break;
            default:
                break;
        }
        octal_number = (val * spot) + octal_number;
        bin_number /= 1000;
        spot *= 10;
    }
    return octal_number;
}

//---------------------------------------BUILD-IN FUNCTINOS---------------------------------------//
//-------------------------------These functions are written in IFJcode22-------------------------//
/**
 * @brief A built-in function that reads strings from standard input
 * -string pushes on the stack
 */
void func_reads(){
    printf("\n#ZACALA FUNKCIA READS !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$reads");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@error");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@read_string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_string");

    PRINT_LANE_TWO_ARG("READ", "LF@read_string", "string");
    PRINT_LANE_TWO_ARG("TYPE", "LF@error", "LF@read_string");
    PRINT_LANE_THREE_ARG("JUMPIFNEQ", "$error_read", "string@string", "LF@error");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_string", "LF@read_string");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_string");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$error_read");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_string", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_string");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief A built-in function that reads int from standard input
 * -int pushes on the stack
 */
void func_readi(){
    printf("\n#ZACALA FUNKCIA READI !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readi");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@error");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@read_int");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");

    PRINT_LANE_TWO_ARG("READ", "LF@read_int", "int");
    PRINT_LANE_TWO_ARG("TYPE", "LF@error", "LF@read_int");
    PRINT_LANE_THREE_ARG("JUMPIFNEQ", "$error_read", "string@int", "LF@error");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_int", "LF@read_int");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$error_read");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_int", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief A built-in function that reads float from standard input
 * -float pushes on the stack
 */
void func_readf(){
    printf("\n#ZACALA FUNKCIA READI !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readf");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@error");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@read_float");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_float");

    PRINT_LANE_TWO_ARG("READ", "LF@read_float", "float");
    PRINT_LANE_TWO_ARG("TYPE", "LF@error", "LF@read_float");
    PRINT_LANE_THREE_ARG("JUMPIFNEQ", "$error_read", "string@float", "LF@error");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_float", "LF@read_float");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_float");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$error_read");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_float", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_float");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

/**
 * @brief Built-in function for printing
 * The number of arguments must be pushed on the stack, and given arguments
 */
void func_write(){
    printf("\n#ZACALA FUNKCIA WRITE !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$write");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@type_of_tmp");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@num_of_cycles");
    PRINT_LANE_ONE_ARG("POPS", "LF@num_of_cycles");

    PRINT_LANE_ONE_ARG("LABEL", "$while_write");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$while_end", "LF@num_of_cycles", "int@0" );

    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");
    PRINT_LANE_TWO_ARG("TYPE", "LF@type_of_tmp", "LF@tmp");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$writenull", "LF@type_of_tmp", "string@nil" );

    PRINT_LANE_ONE_ARG("WRITE", "LF@tmp");
    PRINT_LANE_THREE_ARG("SUB", "LF@num_of_cycles", "LF@num_of_cycles", "int@1" );
    PRINT_LANE_ONE_ARG("JUMP", "$while_write");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$writenull");
    PRINT_LANE_ONE_ARG("WRITE", "nil@nil");
    PRINT_LANE_THREE_ARG("SUB", "LF@num_of_cycles", "LF@num_of_cycles", "int@1" );
    PRINT_LANE_ONE_ARG("JUMP", "$while_write");

    PRINT_LANE_ONE_ARG("LABEL", "$while_end");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief Built-in function to change INT to FLOAT
 * It is necessary to push INT number to the function via stack
 */
void func_floatval(){
    printf("\n#ZACALA FUNKCIA FLOATVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$floatval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@type");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");

    PRINT_LANE_TWO_ARG("TYPE","LF@type", "LF@number");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end_is_float", "LF@type", "string@float");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@number", "nil@nil");
    PRINT_LANE_TWO_ARG("INT2FLOAT","LF@number", "LF@number");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@number");
    PRINT_LANE_ONE_ARG("JUMP", "$end_float");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_TWO_ARG("MOVE", "LF@number", "float@0x0p+0");
    PRINT_LANE_TWO_ARG("INT2FLOAT","LF@number", "LF@number");
    PRINT_LANE_ONE_ARG("LABEL", "$end_is_float");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@number");

    PRINT_LANE_ONE_ARG("LABEL", "$end_float");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief Built-in function to change FLOAT to INT
 * It is necessary to push FLOAT number to the function via stack
 */
void func_intval(){
    printf("\n#ZACALA FUNKCIA INTVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$intval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@type");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");

    PRINT_LANE_TWO_ARG("TYPE","LF@type", "LF@number");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end_is_int", "LF@type", "string@int");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@number", "nil@nil");
    PRINT_LANE_TWO_ARG("FLOAT2INT","LF@number", "LF@number");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@number");
    PRINT_LANE_ONE_ARG("JUMP", "$end_int");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_TWO_ARG("MOVE", "LF@number", "int@0");
    PRINT_LANE_TWO_ARG("FLOAT2INT","LF@number", "LF@number");
    PRINT_LANE_ONE_ARG("LABEL", "$end_is_int");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@number");

    PRINT_LANE_ONE_ARG("LABEL", "$end_int");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief In our case, the function just returns a string or nill
 */
void func_strval(){
    printf("\n#ZACALA FUNKCIA STRVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$strval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@string");
    PRINT_LANE_ONE_ARG("POPS", "LF@string");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@string", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@string");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_ONE_ARG("PUSHS", "string@");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief Built-in function returns the length of the argument that was pushed on stack
 */
void func_strlen(){
    printf("\n#ZACALA FUNKCIA STRLEN !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$strlen");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_val");
    PRINT_LANE_ONE_ARG("POPS", "LF@return_val");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@return_val", "LF@return_val");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_val");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief Built-in function returns the substring of the string that was pushed on stack
 */
void func_substring(){
    printf("\n#ZACALA FUNKCIA SUBSTRING !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$substring");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@end_index");
    PRINT_LANE_ONE_ARG("POPS", "LF@end_index");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@start_index");
    PRINT_LANE_ONE_ARG("POPS", "LF@start_index");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@string");
    PRINT_LANE_ONE_ARG("POPS", "LF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@strlen_string");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@strlen_string", "LF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return");

    //j<0
    PRINT_LANE_TWO_ARG("MOVE", "LF@return", "bool@false");
    PRINT_LANE_THREE_ARG("LT", "LF@return", "LF@end_index", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@return", "bool@true");

    //i<0
    PRINT_LANE_THREE_ARG("LT", "LF@return", "LF@start_index", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@return", "bool@true");

    //to do kontrola ci j > strlen
    PRINT_LANE_THREE_ARG("GT", "LF@return", "LF@end_index", "LF@strlen_string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@return", "bool@true");

    //to do kontrola ci i > strlen
    PRINT_LANE_THREE_ARG("GT", "LF@return", "LF@start_index", "LF@strlen_string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@return", "bool@true");

    // i > j
    PRINT_LANE_THREE_ARG("SUB", "LF@end_index" , "LF@end_index", "int@1"); //j-1
    PRINT_LANE_THREE_ARG("GT", "LF@return", "LF@start_index", "LF@end_index");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@return", "bool@true");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp_string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_string");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_string", "string@");

    PRINT_LANE_ONE_ARG("LABEL", "$start_while");
    PRINT_LANE_THREE_ARG("LT", "LF@return", "LF@end_index", "LF@start_index"); //j<i
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end_while", "LF@return", "bool@true");

    PRINT_LANE_THREE_ARG("GETCHAR", "LF@tmp_string", "LF@string", "LF@start_index");
    PRINT_LANE_THREE_ARG("CONCAT", "LF@return_string","LF@return_string", "LF@tmp_string");

    PRINT_LANE_THREE_ARG("ADD", "LF@start_index", "LF@start_index", "int@1");
    PRINT_LANE_ONE_ARG("JUMP", "$start_while");

    PRINT_LANE_ONE_ARG("LABEL", "$end_while");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_string");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

/**
 * @brief Built-in function to change first CHAR of STRINF to INT
 * It is necessary to push STRING to the function via stack
 */
void func_ord(){
    printf("\n#ZACALA FUNKCIA ORD !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$ord");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@string");
    PRINT_LANE_ONE_ARG("POPS", "LF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_int", "int@0");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@strlen_string");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@strlen_string", "LF@string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@strlen_string", "LF@return_int");

    PRINT_LANE_THREE_ARG("STRI2INT", "LF@return_int", "LF@string", "LF@return_int");

    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_int");
    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
/**
 * @brief Built-in function to change INT to CHAR
 * It is necessary to push INT to the function via stack
 */
void func_chr(){
    printf("\n#ZACALA FUNKCIA CHR !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$chr");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@result");

    PRINT_LANE_THREE_ARG("LT", "LF@result", "LF@number", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@result", "bool@true");

    PRINT_LANE_THREE_ARG("GT", "LF@result", "LF@number", "int@255");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@result", "bool@true");

    PRINT_LANE_TWO_ARG("INT2CHAR", "LF@result", "LF@number");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@result");
    PRINT_LANE_ONE_ARG("LABEL", "$end");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//---------------------------------------EXPR---------------------------------------//
void func_call_asign(element *e){
    char tmp[MAX_HT_SIZE];
    char* print = NULL;
    for(int i = 3; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type != COMMA){
            print = retype_string(e->argslist->list[i].arg);
            PRINT_LANE_ONE_ARG("PUSHS", print);
        }
    }
    func_call(e->argslist->list[1].arg.info);
    PRINT_LANE_ONE_ARG("POPS", "LF@FUNC_RETURNED_ME_A_VAR_THANK_YOU_FUNC");
    print = retype_string(e->name);
    PRINT_LANE_TWO_ARG("MOVE", print, "LF@FUNC_RETURNED_ME_A_VAR_THANK_YOU_FUNC");

    if(print != NULL){
        free(print);
    }
}

void var_expr_gen(element *e){
    char *var = NULL, *arg = NULL;
    bool allFloat = false, printFloat = false;
    Token operator;

    // Check if we have any float

    for(int i = 1; i < e->argslist->len+1; i++){
        if(e->argslist->list[i].arg.type == DECIMAL_NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER){
            allFloat = true;
        }
    }

    var = retype_string(e->name);
    arg = retype_string(e->argslist->list[1].arg);
    PRINT_LANE_TWO_ARG("MOVE", var, arg);
    if(e->argslist->list[1].arg.type == NUMBER && allFloat){
        PRINT_LANE_TWO_ARG("INT2FLOAT", var, var);
    }



    for(int i = 2; i < e->argslist->len+1; i++){
        if(i % 2 == 0){
            operator = e->argslist->list[i].arg;
        } else {
            arg = retype_string(e->argslist->list[i].arg);
            if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                PRINT_LANE_TWO_ARG("INT2FLOAT", "LF@INT2FLOATVAR", arg);
                printFloat = true;
            }
            if(operator.type == MINUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("SUB", var, var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("SUB", var, var,arg);
                }
            } else if(operator.type == PLUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("ADD", var, var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("ADD", var, var, arg);
                }
            } else if(operator.type == DIVIDE){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("DIV", var, var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("DIV", var, var, arg);
                }
            } else if(operator.type == MULTIPLY){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("MUL", var, var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("MUL", var, var, arg);
                }
            } else if(operator.type == DOT){
                PRINT_LANE_THREE_ARG("CONCAT", var, var, arg);
            }
        }
    }

    if(var != NULL){
        free(var);
    }

    if(arg != NULL){
        free(arg);
    }
}

void return_expr(element *e){
    char* print = NULL;
    Token operator;
    bool allFloat = false, printFloat = false;
    char var[35] = "LF@IM_FUNCTION_AND_I_RETURN_THIS";

    if(e->argslist == NULL || e->argslist->list == NULL){
        return;
    }

    for(int i = 0; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type == DECIMAL_NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER){
            allFloat = true;
        }
    }

    if(e->argslist->list[0].arg.type != SEMICOLON){
        print = retype_string(e->argslist->list[0].arg);
        PRINT_LANE_TWO_ARG("MOVE", var, print);
    } else {
        return;
    }

    for(int i = 1; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type != SEMICOLON){
            if(i % 2 != 0){
                operator = e->argslist->list[i].arg;
            } else {
                print = retype_string(e->argslist->list[i].arg);
                if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                    PRINT_LANE_TWO_ARG("INT2FLOAT", "LF@INT2FLOATVAR", print);
                    printFloat = true;
                }
                if(operator.type == MINUS){
                    if(printFloat){
                        PRINT_LANE_THREE_ARG("SUB", var, var,"LF@INT2FLOATVAR");
                    } else {
                        PRINT_LANE_THREE_ARG("SUB", var, var,print);
                    }
                } else if(operator.type == PLUS){
                    if(printFloat){
                        PRINT_LANE_THREE_ARG("ADD", var, var,"LF@INT2FLOATVAR");
                    } else {
                        PRINT_LANE_THREE_ARG("ADD", var, var, print);
                    }
                } else if(operator.type == DIVIDE){
                    if(printFloat){
                        PRINT_LANE_THREE_ARG("DIV", var, var,"LF@INT2FLOATVAR");
                    } else {
                        PRINT_LANE_THREE_ARG("DIV", var, var, print);
                    }
                } else if(operator.type == MULTIPLY){
                    if(printFloat){
                        PRINT_LANE_THREE_ARG("MUL", var, var,"LF@INT2FLOATVAR");
                    } else {
                        PRINT_LANE_THREE_ARG("MUL", var, var, print);
                    }
                } else if(operator.type == DOT){
                    PRINT_LANE_THREE_ARG("CONCAT", var, var, print);
                }
            }
        }
    }

    if(print != NULL){
        free(print);
    }
}

void gen_if(ht_table_t *t, element *e){
    char* print = NULL;
    element compare;
    Token operator;
    int newLF = counter;
    char tmp[MAX_HT_SIZE];
    char if_var[25] = "LF@IF_LEFT_EXPR";
    char if_var_1[25] = "LF@IF_LEFT_EXPR";
    char if_var_2[25] = "LF@IF_RIGHT_EXPR";
    bool allFloat = false, printFloat = false;

    if(e->argslist == NULL){
        return;
    }

    for(int i = 0; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type == DECIMAL_NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER){
            allFloat = true;
        }
    }

    compare.argslist = malloc(sizeof(argList));
    compare.argslist->list = malloc(sizeof(arg));

    print = retype_string(e->argslist->list[0].arg);
    PRINT_LANE_TWO_ARG("MOVE", if_var, print);

    for(int i = 1; i < e->argslist->len+1; i++){
        if(i%2 != 0){
            operator = e->argslist->list[i].arg;
        } else {
            print = retype_string(e->argslist->list[i].arg);

            if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                PRINT_LANE_TWO_ARG("INT2FLOAT", "LF@INT2FLOATVAR", print);
                printFloat = true;
            }

            if(operator.type == PLUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("ADD", if_var, if_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("ADD", if_var, if_var, print);
                }
            } else if(operator.type == MINUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("SUB", if_var, if_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("SUB", if_var, if_var,print);
                }
            } else if(operator.type == DIVIDE){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("DIV", if_var, if_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("DIV", if_var, if_var, print);
                }
            } else if(operator.type == MULTIPLY){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("MUL", if_var, if_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("MUL", if_var, if_var, print);
                }
            } else if(operator.type == DOT){
                PRINT_LANE_THREE_ARG("CONCAT", if_var, if_var, print);
            } else {
                if(strcmp(if_var, if_var_1) == 0){
                    strcpy(if_var, if_var_2);
                } else {
                    strcpy(if_var, if_var_1);
                }
                PRINT_LANE_TWO_ARG("MOVE", if_var, print);
            }
        }
    }

    strcpy(if_var, if_var_1);
    PRINT_LANE_TWO_ARG("MOVE", if_main, if_var);
    strcpy(if_var, if_var_2);

    for(int i = 1; i < e->argslist->len+1; i++){
        if(i%2 != 0){
            operator = e->argslist->list[i].arg;
        } else {

            if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                PRINT_LANE_TWO_ARG("INT2FLOAT", "LF@INT2FLOATVAR", if_var);
                printFloat = true;
            }

            if(operator.type == EQUAL){
                PRINT_LANE_THREE_ARG("EQ", if_main, if_main, if_var);
            } else if(operator.type == NOT_EQUAL){
                PRINT_LANE_THREE_ARG("EQ", if_main, if_main, if_var);
                PRINT_LANE_TWO_ARG("NOT", if_main, if_main);
            } else if(operator.type == LESS){
                PRINT_LANE_THREE_ARG("LT", if_main, if_main, if_var);
            } else if(operator.type == GREATER){
                PRINT_LANE_THREE_ARG("GT", if_main, if_main, if_var);
            } else if(operator.type == LESS_EQUAL){
                PRINT_LANE_THREE_ARG("LT", "LF@ONLY_FOR_LESS_EQUAL", if_main, if_var);
                char lt_jump_label[25] = "$lt_successful_";
                sprintf(tmp, "%d", counter++);
                lt_jump_label[15] = '\0';
                strcat(lt_jump_label, tmp);
                PRINT_LANE_THREE_ARG("JUMPIFEQ", lt_jump_label, "LF@ONLY_FOR_LESS_EQUAL", "bool@true");
                PRINT_LANE_THREE_ARG("EQ", "LF@ONLY_FOR_LESS_EQUAL", if_main, if_var);
                PRINT_LANE_ONE_ARG("LABEL", lt_jump_label);
                PRINT_LANE_TWO_ARG("MOVE", if_main, "LF@ONLY_FOR_LESS_EQUAL");
            } else if(operator.type == GREATER_EQUAL){
                PRINT_LANE_THREE_ARG("GT", "LF@ONLY_FOR_GREATER_EQUAL", if_main, if_var);
                char gt_jump_label[25] = "$gt_successful_";
                sprintf(tmp, "%d", counter++);
                gt_jump_label[15] = '\0';
                strcat(gt_jump_label, tmp);
                PRINT_LANE_THREE_ARG("JUMPIFEQ", gt_jump_label, "LF@ONLY_FOR_GREATER_EQUAL", "bool@true");
                PRINT_LANE_THREE_ARG("EQ", "LF@ONLY_FOR_GREATER_EQUAL", if_main, if_var);
                PRINT_LANE_ONE_ARG("LABEL", gt_jump_label);
                PRINT_LANE_TWO_ARG("MOVE", if_main, "LF@ONLY_FOR_GREATER_EQUAL");
            }
        }
    }

    sprintf(tmp, "%d", counter++);
    else_label[7] = '\0';
    strcat(else_label, tmp);

    PRINT_LANE_THREE_ARG("JUMPIFNEQ", else_label,if_main, "bool@true");

    if(print != NULL){
        free(print);
    }
}

void gen_while(ht_table_t *t, element *e){
    char tmp[MAX_HT_SIZE], *print = NULL;
    bool allFloat = false, printFloat = false;
    char while_var[25] = "LF@WHILE_RIGHT_EXPR";
    char while_var_1[25] = "LF@WHILE_LEFT_EXPR";
    char while_var_2[25] = "LF@WHILE_RIGHT_EXPR";
    Token operator;

    if(e->argslist == NULL){
        return;
    }
    for(int i = 0; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type == DECIMAL_NUMBER || e->argslist->list[i].arg.type == EXPONENT_NUMBER){
            allFloat = true;
        }
    }

    int newLF = counter, newLF2 = counter;

    sprintf(tmp, "%d", counter++);
    cycle_name[13] = '\0';
    strcat(cycle_name, tmp);
    PRINT_LANE_ONE_ARG("LABEL", cycle_name);

    sprintf(tmp, "%d", counter++);
    cycle_end[11] = '\0';
    strcat(cycle_end, tmp);

    for(int i = 0; i < e->argslist->len+1; i++){
        if(i%2 != 0){
            operator = e->argslist->list[i].arg;
        } else {
            print = retype_string(e->argslist->list[i].arg);
            if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                PRINT_LANE_TWO_ARG("INT2FLOAT", "LF@INT2FLOATVAR", print);
                printFloat = true;
            }

            if(operator.type == PLUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("ADD", while_var, while_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("ADD", while_var, while_var, print);
                }
            } else if(operator.type == MINUS){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("SUB", while_var, while_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("SUB", while_var, while_var,print);
                }
            } else if(operator.type == DIVIDE){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("DIV", while_var, while_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("DIV", while_var, while_var, print);
                }
            } else if(operator.type == MULTIPLY){
                if(printFloat){
                    PRINT_LANE_THREE_ARG("MUL", while_var, while_var,"LF@INT2FLOATVAR");
                } else {
                    PRINT_LANE_THREE_ARG("MUL", while_var, while_var, print);
                }
            } else if(operator.type == DOT){
                PRINT_LANE_THREE_ARG("CONCAT", while_var, while_var, print);
            } else {
                if(strcmp(while_var, while_var_1) == 0){
                    strcpy(while_var, while_var_2);
                } else {
                    strcpy(while_var, while_var_1);
                }
                PRINT_LANE_TWO_ARG("MOVE", while_var, print);
            }
        }
    }

    strcpy(while_var, while_var_1);
    PRINT_LANE_TWO_ARG("MOVE", while_main, while_var);
    strcpy(while_var, while_var_2);

    for(int i = 1; i < e->argslist->len+1; i++){
        if(i%2 != 0){
            operator = e->argslist->list[i].arg;
        } else {

            if(allFloat && e->argslist->list[i].arg.type == NUMBER){
                PRINT_LANE_TWO_ARG("INT2FLOAT", while_var_2, while_var_2);
                printFloat = true;
            }

            if(operator.type == EQUAL){
                PRINT_LANE_THREE_ARG("EQ", while_main, while_main, while_var_2);
            } else if(operator.type == NOT_EQUAL){
                PRINT_LANE_THREE_ARG("EQ", while_main, while_main, while_var_2);
                PRINT_LANE_TWO_ARG("NOT", while_main, while_main);
            } else if(operator.type == LESS){
                PRINT_LANE_THREE_ARG("LT", while_main, while_main, while_var_2);
            } else if(operator.type == GREATER){
                PRINT_LANE_THREE_ARG("GT", while_main, while_main, while_var_2);
            } else if(operator.type == LESS_EQUAL){
                PRINT_LANE_THREE_ARG("LT", "LF@ONLY_FOR_LESS_EQUAL", while_main, while_var_2);
                char lt_jump_label[25] = "$lt_successful_";
                sprintf(tmp, "%d", counter++);
                lt_jump_label[15] = '\0';
                strcat(lt_jump_label, tmp);
                PRINT_LANE_THREE_ARG("JUMPIFEQ", lt_jump_label, "LF@ONLY_FOR_LESS_EQUAL", "bool@true");
                PRINT_LANE_THREE_ARG("EQ", "LF@ONLY_FOR_LESS_EQUAL", while_main, while_var_2);
                PRINT_LANE_ONE_ARG("LABEL", lt_jump_label);
                PRINT_LANE_TWO_ARG("MOVE", while_main, "LF@ONLY_FOR_LESS_EQUAL");
            } else if(operator.type == GREATER_EQUAL){
                PRINT_LANE_THREE_ARG("GT", "LF@ONLY_FOR_GREATER_EQUAL", while_main, while_var_2);
                char gt_jump_label[25] = "$gt_successful_";
                sprintf(tmp, "%d", counter++);
                gt_jump_label[15] = '\0';
                strcat(gt_jump_label, tmp);
                PRINT_LANE_THREE_ARG("JUMPIFEQ", gt_jump_label, "LF@ONLY_FOR_GREATER_EQUAL", "bool@true");
                PRINT_LANE_THREE_ARG("EQ", "LF@ONLY_FOR_GREATER_EQUAL", while_main, while_var_2);
                PRINT_LANE_ONE_ARG("LABEL", gt_jump_label);
                PRINT_LANE_TWO_ARG("MOVE", while_main, "LF@ONLY_FOR_GREATER_EQUAL");
            }
        }
    }

    PRINT_LANE_THREE_ARG("JUMPIFNEQ", cycle_end, while_main, "bool@true");

    if(print != NULL){
        free(print);
    }
}

//----------------------------------------HELP--------------------------------------//
bool is_var_new(char* var_list, char* var){
    if(var_list == NULL){
        return true;
    }
    char* one_func = malloc(sizeof(char) * (int) strlen(var_list));
    for(int i = 0; i < strlen(var_list); i++){
        int one_func_index = 0;
        while(var_list[i] != ';'){
            one_func[one_func_index++] = var_list[i];
            i++;
        }
        one_func[one_func_index] = '\0';
        if(strcmp(one_func, var) == 0){
            free(one_func);
            return false;
        }
    }
    free(one_func);
    return true;
}