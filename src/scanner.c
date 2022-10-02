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
            if(isspace(c)) return Start;
            if(isalpha(c) || c == '_') return S_Identifier;
            if(isdigit(c)) return S_Number;
            if(c == '-') return S_Minus;
            if(c == '*') return S_Multiply;
            if(c == ',') return S_Comma;
            if(c == '+') return S_Plus;
            if(c == ';') return S_Semicolon;
            if(c == '}') return S_Right_Curly_Bracket;
            if(c == '{') return S_Left_Curly_Bracket;
            if(c == ')') return S_Right_Bracket;
            if(c == '(') return S_Left_Bracket;
            if(c == '.') return S_Dot;
            if(c == '/') return S_Divide;
            if(c == '?') return S_Question_Mark;
            if(c == '$') return S_Dollar;
            if(c == '!') return S_Exclamation;
            if(c == '=') return S_Minus;
            if(c == '-') return S_Assign;
            if(c == '>') return S_Greater;
            if(c == '<') return S_Less;
            if(c == '"') return S_String_0;
            return ERROR;
        case S_EOF:
            return ERROR;
        case S_Dot:
            return ERROR;
        case S_Left_Bracket:
            return ERROR;
        case S_Right_Bracket:
            return ERROR;
        case S_Left_Curly_Bracket:
            return ERROR;
        case S_Right_Curly_Bracket:
            return ERROR;
        case S_Semicolon:
            return ERROR;
        case S_Plus:
            return ERROR;
        case S_Comma:
            return ERROR;
        case S_Multiply:
            return ERROR;
        case S_Minus:
            return ERROR;
        case S_Question_Mark:
            if(c == '>') return S_PHP_END;
            return ERROR;
        case S_Number:
            if(isdigit(c)) return S_Number;
            if(c == '.') return S_Decimal_0;
            if(c == 'e' || c == 'E') return S_Exponent_0;
            return ERROR;
        case S_Exponent_0:
            if(isdigit(c)) return S_Exponent_1;
            return ERROR;
        case S_Exponent_1:
            if(isdigit(c)) return S_Exponent_1;
            return ERROR;
        case S_Decimal_0:
            if(isdigit(c)) return S_Decimal_1;
            return ERROR;
        case S_Decimal_1:
            if(isdigit(c)) return S_Decimal_1;
            if(c == 'e' || c == 'E') return S_Exponent_0;
            return ERROR;
        case S_Divide:
            if(c == '/') return S_Comment;
            return ERROR;
        case S_Comment:
            if(c == '\n') return Start;
            if(c != '\n') return S_Comment;
        case S_Dollar:
            if(isalnum(c)) return S_Var;
            return ERROR;
        case S_Var:
            if(isalnum(c) || c == '_') return S_Var;
            return ERROR;
        case  S_Exclamation:
            if(c == '=') return S_Not_Equal_0;
            return ERROR;
        case S_Not_Equal_0:
            if(c == '=') return S_Not_Equal_1;
            return ERROR;
        case S_Not_Equal_1:
            return ERROR;
        case S_Assign:
            if(c == '=') return S_Equal_0;
            return ERROR;
        case S_Equal_0:
            if(c == '=') return S_Equal_1;
            return ERROR;
        case S_Equal_1:
            return ERROR;
        case S_Identifier:
            if(isalnum(c) || c == '_') return S_Identifier;
            return ERROR;
        case S_Greater:
            if(c == '=') return S_Greater_Equal;
            return ERROR;
        case S_Greater_Equal:
            return ERROR;
        case S_Less:
            if(c == '?') return S_PHP_0;
            if(c == '=') return S_Less_Equal;
            return ERROR;
        case S_Less_Equal:
            return ERROR;
        case S_String_0:
            if((int)c > 31 && c != '$') return S_String_0;
            if(c == '\\') return S_Escape_0;
            if(c == '"') return S_String_1;
        case S_String_1:
            return ERROR;
        case S_Escape_0:
            if(c == '\\' || c == '"' || c == 'n' || c == 't' || c == '$') return S_Escape_1;
            if(c == 'x') return S_Hex_0;
            if(c == '"') return S_String_1;
            if(isdigit(c) && (atoi(c) >= 0 && atoi(c) <= 3)) return S_Octal_0;
            return ERROR;
        case S_Escape_1:
            if((int)c > 31 && c != '$') return S_String_0;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Octal_0:
            if(isdigit(c) && (atoi(c) >= 0 && atoi(c) <= 7)) return S_Octal_0;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Octal_1:
            if(isdigit(c) && (atoi(c) >= 0 && atoi(c) <= 7)) return S_Octal_0;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Octal_2:
            if((int)c > 31 && c != '$') return S_String_0;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Hex_0:
            if(isalnum(c)) return S_Hex_1;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Hex_1:
            if(isalnum(c)) return S_Hex_2;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_Hex_2:
            if((int)c > 31 && c != '$') return S_String_0;
            if(c == '"') return S_String_1;
            return ERROR;
        case S_PHP_0:
            if(c == 'p') return S_PHP_1;
            return ERROR;
        case S_PHP_1:
            if(c == 'h') return S_PHP_2;
            return ERROR;
        case S_PHP_2:
            if(c == 'p') return S_PHP_3;
            return ERROR;
        case S_PHP_3:
            if(isspace(c)) return S_PHP_4;
            return ERROR;
        case S_PHP_4:
            if(c == 'd') return S_PHP_5;
            return ERROR;
        case S_PHP_5:
            if(c == 'e') return S_PHP_6;
            return ERROR;
        case S_PHP_6:
            if(c == 'c') return S_PHP_7;
            return ERROR;
        case S_PHP_7:
            if(c == 'l') return S_PHP_8;
            return ERROR;
        case S_PHP_8:
            if(c == 'a') return S_PHP_9;
            return ERROR;
        case S_PHP_9:
            if(c == 'r') return S_PHP_10;
            return ERROR;
        case S_PHP_10:
            if(c == 'e') return S_PHP_11;
            return ERROR;
        case S_PHP_11:
            if(c == '(') return S_PHP_12;
            return ERROR;
        case S_PHP_12:
            if(c == 's') return S_PHP_13;
            return ERROR;
        case S_PHP_13:
            if(c == 't') return S_PHP_14;
            return ERROR;
        case S_PHP_14:
            if(c == 'r') return S_PHP_15;
            return ERROR;
        case S_PHP_15:
            if(c == 'i') return S_PHP_16;
            return ERROR;
        case S_PHP_16:
            if(c == 'c') return S_PHP_17;
            return ERROR;
        case S_PHP_17:
            if(c == 't') return S_PHP_18;
            return ERROR;
        case S_PHP_18:
            if(c == '_') return S_PHP_19;
            return ERROR;
        case S_PHP_19:
            if(c == 't') return S_PHP_20;
            return ERROR;
        case S_PHP_20:
            if(c == 'y') return S_PHP_21;
            return ERROR;
        case S_PHP_21:
            if(c == 'p') return S_PHP_22;
            return ERROR;
        case S_PHP_22:
            if(c == 'e') return S_PHP_23;
            return ERROR;
        case S_PHP_23:
            if(c == 's') return S_PHP_24;
            return ERROR;
        case S_PHP_24:
            if(c == '=') return S_PHP_25;
            return ERROR;
        case S_PHP_25:
            if(c == '1') return S_PHP_26;
            return ERROR;
        case S_PHP_26:
            if(c == ')') return S_PHP_27;
            return ERROR;
        case S_PHP_27:
            if(c == ';') return S_PHP_28;
            return ERROR;
        case S_PHP_28:
            return ERROR;
        case S_PHP_END:
            return ERROR;
        case ERROR:
            fprintf(stderr, "Program Error\n");
            return ERROR;
    }
}

Token selectToken(AutomatStates final) {

}

void getToken(){
    AutomatStates current_state = Start;
    while(1) {
        int c = getchar();
        if(c == EOF){
            return;
        }
        AutomatStates next_state = nextState(current_state, c);
        if(next == ERROR){
            return ERROR_T;
        }
        current_state = next_state;
    }
}

int main(int argc, char* argv[]){
    getToken();
    return 0;
}