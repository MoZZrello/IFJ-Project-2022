//
// Created by Richard Harman on 2. 10. 2022.
//

#include "scanner.h"
#include "ctype.h"
#include "stdio.h"
#include "stdbool.h"

AutomatStates nextState(AutomatStates input, char c){
    switch(input){
        case Start:
        case S_EOF:
        case S_Dot:
        case S_Left_Bracket:
        case S_Right_Bracket:
        case S_Left_Curly_Bracket:
        case S_Right_Curly_Bracket:
        case S_Semicolon:
        case S_Plus:
        case S_Comma:
        case S_Multiply:
        case S_Minus:
        case S_Question_Mark:
        case S_Number:
        case S_Exponent_0:
        case S_Exponent_1:
        case S_Decimal_0:
        case S_Decimal_1:
        case S_Divide:
        case S_Comment:
        case S_Dollar:
        case S_Var:
        case  S_Exclamation:
        case S_Not_Equal_0:
        case S_Not_Equal_1:
        case S_Assign:
        case S_Equal_0:
        case S_Equal_1:
        case S_Identifier:
        case S_Greater:
        case S_Greater_Equal:
        case S_Less:
        case S_Less_Equal:
        case S_String_0:
        case S_String_1:
        case S_Escape_0:
        case S_Escape_1:
        case S_Octal_0:
        case S_Octal_1:
        case S_Octal_2:
        case S_Hex_0:
        case S_Hex_1:
        case S_Hex_2:
        case S_PHP_0:
        case S_PHP_1:
        case S_PHP_2:
        case S_PHP_3:
        case S_PHP_4:
        case S_PHP_5:
        case S_PHP_6:
        case S_PHP_7:
        case S_PHP_8:
        case S_PHP_9:
        case S_PHP_10:
        case S_PHP_11:
        case S_PHP_12:
        case S_PHP_13:
        case S_PHP_14:
        case S_PHP_15:
        case S_PHP_16:
        case S_PHP_17:
        case S_PHP_18:
        case S_PHP_19:
        case S_PHP_20:
        case S_PHP_21:
        case S_PHP_22:
        case S_PHP_23:
        case S_PHP_24:
        case S_PHP_25:
        case S_PHP_26:
        case S_PHP_27:
        case S_PHP_28:
        case ERROR:
            fprintf(stderr, "Program Error\n");
            return ERROR;
    }
}

Token selectToken(AutomatStates final) {

}

void getToken(){
    AutomatStates current_state = Start;
    int c;
    while(1) {
        int c = getchar();
        if(c == EOF){
            return;
        }
        printf("%c", c);
        /*AutomatStates next_state = nextState(current_state, c);
        if(next == ERROR){
            return ERROR_T;
        }
        current_state = next_state;*/
    }
}

int main(int argc, char* argv[]){
    getToken();
    return 0;
}