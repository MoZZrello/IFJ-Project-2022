//
// Created by Marek Spirka on 27. 11. 2022.
//
#include "generator.h"
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
    bool is_end = false;  //is_end ak sa dostaneme na koniec funkcie prestaneme print == }
    element* e = NULL;
    char func[MAX_HT_SIZE];
    int ret_type, i = 0;
    while(!0){
        sprintf(func, "%d", i++);
        e = ht_get(table, func);
        if(e == NULL)break;

        if(e->ret_type.type != ERROR_T){
            ret_type = def_func_start(e);
            func_arg_print(e);//print arg of functions
            is_function = true;
            is_end = false;
            continue;
        }
        //telo funkcie zacina { --preskocim
        if (e->name.type == LEFT_CURLY_BRACKET){
            continue;
        //telo funkcie skoncilo }
        }else if (e->name.type == RIGHT_CURLY_BRACKET){
            is_end = true;
        }
        //printujem telo funkcie
        if(is_function && is_end == false){
            func_main_print(table, e, ret_type);
        }
    }
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
    printf("\n");
}
/**
 *@brief A function that prints an user-defined function
 *@param element* e -> pointer on element from table
 */
int def_func_start(element* e ){
    int return_type = 0;
    if(e->argslist != NULL) {
        printf("\n");
        printf("#ZACALA NOVA FUNKCIA !!!!!\n");
        char final[500];
        snprintf(final, sizeof final, "$%s", e->name.info);
        PRINT_LANE_ONE_ARG("LABEL", final);
        PRINT_LANE_ZERO_ARG("CREATEFRAME");
        PRINT_LANE_ZERO_ARG("PUSHFRAME");
    }
    if(e->ret_type.kwt == STRING_K){
        return_type = 1; //func return is string
    }else if(e->ret_type.kwt == INT_K){
        return_type = 2; //func return is int
    }else if(e->ret_type.kwt == FLOAT_K){
        return_type = 3; //func return is float
    }
    return return_type;
}
/**
 *@brief A function defines and assigns values to the variable arguments that are passed to the function
 *@param element* e -> pointer on element from table
 */
void func_arg_print(element* e){
    if(e->argslist != NULL) {
    char final_var [500];

        for (int i = 0; i < e->argslist->len; ++i) {
            memmove(e->argslist->list[i].arg.info, e->argslist->list[i].arg.info + 1,
                    strlen(e->argslist->list[i].arg.info));
            snprintf(final_var, sizeof final_var, "LF@%s", e->argslist->list[i].arg.info);
            PRINT_LANE_ONE_ARG("DEFVAR", final_var);
            PRINT_LANE_ONE_ARG("POPS", final_var);
        }
    }
}
/**
 * TODO - dokoncit funkciu -- funkcia, ktora by mala pushovat pri returne
 */
void func_return(element* e, int ret_type){
    if(ret_type == 0){
        if(e->name.kwt == RETURN_K){

        }
    }else if(ret_type == 1){
        if(e->name.kwt == RETURN_K){
            PRINT_LANE_ONE_ARG("DEFVAR", "LT@return");
            PRINT_LANE_TWO_ARG("MOVE", "LT@return", "nieco!!!");
            PRINT_LANE_ONE_ARG("PUSHS", "LT@return");
        }
    }else if(ret_type == 2){
        if(e->name.kwt == RETURN_K){
            PRINT_LANE_ONE_ARG("DEFVAR", "LT@return");
            PRINT_LANE_TWO_ARG("MOVE", "LT@return", "nieco!!!");
            PRINT_LANE_ONE_ARG("PUSHS", "LT@return");
        }
    }else if(ret_type == 3){
        if(e->name.kwt == RETURN_K){
            PRINT_LANE_ONE_ARG("DEFVAR", "LT@return");
            PRINT_LANE_TWO_ARG("MOVE", "LT@return", "nieco!!!");
            PRINT_LANE_ONE_ARG("PUSHS", "LT@return");
        }
    }
}
/**
 *@brief A function that prints the function body
 *@param element* e -> pointer on element from table
 * TODO - make it :D tlacit by mala vsetko okrem volania inej funkcii
 */
void func_main_print(ht_table_t *table, element* e, int ret_type){
    gen_call_func(table, *e); //ak sa z funkcie vola druha funkcia
    func_return(e, ret_type);
}
/**
 *@brief A function that declares the arguments to be passed to the function
 *@param element call -> the arguments we want to push for the called function
 *@param ht_table_t *table -> pointer to place where the entire code from input is stored
 */
