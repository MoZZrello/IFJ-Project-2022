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
void gen_program(ht_table_t *table, int key){
    start_program();
    gen_function(table, key);
    gen_main(table, key);
}

void start_program(){
    PRINT_LANE_ZERO_ARG(".IFJcode22");
    PRINT_LANE_ONE_ARG("JUMP", "$main");
}

//funckia na generovanie celej funkcii
void gen_function(ht_table_t *table, int key){
    bool is_function = false; // zistenie ci mame funkciu ak ano printime ak nie nic sa nestane,
    bool is_end = false;  //is_end ak sa dostaneme na koniec funkcie prestaneme print == }

    for(int i = 0; i < key; i++){
        char func[MAX_HT_SIZE];
        sprintf(func, "%d", i);
        element* e = ht_get(table, func);

        if(e->ret_type.type != ERROR_T){
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
            PRINT_LANE_ZERO_ARG("POPFRAME");
            PRINT_LANE_ZERO_ARG("RETURN");
            is_end = true;
        }
        //printujem telo funkcie
        if(is_function && is_end == false){
                def_func_main_print(e);
        }
    }
    printf("\n");
}

//funckia na printenie zaciatku funkcie
void def_func_arg_print(element* e){
    printf("\n");
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    char final [500];
    snprintf(final,sizeof final, "$%s", e->name.info );
    PRINT_LANE_ONE_ARG("LABEL", final );
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    char final_var [500];
    for (int i = 0; i < e->argslist->len; ++i) {
        memmove(e->argslist->list[i].arg.info, e->argslist->list[i].arg.info+1, strlen(e->argslist->list[i].arg.info));
        snprintf(final_var,sizeof final_var, "TF@%s", e->argslist->list[i].arg.info );
        PRINT_LANE_ONE_ARG("DEFVAR", final_var );
        PRINT_LANE_ONE_ARG("POPS", final_var );
    }

}
//printujem telo medzi { a }
void def_func_main_print(element* e){
    /*
        if(strcmp(e->name.info , "write") == 0){
            func_write();
        }*/
    printf("%s\n", e->name.info);

}

void gen_main(ht_table_t *table, int key){
    PRINT_LANE_ONE_ARG("LABEL", "$main");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    char index[MAX_HT_SIZE];
    for(int i=0; i < key; i++){
        element* e = NULL;
        sprintf(index, "%d", i);
        e = ht_get(table, index);

        if(e->name.type == IDENTIFIER){
            if(strcmp(e->name.info, "if") == 0){

            } else if(strcmp(e->name.info, "while") == 0){

            } else if(strcmp(e->name.info, "else") == 0){

            } else if(e->ret_type.type == ERROR_T) { // function calls
                gen_call_arg(table, *e, key);
            }
        }
        //printf("%s\n", e->name.info);
    }
}

void gen_call_arg(ht_table_t *table, element call, int key){
    char index[MAX_HT_SIZE];
    element* compare_e = NULL;
    char* print = NULL;

    for(int i=0; i < key; i++){
        sprintf(index, "%d", i);
        compare_e = ht_get(table, index);
        if(compare_e == NULL) break;

        if (compare_e->ret_type.type != ERROR_T){
            if(strcmp(compare_e->name.info, call.name.info) == 0){
                for (int j = 0; j < call.argslist->len ; ++j) {
                    if(strcmp(call.argslist->list[j].arg.info , ",") != 0) {
                        print = retype_arg_for_func(call.argslist->list[j]);
                        PRINT_LANE_ONE_ARG("PUSHS", print );
                    }
                }
                char final [500];
                snprintf(final,sizeof final, "$%s", call.name.info );
                PRINT_LANE_ONE_ARG("CALL", final );
            }
        }
    }
}
//Vstavane funkcie

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
        strcat(final_arg, arg.arg.info);
    }else if(arg.arg.type == STRING){
        final_arg = malloc(sizeof (char) * (int)strlen(arg.arg.info) + 1);
        if(final_arg == NULL){
            callError(ERR_INTERNAL);
        }
        strcpy(final_arg, "string@");
        memmove(arg.arg.info, arg.arg.info-1, strlen(arg.arg.info));
        memmove(arg.arg.info, arg.arg.info+2, strlen(arg.arg.info));
        strcat(final_arg, arg.arg.info);
    }
    return final_arg;
}

void func_reads(){
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$reads");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "LF@tmp", "string");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@tmp");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readi(){
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readi");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "LF@tmp", "int");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@tmp");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readf(){
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$readf");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "LF@tmp", "float");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@tmp");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack to co chceme precitat
void func_write(){
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$write");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@type_of_tmp");
    PRINT_LANE_TWO_ARG("TYPE", "LF@type_of_tmp", "LF@tmp");

    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$writenull", "LF@type_of_tmp", "string@nil" );

    PRINT_LANE_ONE_ARG("WRITE", "LF@tmp");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");

    PRINT_LANE_ONE_ARG("LABEL", "$writenull");
    PRINT_LANE_ONE_ARG("WRITE", "nil@nil");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

//na stack treba dat INT a funkcia vrati FLOAT
void func_floatval(){
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$floatval");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$intval");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$strval");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$strlen");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$func_strlen");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$ord");
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
    printf("#ZACALA NOVA FUNKCIA !!!!!\n");
    PRINT_LANE_ONE_ARG("LABEL", "$chr");
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