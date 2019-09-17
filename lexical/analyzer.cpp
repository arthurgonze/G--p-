//
// Created by souzajbr on 10/09/2019.
//

#include "ctype.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../token.h"
#include "error.h"
#include "io.h"
#include "../SymbolTable.h"
#include "analyzer.h"

#define BUFFER_SIZE 32
#define INITIAL_STATE 0

int currentState = INITIAL_STATE;
int currentInput;
int currentLine = 1;
int currentColumn = 1;

char *lexemeBuffer = NULL; //TODO refatorar isso para gerar uma struct de saida
int lexemeLength = 0;
int lexemeBufferSize = 0;

struct SymbolTable reservedWordsTable;
struct SymbolTable literalsTable;
struct SymbolTable identifiersTable;
struct SymbolTable reservedWordsUsedTable;

struct SymbolTable get_literals_table() {
    return literalsTable;
}

struct SymbolTable get_identifiers_table() {
    return identifiersTable;
}

struct SymbolTable get_reserved_words_table() {
    return reservedWordsUsedTable;
}

void get_next_char() {

    if (lexemeLength == lexemeBufferSize) {
        lexemeBufferSize += BUFFER_SIZE;
        lexemeBuffer = (char *) realloc(lexemeBuffer, lexemeBufferSize);
    }

    lexemeBuffer[lexemeLength++] = (currentInput = io_get_next_char());
    currentColumn++;
}

void clear_lexeme() {

    memset(lexemeBuffer, 0, lexemeBufferSize);

    lexemeBuffer[0] = (char) currentInput;
    lexemeLength = 1;
}

void get_next_char_and_go_to(int state) {
    get_next_char();
    currentState = state;
}

void go_to_state(int state) {
    currentState = state;
}

void remove_last_char_from_lexeme() {
    lexemeBuffer[lexemeLength - 1] = '\0';
}

struct token_info found_token_and_restart(int token) {
    remove_last_char_from_lexeme();

    struct token_info info;

    info.token = token;

    info.lexeme = (char *) malloc(strlen(lexemeBuffer) + 1);
    strcpy(info.lexeme, lexemeBuffer);

    clear_lexeme();

    go_to_state(0);
    return info;
}

struct token_info found_token_and_get_next_input(int token) {
    get_next_char();
    return found_token_and_restart(token);
}

struct token_info found_token_and_check_for_reserved_word() {
    remove_last_char_from_lexeme();

    int token = reservedWordsTable.cSearch(lexemeBuffer);

    if(token >= 0) {
        reservedWordsUsedTable.cInsert(token, lexemeBuffer);
        return found_token_and_restart(token);
    }
    else {
        identifiersTable.cInsert(ID, lexemeBuffer);
        return found_token_and_restart(ID);
    }
}

struct token_info found_literal_and_restart(int token) {
    remove_last_char_from_lexeme();
    literalsTable.cInsert(token, lexemeBuffer);
    return found_token_and_restart(token);
}

void lexical_analyzer_init() {

    reservedWordsTable.cInsert(IF, "if");
    reservedWordsTable.cInsert(ELSE, "else");
    reservedWordsTable.cInsert(TRY, "try");
    reservedWordsTable.cInsert(CATCH, "catch");
    reservedWordsTable.cInsert(INT, "int");
    reservedWordsTable.cInsert(FLOAT, "float");
    reservedWordsTable.cInsert(BOOL, "bool");
    reservedWordsTable.cInsert(RETURN, "return");
    reservedWordsTable.cInsert(WHILE, "while");
    reservedWordsTable.cInsert(BREAK, "break");
    reservedWordsTable.cInsert(SWITCH, "switch");
    reservedWordsTable.cInsert(PRINT, "print");
    reservedWordsTable.cInsert(READLN, "readln");
    reservedWordsTable.cInsert(CASE, "case");
    reservedWordsTable.cInsert(THROW, "throw");
    reservedWordsTable.cInsert(TRUE, "true");
    reservedWordsTable.cInsert(FALSE, "false");
    reservedWordsTable.cInsert(CHAR, "char");
    reservedWordsTable.cInsert(TYPEDEF, "typedef");
    reservedWordsTable.cInsert(STRUCT, "struct");

    get_next_char();

}

