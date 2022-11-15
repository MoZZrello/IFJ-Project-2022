//
// Created by Richard Harman on 2. 10. 2022.
//

#include "scanner.h"
#include "strings.h"

AutomatStates nextState(AutomatStates input, char c){
    switch(input){
        case Start:
            if(isspace(c)) return Start;
            if(isalpha(c)) return S_Identifier;
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
            if(c == '"' || c == 39) return S_String_0;
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
            return S_Comment_ML;
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
            if(c == '"' || c == 39) return S_String_1;
            if(c == '\\') return S_Escape_0;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_String_1:
            return ERROR;
        case S_Escape_0:
            if(c == '\\' || c == '"' || c == 'n' || c == 't' || c == '$' || c == 39) return S_Escape_1;
            if(c == 'x') return S_Hex_0;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 3)) return S_Octal_0;
            return S_String_0;
        case S_Escape_1:
            if(c == '"' || c == 39) return S_String_1;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_Octal_0:
            if(c == '"' || c == 39) return S_String_1;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 7)) return S_Octal_0;
            return ERROR;
        case S_Octal_1:
            if(c == '"' || c == 39) return S_String_1;
            if(isdigit(c) && ((c - '0') >= 0 && (c - '0') <= 7)) return S_Octal_0;
            return ERROR;
        case S_Octal_2:
            if(c == '"' || c == 39) return S_String_1;
            if((c > 31 && c != '$') || isspace(c)) return S_String_0;
            return ERROR;
        case S_Hex_0:
            if(c == '"' || c == 39) return S_String_1;
            if(isalnum(c)) return S_Hex_1;
            return ERROR;
        case S_Hex_1:
            if(c == '"' || c == 39) return S_String_1;
            if(isalnum(c)) return S_Hex_2;
            return ERROR;
        case S_Hex_2:
            if(c == '"' || c == 39) return S_String_1;
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
            return ERROR;
        case S_PHP_END:
            return ERROR;
        case ERROR:
            fprintf(stderr, "Program Error\n");
            return ERROR;
    }
}

Token returnTokenCreator(AutomatStates final_state, string* str) {
    tmp_token = (Token *) calloc(1, sizeof(Token));
    if(tmp_token == NULL){
        fprintf(stderr, "Memory didn't allocate!\n");
        return (Token){.type=ERROR_T, .info="Memory didn't allocate."};
    }
    tmp_token->kwt = keywordCheck(str);
    if (tmp_token->kwt != UNKNOWN_K){
        tmp_token->isKeyword = true;
    } else {
        tmp_token->isKeyword = false;
    }
    tmp_token->info = str->data;

    switch(final_state){
        case S_EOF:
            tmp_token->type = EOF_T;
            return *tmp_token;
        case S_Dot:
            tmp_token->type = DOT;
            return *tmp_token;
        case S_DoubleDot:
            tmp_token->type = DOUBLE_DOT;
            return *tmp_token;
        case S_Left_Bracket:
            tmp_token->type = LEFT_BRACKET;
            return *tmp_token;
        case S_Right_Bracket:
            tmp_token->type = RIGHT_BRACKET;
            return *tmp_token;
        case S_Left_Curly_Bracket:
            tmp_token->type = LEFT_CURLY_BRACKET;
            return *tmp_token;
        case S_Right_Curly_Bracket:
            tmp_token->type = RIGHT_CURLY_BRACKET;
            return *tmp_token;
        case S_Semicolon:
            tmp_token->type = SEMICOLON;
            return *tmp_token;
        case S_Plus:
            tmp_token->type = PLUS;
            return *tmp_token;
        case S_Comma:
            tmp_token->type = COMMA;
            return *tmp_token;
        case S_Multiply:
            tmp_token->type = MULTIPLY;
            return *tmp_token;
        case S_Minus:
            tmp_token->type = MINUS;
            return *tmp_token;
        case S_Number:
            tmp_token->type = NUMBER;
            return *tmp_token;
        case S_Decimal_1:
            tmp_token->type = DECIMAL_NUMBER;
            return *tmp_token;
        case S_Exponent_1:
            tmp_token->type = EXPONENT_NUMBER;
            return *tmp_token;
        case S_Divide:
            tmp_token->type = DIVIDE;
            return *tmp_token;
        case S_Question_Mark:
            tmp_token->type = QUESTION_MARK;
            return *tmp_token;
        case S_PHP_END:
            tmp_token->type = PHP_END;
            return *tmp_token;
        case S_Var:
            tmp_token->type = VAR_ID;
            return *tmp_token;
        case S_Not_Equal_1:
            tmp_token->type = NOT_EQUAL;
            return *tmp_token;
        case S_Equal_1:
            tmp_token->type = EQUAL;
            return *tmp_token;
        case S_Identifier:
            tmp_token->type = IDENTIFIER;
            return *tmp_token;
        case  S_Greater:
            tmp_token->type = GREATER;
            return *tmp_token;
        case S_Greater_Equal:
            tmp_token->type = GREATER_EQUAL;
            return *tmp_token;
        case S_Less:
            tmp_token->type = LESS;
            return *tmp_token;
        case S_Assign:
            tmp_token->type = ASSIGN;
            return *tmp_token;
        case S_Less_Equal:
            tmp_token->type = LESS_EQUAL;
            return *tmp_token;
        case S_PHP_4:
            tmp_token->type = PHP;
            return *tmp_token;
        case S_String_1:
            tmp_token->type = STRING;
            return *tmp_token;
        default:
            tmp_token->type = ERROR_T;
            return *tmp_token;
    }
}

