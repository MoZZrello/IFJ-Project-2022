//
// Created by Richard Harman on 2. 10. 2022.
//

#include "scanner.h"

Token* tmp;

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
            if(c == '=') return S_Assign;
            if(c == '>') return S_Greater;
            if(c == ':') return S_DoubleDot;
            if(c == '<') return S_Less;
            if(c == '"') return S_String_0;
            return ERROR;
        case S_EOF:
            return ERROR;
        case S_Dot:
            return ERROR;
        case S_DoubleDot:
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
            if(c == '/') return S_Comment_SL;
            if(c == '*') return S_Comment_ML;
            return ERROR;
        case S_Comment_SL:
            if(c == '\n') return Start;
            if(c != '\n') return S_Comment_SL;
        case S_Comment_ML:
            if(c == '*') return S_Comment_ML_End;
            if(c != '*') return S_Comment_ML;
        case S_Comment_ML_End:
            if(c == '/') return Start;
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
            if(c == '"') return S_String_1;
            if(c == '\\') return S_Escape_0;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_String_1:
            return ERROR;
        case S_Escape_0:
            if(c == '\\' || c == '"' || c == 'n' || c == 't' || c == '$') return S_Escape_1;
            if(c == 'x') return S_Hex_0;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 3)) return S_Octal_0;
            return ERROR;
        case S_Escape_1:
            if(c == '"') return S_String_1;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_Octal_0:
            if(c == '"') return S_String_1;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 7)) return S_Octal_0;
            return ERROR;
        case S_Octal_1:
            if(c == '"') return S_String_1;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 7)) return S_Octal_0;
            return ERROR;
        case S_Octal_2:
            if(c == '"') return S_String_1;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_Hex_0:
            if(c == '"') return S_String_1;
            if(isalnum(c)) return S_Hex_1;
            return ERROR;
        case S_Hex_1:
            if(c == '"') return S_String_1;
            if(isalnum(c)) return S_Hex_2;
            return ERROR;
        case S_Hex_2:
            if(c == '"') return S_String_1;
            if(c > 31 && c != '$') return S_String_0;
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

