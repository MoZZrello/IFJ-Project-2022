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

//Vstavane funkcie

void func_reads(){

}

    //do @strlen_ret_val string a potom tam vratime int hodnotu
void func_strlen(){
    PRINT_LANE_ONE_ARG("LABEL", "$func_streln");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_val");
    PRINT_LANE_ONE_ARG("POPS", "LF@return_val");
    PRINT_LANE_TWO_ARG("STRLEN", "LF@return_val", "LF@return_val");
    PRINT_LANE_ONE_ARG("PUSHS", "LF@return_val");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_floatval(){
    PRINT_LANE_ONE_ARG("LABEL", "int2float");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");
    PRINT_LANE_TWO_ARG("INT2FLOAT","LF@return_int", "LF@number");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}

void func_intval(){
    PRINT_LANE_ONE_ARG("LABEL", "float2int");
    PRINT_LANE_ZERO_ARG("CREATEFRAME");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@number");
    PRINT_LANE_ONE_ARG("DEFVAR", "LF@return_int");
    PRINT_LANE_TWO_ARG("FLOAT2INT","LF@return_int", "LF@number");
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}