bool is_letter(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

void fail(char *reason) {
    error_push(currentLine, currentColumn, reason);
    clear_lexeme();
}

void lexical_analyzer_dispose() {
    free(lexemeBuffer);
}


struct token_info lexical_analyzer_next_token() {

    while (true) {

        switch (currentState) {

            case INITIAL_STATE:
                if (currentInput == '\n') {
                    currentLine++;
                    currentColumn = 1;
                }

                if (isspace(currentInput)) {
                    get_next_char_and_go_to(INITIAL_STATE);
                    clear_lexeme();
                } else if (is_letter(currentInput)) {
                    get_next_char_and_go_to(13);
                } else if (is_digit(currentInput))
                    get_next_char_and_go_to(19);
                else
                    switch (currentInput) {
                        case '<':
                            get_next_char_and_go_to(1);
                            break;
                        case '=':
                            get_next_char_and_go_to(4);
                            break;
                        case '>':
                            get_next_char_and_go_to(5);
                            break;
                        case '!':
                            get_next_char_and_go_to(10);
                            break;
                        case '.':
                            get_next_char_and_go_to(14);
                            break;
                        case '+':
                            get_next_char_and_go_to(27);
                            break;
                        case ')':
                            get_next_char_and_go_to(28);
                            break;
                        case '-':
                            get_next_char_and_go_to(29);
                            break;
                        case '(':
                            get_next_char_and_go_to(30);
                            break;
                        case '{':
                            get_next_char_and_go_to(31);
                            break;
                        case '}':
                            get_next_char_and_go_to(32);
                            break;
                        case '[':
                            get_next_char_and_go_to(33);
                            break;
                        case ']':
                            get_next_char_and_go_to(34);
                            break;
                        case ',':
                            get_next_char_and_go_to(35);
                            break;
                        case ';':
                            get_next_char_and_go_to(36);
                            break;
                        case '"':
                            get_next_char_and_go_to(37);
                            break;
                        case '\'':
                            get_next_char_and_go_to(38);
                            break;
                        case '*':
                            get_next_char_and_go_to(39);
                            break;
                        case '|':
                            get_next_char_and_go_to(43);
                            break;
                        case '&':
                            get_next_char_and_go_to(46);
                            break;
                        case '/':
                            get_next_char_and_go_to(49);
                            break;
                        case EOF:
                        case ENDOFFILE:
                            go_to_state(54);
                            break;
                        default:
                            get_next_char_and_go_to(18); //TODO tratar isso
                    }
                break;
            case 1:
                switch (currentInput) {
                    case '=':
                        go_to_state(2);
                        break;
                    default:
                        go_to_state(3);
                        break;
                }
                break;
            case 2:
                return found_token_and_get_next_input(LE);
            case 3:
                return found_token_and_restart(LT);
            case 4:
                switch (currentInput) {
                    case '=':
                        get_next_char_and_go_to(6);
                        break;
                    default:
                        go_to_state(7);
                        break;
                }
                break;
            case 5:
                switch (currentInput) {
                    case '=':
                        get_next_char_and_go_to(8);
                        break;
                    default:
                        go_to_state(9);
                        break;
                }
                break;
            case 6:
                return found_token_and_get_next_input(EQ);
            case 7:
                return found_token_and_restart(ASSIGN);
            case 8:
                return found_token_and_get_next_input(GE);
            case 9:
                return found_token_and_restart(GT);
            case 10:
                switch (currentInput) {
                    case '=':
                        get_next_char_and_go_to(11);
                        break;
                    default:
                        go_to_state(12);
                        break;
                }
                break;
            case 11:
                return found_token_and_get_next_input(NE);
            case 12:
                return found_token_and_restart(NOT);
            case 13:
                if (is_letter(currentInput) || is_digit(currentInput))
                    get_next_char_and_go_to(13);
                else
                    go_to_state(15);
                break;
            case 14:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(55);
                else
                    go_to_state(53);
                break;
            case 15:
                return found_token_and_check_for_reserved_word();
            case 17:
                go_to_state(INITIAL_STATE);
                fail("Unexpected end of file");
                break;
            case 18:
                go_to_state(INITIAL_STATE);
                fail("Unexpected character");
                break;
            case 19:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(19);
                else if (currentInput == '.')
                    get_next_char_and_go_to(25);
                else if (currentInput == 'e' || currentInput == 'E')
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput))
                    go_to_state(18);
                else
                    go_to_state(24);
                break;
            case 20:
                if (is_digit(currentInput)) //TODO consertar automato nesse estado
                    get_next_char_and_go_to(22);
                else if (currentInput == '+' || currentInput == '-')
                    get_next_char_and_go_to(21);
                else
                    go_to_state(18);
                break;
            case 21:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(22);
                else
                    go_to_state(18);
                break;
            case 22:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(22);
                else {
                    go_to_state(23);
                }
                break;
            case 23:
                return found_token_and_restart(NUMFLOAT);
            case 24:
                return found_token_and_restart(NUMINT);
            case 25:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(26);
                else if (is_letter(currentInput))
                    go_to_state(18);
                else
                    go_to_state(23);
                break;
            case 26:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(26);
                else if (currentInput == 'E' || currentInput == 'e')
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput))
                    go_to_state(18);
                else
                    go_to_state(23);
                break;
            case 27:
                return found_token_and_restart(PLUS);
            case 28:
                return found_token_and_restart(RPARENT);
            case 29:
                return found_token_and_restart(MINUS);
            case 30:
                return found_token_and_restart(LPARENT);
            case 31:
                return found_token_and_restart(LBRACE);
            case 32:
                return found_token_and_restart(RBRACE);
            case 33:
                return found_token_and_restart(LBRACKET);
            case 34:
                return found_token_and_restart(RBRACKET);
            case 35:
                return found_token_and_restart(COLON);
            case 36:
                return found_token_and_restart(SEMICOLON);
            case 37:
                switch (currentInput) {
                    case '"':
                        get_next_char_and_go_to(40);
                        break;
                    case '\\':
                        get_next_char_and_go_to(41);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        get_next_char_and_go_to(37);
                        break;
                }
                break;
            case 38:
                switch (currentInput) {
                    case '\\':
                        get_next_char_and_go_to(99);
                        break;
                    default:
                        get_next_char_and_go_to(42);
                        break;
                }
                break;
            case 39:
                return found_token_and_restart(STAR); //TODO alterar o automato para mudar o label
            case 40:
                return found_literal_and_restart(LITERAL);
            case 41:
                get_next_char_and_go_to(37);
                break;
            case 42:
                switch (currentInput) {
                    case '\'':
                        get_next_char_and_go_to(98); //TODO atualizar o automato
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        go_to_state(18);
                        break;
                }
                break;
            case 43:
                if (currentInput == '|')
                    get_next_char_and_go_to(44);
                else
                    go_to_state(45);
                break;
            case 44:
                return found_token_and_restart(OR);
            case 45:
                return found_token_and_restart(PIPE);
            case 46:
                if (currentInput == '&')
                    get_next_char_and_go_to(47);
                else
                    go_to_state(48);
                break;
            case 47:
                return found_token_and_restart(AND);
            case 48:
                return found_token_and_restart(ADDRESS);
            case 49:
                switch (currentInput) {
                    case '*':
                        get_next_char_and_go_to(51);
                        break;
                    default:
                        go_to_state(50);
                        break;
                }
                break;
            case 50:
                return found_token_and_restart(SLASH);
            case 51:
                switch (currentInput) {
                    case '*':
                        get_next_char_and_go_to(52);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        get_next_char_and_go_to(51);
                        break;
                }
                break;
            case 52:
                switch (currentInput) {
                    case '/':
                        get_next_char_and_go_to(INITIAL_STATE);
                        break;
                    case EOF:
                    case ENDOFFILE:
                        go_to_state(17);
                        break;
                    default:
                        get_next_char_and_go_to(51);
                        break;
                }
                break;
            case 53:
                return found_token_and_restart(DOT);
            case 54:
                return found_token_and_restart(ENDOFFILE);
            case 55:
                if (is_digit(currentInput))
                    get_next_char_and_go_to(55);
                else if (currentInput == 'e' ||
                         currentInput == 'E') //TODO consertar o automato para inserir esse estado
                    get_next_char_and_go_to(20);
                else if (is_letter(currentInput)) {
                    go_to_state(18);
                } else
                    //TODO Retornar o valor do número
                    go_to_state(23);
                break;
            case 98:
                return found_literal_and_restart(LITERALCHAR);
            case 99:
                get_next_char_and_go_to(38);
                break;
        }

    }
}