Token returnTokenCreator(AutomatStates final_state, char* token_msg) {
    tmp = malloc(sizeof(Token));
    if(tmp == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        free(tmp);
        return (Token){.type=ERROR_T, .info="Memory didn't allocate."};
    }
    tmp->info = calloc(strlen(token_msg), sizeof(char));
    if(tmp->info == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        free(tmp->info);
        return (Token){.type=ERROR_T, .info="Memory didn't allocate."};
    }
    tmp->info = token_msg;
    switch(final_state){
        case S_EOF:
            printf("|FINAL EOF|\n");
            tmp->type = EOF_T;
            return *tmp;
        case S_Dot:
            printf("|FINAL DOT|\n");
            tmp->type = DOT;
            return *tmp;
        case S_DoubleDot:
            printf("|FINAL DOUBLEDOT|\n");
            tmp->type = DOUBLEDOT;
            return *tmp;
        case S_Left_Bracket:
            printf("|FINAL LEFT_BRACKET|\n");
            tmp->type = LEFT_BRACKET;
            return *tmp;
        case S_Right_Bracket:
            printf("|FINAL RIGHT_BRACKET|\n");
            tmp->type = RIGHT_BRACKET;
            return *tmp;
        case S_Left_Curly_Bracket:
            printf("|FINAL LEFT_CURLY_BRACKET|\n");
            tmp->type = LEFT_CURLY_BRACKET;
            return *tmp;
        case S_Right_Curly_Bracket:
            printf("|FINAL RIGHT_CURLY_BRACKET|\n");
            tmp->type = RIGHT_CURLY_BRACKET;
            return *tmp;
        case S_Semicolon:
            printf("|FINAL SEMICOLON|\n");
            tmp->type = SEMICOLON;
            return *tmp;
        case S_Plus:
            printf("|FINAL PLUS|\n");
            tmp->type = PLUS;
            return *tmp;
        case S_Comma:
            printf("|FINAL COMMA|\n");
            tmp->type = COMMA;
            return *tmp;
        case S_Multiply:
            printf("|FINAL MULTIPLY|\n");
            tmp->type = MULTIPLY;
            return *tmp;
        case S_Minus:
            printf("|FINAL MINUS|\n");
            tmp->type = MINUS;
            return *tmp;
        case S_Number:
            printf("|FINAL NUMBER|\n");
            tmp->type = NUMBER;
            return *tmp;
        case S_Decimal_1:
            printf("|FINAL DECIMAL|\n");
            tmp->type = DECIMAL_NUMBER;
            return *tmp;
        case S_Exponent_1:
            printf("|FINAL EXPONENT|\n");
            tmp->type = EXPONENT_NUMBER;
            return *tmp;
        case S_Divide:
            printf("|FINAL DIVIDE|\n");
            tmp->type = DIVIDE;
            return *tmp;
        case S_Question_Mark:
            printf("|FINAL ?|\n");
            tmp->type = QUESTION_MARK;
            return *tmp;
        case S_PHP_END:
            printf("|FINAL PHP_END|\n");
            tmp->type = PHP_END;
            return *tmp;
        case S_Var:
            printf("|FINAL VAR|\n");
            tmp->type = VAR_ID;
            return *tmp;
        case S_Not_Equal_1:
            printf("|FINAL NOT_EQUAL|\n");
            tmp->type = NOT_EQUAL;
            return *tmp;
        case S_Equal_1:
            printf("|FINAL EQUAL|\n");
            tmp->type = EQUAL;
            return *tmp;
        case S_Identifier:
            printf("|FINAL IDENTIFIER|\n");
            tmp->type = IDENTIFIER;
            return *tmp;
        case  S_Greater:
            printf("|FINAL GREATER|\n");
            tmp->type = GREATER;
            return *tmp;
        case S_Greater_Equal:
            printf("|FINAL GRETAER_EQUAL|\n");
            tmp->type = GREATER_EQUAL;
            return *tmp;
        case S_Less:
            printf("|FINAL LESS|\n");
            tmp->type = LESS;
            return *tmp;
        case S_Assign:
            printf("|FINAL ASSIGN|\n");
            tmp->type = ASSIGN;
            return *tmp;
        case S_Less_Equal:
            printf("|FINAL LESS_EQUAL|\n");
            tmp->type = LESS_EQUAL;
            return *tmp;
        case S_PHP_28:
            printf("|FINAL PHP_DECLARE|\n");
            tmp->type = PHP_DECLARE;
            return *tmp;
        case S_String_1:
            printf("|FINAL STRING|\n");
            tmp->type = STRING;
            return *tmp;
        case ERROR:
            printf("|FINAL ERROR|\n");
            tmp->type = ERROR_T;
            return *tmp;
        default:
            printf("!NEZNAMA!\n");
            tmp->type = ERROR_T;
            return *tmp;
    }
}

Token getToken(){
    Token return_token;
    AutomatStates current_state = Start;
    int index = 0;
    int size = 1;
    char *str = calloc(size, sizeof(char));
    if(str == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        free(str);
        return returnTokenCreator(ERROR, "Memory allocation error");
    }
    while(1) {
        int c = getchar();
        if(c == EOF){
            if(current_state == Start){
                free(str);
                return returnTokenCreator(S_EOF, "EOF on start");
            }else{
                return_token = returnTokenCreator(current_state, str);
                free(str);
                return return_token;
            }
        }

        if(current_state != S_Comment_ML || current_state != S_Comment_SL){
            str[index] = c;
            index++;
            if(size == index){
                size++;
                str = realloc(str, sizeof(char)*size);
                if(str == NULL){
                    fprintf(stderr, "Memory didn't allocate!\n");
                    return returnTokenCreator(ERROR, "Memory allocation error");
                }
            }
        }

        AutomatStates next_state = nextState(current_state, c);
        if(next_state == ERROR){
            str[index-1] = '\0';
            ungetc(c, stdin);
            return_token = returnTokenCreator(current_state, str);
            free(str);
            return return_token;
        }

        if(next_state == Start){
            index = 0;
            size = 1;
            free(str);
            str = calloc(size, sizeof(char));
        }

        current_state = next_state;
    }
}

int main(int argc, char* argv[]){
    printf("Starting program...\n");
    while(1){
        Token t = getToken();
        if(t.type == EOF_T){
            break;
        }
        printf("%s\n", t.info);
        free(tmp->info);
        free(tmp);
    }
    return 0;
}