//
// Created by Marek Spirka on 27. 11. 2022.
//
#include "generator.h"

void start_program(){
    PRINT_LANE_ZERO_ARG(".IFJcode22 \n");
}

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

// Instructions generator



//Vstavane funkcie
    //do @strlen_ret_val string a potom tam vratime int hodnotu
void func_strlen(){
    PRINT_LANE_ONE_ARG("LABEL", "$func_streln");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_val");
    PRINT_LANE_ONE_ARG("POPS", "LF@return_val");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@return_val", "LF@return_val");
   // PRINT_LANE_ONE_ARG("PUSHS", "LF@return_val");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}
    //na stack treba dat jednu hodnotu ktoru menime
void func_floatval(){
    PRINT_LANE_ONE_ARG("LABEL", "$int2float");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@numbert");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");

    PRINT_LANE_TWO_ARG("INT2FLOAT","LF@return_int", "LF@number");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

    //na stack treba dat jednu hodnotu ktoru menime
void func_intval(){
    PRINT_LANE_ONE_ARG("LABEL", "$float2int");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@numbert");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_float");

    PRINT_LANE_TWO_ARG("FLOAT2INT","LF@return_float", "LF@number");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

    //do number sa nacita zo stacku cislo, ktore dame do char
void func_chr(){
    PRINT_LANE_ONE_ARG("LABEL", "$ord");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@numbert");
    PRINT_LANE_ONE_ARG("POPS", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@result");

    PRINT_LANE_THREE_ARG("LT", "LF@result", "LF@number", "int@0");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@result", "bool@true");

    PRINT_LANE_THREE_ARG("GT", "LF@result", "LF@number", "int@255");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@result", "bool@true");

    PRINT_LANE_TWO_ARG("INT2CHAR", "LF@result", "LF@number");
    PRINT_LANE_ONE_ARG("LABEL", "$end");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

    //najrpv treba pushnut string
void func_ord(){
    PRINT_LANE_ONE_ARG("LABEL", "$chr");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@string");
    PRINT_LANE_ONE_ARG("POPS", "LF@string");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_TWO_ARG("MOVE", "LF@number", "int@0");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");
    PRINT_LANE_TWO_ARG("MOVE", "LF@return_int", "nil@nil");

    PRINT_LANE_ONE_ARG("DEFVAR", "LF@strlen_string");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@strlen_string", "LF@string");
    PRINT_LANE_THREE_ARG("JUMPIFEQ", "$end", "LF@strlen_string", "LF@number");

    PRINT_LANE_THREE_ARG("STRI2INT", "LF@return_int", "LF@string", "LF@number");

    PRINT_LANE_ONE_ARG("LABEL", "$end");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_reads(){
    PRINT_LANE_ONE_ARG("LABEL", "$reads");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "string");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readi(){
    PRINT_LANE_ONE_ARG("LABEL", "$readi");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "int");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_readf(){
    PRINT_LANE_ONE_ARG("LABEL", "$readf");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");

    PRINT_LANE_ONE_ARG("DEFVAR", "TF@tmp");
    PRINT_LANE_ONE_ARG("POPS", "LF@tmp");

    PRINT_LANE_TWO_ARG("READ", "TF@tmp", "float");

    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}





