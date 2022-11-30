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

void func_reads(){
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





