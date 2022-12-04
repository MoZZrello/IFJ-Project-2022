//
// Created by Marek Spirka on 27. 11. 2022.
//
#include "generator.h"

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

//generator celeho kodu
void gen_program(ht_table_t *table, int no_build_in_func){
    start_program();
    gen_built_in_functions(table, no_build_in_func);
    gen_function(table);
    gen_main(table);
}

void start_program(){
    PRINT_LANE_ZERO_ARG(".IFJcode22");
    PRINT_LANE_ONE_ARG("JUMP", "$main");
}

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

//funckia na generovanie celej funkcii
void gen_function(ht_table_t *table){
    bool is_function = false; // zistenie ci mame funkciu ak ano printime ak nie nic sa nestane,
    bool is_end = false;  //is_end ak sa dostaneme na koniec funkcie prestaneme print == }
    element* e = NULL;
    int i = 0;
    while(!0){
        char func[MAX_HT_SIZE];
        sprintf(func, "%d", i++);
        e = ht_get(table, func);
        if(e == NULL)break;

        if(e->ret_type.type != ERROR_T){
            def_func_start(e);
            def_func_arg_print(e);//print arg of functions
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
            gen_call_func(table, *e);
            def_func_main_print(e);
        }
    }
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
    printf("\n");
}

//funckia na printenie zaciatku funkcie
void def_func_start(element* e ){
    if(e->argslist != NULL) {
        printf("\n");
        printf("#ZACALA NOVA FUNKCIA !!!!!\n");
        char final[500];
        snprintf(final, sizeof final, "$%s", e->name.info);
        PRINT_LANE_ONE_ARG("LABEL", final);
        PRINT_LANE_ZERO_ARG("CREATEFRAME");
    }
}

void def_func_arg_print(element* e){
    if(e->argslist != NULL) {
    char final_var [500];

        for (int i = 0; i < e->argslist->len; ++i) {
            memmove(e->argslist->list[i].arg.info, e->argslist->list[i].arg.info + 1,strlen(e->argslist->list[i].arg.info));
            snprintf(final_var, sizeof final_var, "TF@%s", e->argslist->list[i].arg.info);
            PRINT_LANE_ONE_ARG("DEFVAR", final_var);
            PRINT_LANE_ONE_ARG("POPS", final_var);
        }
    }

}
//printujem telo medzi { a }

void def_func_main_print(element* e){
    /*
        if(strcmp(e->name.info , "write") == 0){
            func_write();
        }*/
    //printf("%s\n", e->name.info);

}

//push arg to stuck and call functions
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
                        if (strcmp(call.argslist->list[j].arg.info, ",") != 0) {
                            print = retype_arg_for_func(call.argslist->list[j]);
                            PRINT_LANE_ONE_ARG("PUSHS", print);
                        }
                    }
                    func_call(call.name.info);
                    def_func_arg_print(&call); ///vola sa to vela krat staci len raz
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
//call functions
void func_call(char* call){
    char final[500];
    snprintf(final, sizeof final, "$%s", call);
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("CALL", final);
}

//---------------------------------------MAIN-----------------------------------------------------//

void gen_main(ht_table_t *table){
    bool inFunction = false;
    int curly = 0;
    int i = 0;
    char index[MAX_HT_SIZE];

    PRINT_LANE_ONE_ARG("LABEL", "$main");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    while(!0){
        element* e = NULL;
        sprintf(index, "%d", i++);
        e = ht_get(table, index);
        if(e == NULL){break;}
        if(e->name.type == IDENTIFIER){
            if(e->ret_type.type == ERROR_T) {
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
            if(inFunction == false){
                printf("%s\n", e->name.info);
            }
        }
    }
}

char *retype_arg_for_func(arg arg){
    char *final_arg = NULL;
    char tmpC[MAX_HT_SIZE];
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
        sprintf(final_arg, "float@%a", arg.arg.info);

    }else if(arg.arg.type == STRING){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "string@");
        memmove(arg.arg.info, arg.arg.info-1, strlen(arg.arg.info));
        memmove(arg.arg.info, arg.arg.info+2, strlen(arg.arg.info));
        strcat(final_arg, arg.arg.info);
    }else if(arg.arg.type == VAR_ID){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "TF@");
        memmove(arg.arg.info, arg.arg.info+1, strlen(arg.arg.info));
        strcat(final_arg, arg.arg.info);
    }
    return final_arg;
}

//---------------------------------------BUILD-IN FUNCTINOS---------------------------------------//

void func_reads(){
    printf("\n#ZACALA FUNKCIA READS !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$reads");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "TF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "string");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@tmp");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readi(){
    printf("\n#ZACALA FUNKCIA READI !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readi");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "TF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "int");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@tmp");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readf(){
    printf("\n#ZACALA FUNKCIA READF !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readf");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "TF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "float");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@tmp");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack to co chceme precitat
void func_write(){
    printf("\n#ZACALA FUNKCIA WRITE !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$write");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "TF@tmp");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@type_of_tmp");
    PRINT_LANE_TWO_ARG("TYPE", "TF@type_of_tmp", "TF@tmp");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$writenull", "TF@type_of_tmp", "string@nil" );

    PRINT_LANE_ONE_ARG("WRITE", "TF@tmp");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$writenull");
    PRINT_LANE_ONE_ARG("WRITE", "nil@nil");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack treba dat INT a funkcia vrati FLOAT