Token getToken(string str){
    Token return_token;
    AutomatStates current_state = Start;
    while(1) {
        int c = getchar();
        if(c == EOF){
            if(current_state == Start){
                str.data = "EOF";
                return returnTokenCreator(S_EOF, &str);
            }else{
                return_token = returnTokenCreator(current_state, &str);
                return return_token;
            }
        }

        AutomatStates next_state = nextState(current_state, (char)c);
        if(next_state == ERROR){
            ungetc(c, stdin);
            return_token = returnTokenCreator(current_state, &str);
            return return_token;
        }

        if(current_state != S_Comment_ML && current_state != S_Comment_SL){
            addChar(&str, (char)c);
        }

        if(next_state == Start){
            stringClear(&str);
        }

        current_state = next_state;
    }
}

char* getTypeName(Token t){
    switch(t.type){
        case VAR_ID:
            return "|VAR_ID|";
        case IDENTIFIER:
            return "|IDENTIFIER|";
        case STRING:
            return "|STRING|";
        case EOF_T:
            return "|EOF|";
        case LEFT_BRACKET:
            return "|LEFT BRACKET|";
        case RIGHT_BRACKET:
            return "|RIGHT BRACKET|";
        case LEFT_CURLY_BRACKET:
            return "|LEFT CURLY BRACKET|";
        case RIGHT_CURLY_BRACKET:
            return "|RIGHT CURLY BRACKET|";
        case SEMICOLON:
            return "|SEMICOLON|";
        case PLUS:
            return "|PLUS|";
        case MINUS:
            return "|MINUS|";
        case COMMA:
            return "|COMMA|";
        case MULTIPLY:
            return "|MULTIPLY|";
        case DOT:
            return "|DOT|";
        case DOUBLE_DOT:
            return "|DOUBLE DOT|";
        case QUESTION_MARK:
            return "|QUESTION MARK|";
        case DIVIDE:
            return "|DIVIDE|";
        case NUMBER:
            return "|NUMBER|";
        case DECIMAL_NUMBER:
            return "|DECIMAL NUMBER|";
        case EXPONENT_NUMBER:
            return "|EXPONENT NUMBER|";
        case GREATER:
            return "|GREATER|";
        case GREATER_EQUAL:
            return "|GREATER EQUAL|";
        case LESS:
            return "|LESS|";
        case LESS_EQUAL:
            return "|LESS EQUAL|";
        case ASSIGN:
            return "|ASSIGN|";
        case EQUAL:
            return "|EQUAL|";
        case NOT_EQUAL:
            return "|NOT EQUAL|";
        case COMMENT:
            return "|COMMENT|";
        case PHP:
            return "|PHP|";
        case PHP_END:
            return "|PHP END|";
        case ERROR_T:
            return "|ERROR|";
        default:
            return "!UNKNOWN!";
    }
}

char* getKeywordTypeName(Token t){
    switch(t.kwt){
        case ELSE_K:
            return "|KW - ELSE|";
        case FLOAT_K:
            return "|KW - FLOAT|";
        case FUNCTION_K:
            return "|KW - FUNCTION|";
        case IF_K:
            return "|KW - IF|";
        case INT_K:
            return "|KW - INT|";
        case NULL_K:
            return "|KW - NULL|";
        case RETURN_K:
            return "|KW - RETURN|";
        case STRING_K:
            return "|KW - STRING|";
        case VOID_K:
            return "|KW - VOID|";
        default:
            return "";
    }
}

void tokenFree(){
    free(tmp_token->info);
    free(tmp_token);
}