void gen_call_func(ht_table_t *table, element call){
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
                            print = retype_string(call.argslist->list[j]);
                            PRINT_LANE_ONE_ARG("PUSHS", print);
                            free(print);
                        }

                    }
                    //argumenty do write musime pushovat opacne na zasobnik a to od konca
                    if(strcmp(call.name.info, "write") == 0) {
                        for (int j = call.argslist->len - 1; j >= 0; j--) {
                            if (strcmp(call.argslist->list[j].arg.info, ",") != 0 )  {
                                print = retype_string(call.argslist->list[j]);
                                PRINT_LANE_ONE_ARG("PUSHS", print);
                                free(print);
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

//printujeme main
void gen_main(ht_table_t *table, int key){
    bool inFunction = false;
    int curly = 0;
    char index[MAX_HT_SIZE];

    PRINT_LANE_ONE_ARG("LABEL", "$main");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    for (int i = 0; i < key ; ++i) {
        element* e = NULL;
        sprintf(index, "%d", i);
        e = ht_get(table, index);
        if(e == NULL){break;}
        if(e->name.type == IDENTIFIER){
            if (e->name.isKeyword && e->name.kwt == RETURN_K && inFunction == false){
                printf("RETURN V MAINE\n");
            } else if(e->ret_type.type == ERROR_T) {
                if(inFunction == false) {
                    gen_call_func(table, *e);
                }
            } else {
                inFunction = true;
            }
        } else if(e->name.type == LEFT_CURLY_BRACKET){
            curly++;
        } else if(e->name.type == RIGHT_CURLY_BRACKET){
            curly--;
            if(curly == 0){
                inFunction = false;
            }
        } else if (e->name.type == VAR_ID){
            if(inFunction == false && e->argslist != NULL){
                //printf("%s\n", e->name.info);
                if(e->argslist->list[1].arg.type == IDENTIFIER){
                    func_call_asign(e);
                    printf("FUNC CALL\n");
                } else {
                    printf("EXPR ASSIGN\n");
                }
            }
        }
    }
}

char *retype_string(arg arg){
    char *final_arg = NULL;
    char tmp[MAX_HT_SIZE];
    if(arg.arg.type == NUMBER){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "int@");
        strcat(final_arg, arg.arg.info);

    }else if(arg.arg.type == DECIMAL_NUMBER){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "float@");
        sprintf(tmp, "%a", strtod(arg.arg.info, NULL));
        strcat(final_arg, tmp);
    }else if(arg.arg.type == STRING){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "string@");
        arg.arg.info[strlen(arg.arg.info)-1] = '\0';
        memmove(arg.arg.info, arg.arg.info+1, strlen(arg.arg.info));
        strcat(final_arg, arg.arg.info);
    }else if(arg.arg.type == VAR_ID){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "LF@");
        memmove(arg.arg.info, arg.arg.info+1, strlen(arg.arg.info));
        strcat(final_arg, arg.arg.info);
    }
    return final_arg;
}

//---------------------------------------BUILD-IN FUNCTINOS---------------------------------------//
/////------------dorob!!!!!!!!!!!!!!!!!!!
void func_reads(){
    printf("\n#ZACALA FUNKCIA READS !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$reads");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@error");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@read_string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_string");

    PRINT_LANE_TWO_ARG("READ", "LF@read_int", "string");
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

//na stack to co chceme precitat
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

//na stack treba dat INT a funkcia vrati FLOAT
void func_floatval(){
    printf("\n#ZACALA FUNKCIA FLOATVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$floatval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_float");

    PRINT_LANE_TWO_ARG("INT2FLOAT","LF@return_float", "LF@number");

    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_float");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack treba dat FLOAT a funckia vrati INT
void func_intval(){
    printf("\n#ZACALA FUNKCIA INTVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$intval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");

    PRINT_LANE_TWO_ARG("FLOAT2INT","LF@return_int", "LF@number");

    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack string a ona vrati bud string alebo null string
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

//na stack treba dat string a funckia vrati jeho dlzku
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

//na stack v poradi treba dat ->string, start_index (i), end_index (j) ... funkcia vrati string medzi tymito indexami
void func_substring(){
    printf("\n#ZACALA FUNKCIA SUBSTRING !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$func_strlen");
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

//na stack string a funkia vracia INT hodnotu prveho znaku stringu
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

//na stack treba dat INT a funkcia vrati charakter v prislunom indexe ASCII
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
    char* print = NULL;
    for(int i = 3; i < e->argslist->len; i++){
        if(e->argslist->list[i].arg.type != COMMA){
            print = retype_string(e->argslist->list[i]);
            PRINT_LANE_ONE_ARG("PUSHS", print);
        }
    }
}