void func_floatval(){
    printf("\n#ZACALA FUNKCIA FLOATVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$floatval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@number");
    PRINT_LANE_ONE_ARG("POPS", "TF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return_float");

    PRINT_LANE_TWO_ARG("INT2FLOAT","TF@return_float", "TF@number");

    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_float");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack treba dat FLOAT a funckia vrati INT
void func_intval(){
    printf("\n#ZACALA FUNKCIA INTVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$intval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@number");
    PRINT_LANE_ONE_ARG("POPS", "TF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return_int");

    PRINT_LANE_TWO_ARG("FLOAT2INT","TF@return_int", "TF@number");

    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack string a ona vrati bud string alebo null string
void func_strval(){
    printf("\n#ZACALA FUNKCIA STRVAL !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$strval");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@string");
    PRINT_LANE_ONE_ARG("POPS", "TF@string");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@string", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@string");
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

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return_val");
    PRINT_LANE_ONE_ARG("POPS", "TF@return_val");
    PRINT_LANE_TWO_ARG("STRLEN", "TF@return_val", "TF@return_val");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_val");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack v poradi treba dat ->string, start_index (i), end_index (j) ... funkcia vrati string medzi tymito indexami
void func_substring(){
    printf("\n#ZACALA FUNKCIA SUBSTRING !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$func_strlen");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@end_index");
    PRINT_LANE_ONE_ARG("POPS", "TF@end_index");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@start_index");
    PRINT_LANE_ONE_ARG("POPS", "TF@start_index");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@string");
    PRINT_LANE_ONE_ARG("POPS", "TF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@strlen_string");
    PRINT_LANE_TWO_ARG("STRLEN", "TF@strlen_string", "TF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return");

    //j<0
    PRINT_LANE_TWO_ARG("MOVE", "TF@return", "bool@false");
    PRINT_LANE_THREE_ARG("LT", "TF@return", "TF@end_index", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@return", "bool@true");

    //i<0
    PRINT_LANE_THREE_ARG("LT", "TF@return", "TF@start_index", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@return", "bool@true");

    //to do kontrola ci j > strlen
    PRINT_LANE_THREE_ARG("GT", "TF@return", "TF@end_index", "TF@strlen_string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@return", "bool@true");

    //to do kontrola ci i > strlen
    PRINT_LANE_THREE_ARG("GT", "TF@return", "TF@start_index", "TF@strlen_string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@return", "bool@true");

    // i > j
    PRINT_LANE_THREE_ARG("SUB", "TF@end_index" , "TF@end_index", "int@1"); //j-1
    PRINT_LANE_THREE_ARG("GT", "TF@return", "TF@start_index", "TF@end_index");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@return", "bool@true");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp_string");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return_string");
    PRINT_LANE_TWO_ARG("MOVE", "TF@return_string", "string@");

    PRINT_LANE_ONE_ARG("LABEL", "$start_while");
    PRINT_LANE_THREE_ARG("LT", "TF@return", "TF@end_index", "TF@start_index"); //j<i
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end_while", "LF@return", "bool@true");

    PRINT_LANE_THREE_ARG("GETCHAR", "TF@tmp_string", "TF@string", "TF@start_index");
    PRINT_LANE_THREE_ARG("CONCAT", "TF@return_string","TF@return_string", "LF@tmp_string");

    PRINT_LANE_THREE_ARG("ADD", "TF@start_index", "TF@start_index", "int@1");
    PRINT_LANE_ONE_ARG("JUMP", "$start_while");

    PRINT_LANE_ONE_ARG("LABEL", "$end_while");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_string");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_TWO_ARG("MOVE", "TF@return", "nil@nil");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@return");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack string a funkia vracia INT hodnotu prveho znaku stringu
void func_ord(){
    printf("\n#ZACALA FUNKCIA ORD !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$ord");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@string");
    PRINT_LANE_ONE_ARG("POPS", "TF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@return_int");
    PRINT_LANE_TWO_ARG("MOVE", "TF@return_int", "int@0");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@strlen_string");
    PRINT_LANE_TWO_ARG("STRLEN", "TF@strlen_string", "TF@string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@strlen_string", "TF@return_int");

    PRINT_LANE_THREE_ARG("STRI2INT", "TF@return_int", "TF@string", "TF@return_int");

    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_int");
    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@return_int");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack treba dat INT a funkcia vrati charakter v prislunom indexe ASCII
void func_chr(){
    printf("\n#ZACALA FUNKCIA CHR !\n");
    PRINT_LANE_ONE_ARG("LABEL", "$chr");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@number");
    PRINT_LANE_ONE_ARG("POPS", "TF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "TF@result");

    PRINT_LANE_THREE_ARG("LT", "TF@result", "TF@number", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@result", "bool@true");

    PRINT_LANE_THREE_ARG("GT", "TF@result", "TF@number", "int@255");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "TF@result", "bool@true");

    PRINT_LANE_TWO_ARG("INT2CHAR", "TF@result", "TF@number");
    PRINT_LANE_ONE_ARG("PUSHS", "TF@result");
    PRINT_LANE_ONE_ARG("LABEL", "$end");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}