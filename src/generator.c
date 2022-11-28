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

void func_strlen(){
    PRINT_LANE_ONE_ARG("LABEL", "$func_streln");
    PRINT_LANE_ZERO_ARG("PUSHFRAME");
    char* return_val = "LF@return_val";
    PRINT_LANE_ONE_ARG("DEFVAR", return_val);
    PRINT_LANE_ONE_ARG("POPS", return_val);
    PRINT_LANE_TWO_ARG("STRLEN", return_val, return_val);
    PRINT_LANE_ONE_ARG("PUSHS", return_val);
    PRINT_LANE_ZERO_ARG("POPFRAME");
    PRINT_LANE_ZERO_ARG("RETURN");
}



