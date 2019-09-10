//
// Created by souzajbr on 10/09/2019.
//

#include "ctype.h"
#include "../token.h"
#include "stdbool.h"
#include "stdio.h"

#define INITIAL_STATE 0;

int currentState = INITIAL_STATE;

char currentInput;

int count = 0;
//char* input = "< <= == >= > = != ! asda 10.03 10/";
char* input = "(10+30) (10.1 + .030 - .3e-40)/";
char getNextChar() {
    currentInput = input[count++];
}

void getNextCharAndGoTo(int state) {
    currentInput = getNextChar();
    currentState = state;
}

void goToState(int state){
    currentState = state;
}
void init() {
    getNextChar();
}

bool isLetter(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

void fail(char* reason) {
    //TODO jogar os erros para uma pilha
    printf("Lexicon Error: %s", reason);
}

int getNextToken() {

    while(1) {

        switch (currentState) {

            case 0:
                if(isspace(currentInput)) {
                    getNextCharAndGoTo(0);
                }
                else if (isLetter(currentInput)) {
                    getNextCharAndGoTo(13);
                }
                else if (isDigit(currentInput))
                    getNextCharAndGoTo(19);
                else switch (currentInput) {
                    case '<':
                        getNextCharAndGoTo(1);
                        break;
                    case '=':
                        getNextCharAndGoTo(4);
                        break;
                    case '>':
                        getNextCharAndGoTo(5);
                        break;
                    case '!':
                        getNextCharAndGoTo(10);
                        break;
                    case '.':
                        getNextCharAndGoTo(14);
                        break;
                    case '+':
                        getNextCharAndGoTo(27);
                        break;
                    case ')':
                        getNextCharAndGoTo(28);
                        break;
                    case '-':
                        getNextCharAndGoTo(29);
                        break;
                    case '(':
                        getNextCharAndGoTo(30);
                        break;
                    default:
                        return -1;
                }
                break;
            case 1:
                switch (currentInput) {
                    case '=':
                        goToState(2);
                        break;
                    default:
                        goToState(3);
                        break;
                }
                break;
            case 2:
                getNextCharAndGoTo(0);
                return LE;
            case 3:
                goToState(0);
                return LT;
            case 4:
                switch (currentInput) {
                    case '=':
                        getNextCharAndGoTo(6);
                        break;
                    default:
                        goToState(7);
                        break;
                }
                break;
            case 5:
                switch (currentInput) {
                    case '=':
                        getNextCharAndGoTo(8);
                        break;
                    default:
                        goToState(9);
                        break;
                }
                break;
            case 6:
                getNextCharAndGoTo(0);
                return EQ;
            case 7:
                goToState(0);
                return ASSIGN;
            case 8:
                getNextCharAndGoTo(0);
                return GE;
            case 9:
                goToState(0);
                return GT;
            case 10:
                switch (currentInput) {
                    case '=':
                        getNextCharAndGoTo(11);
                        break;
                    default:
                        goToState(12);
                        break;
                }
                break;
            case 11:
                getNextCharAndGoTo(0);
                return NE;
            case 12:
                goToState(0);
                return NOT;
            case 13:
                //TODO implementar a leitura do lexema
                if(isLetter(currentInput) || isDigit(currentInput))
                    getNextCharAndGoTo(13);
                else
                    goToState(15);
                break;
            case 14:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(55);
                else
                    goToState(53);
                break;
            case 15:
                goToState(0);
                //TODO verificar para ver se é palavra reservada
                //TODO adicionar
                return ID;
            case 18:
                goToState(0);
                fail("Unexpected character");
                break;
            case 19:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(19);
                else if (currentInput == '.')
                    getNextCharAndGoTo(25);
                else if(currentInput == 'e' || currentInput == 'E')
                    getNextCharAndGoTo(20);
                else if (isLetter(currentInput))
                    getNextCharAndGoTo(18);
                else
                    goToState(24);
                break;
            case 20:
                if (isDigit(currentInput)) //TODO consertar automato nesse estado
                    getNextCharAndGoTo(22);
                else if (currentInput == '+' || currentInput == '-')
                    getNextCharAndGoTo(21);
                else
                    getNextCharAndGoTo(18);
                break;
            case 21:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(22);
                else
                    getNextCharAndGoTo(18);
                break;
            case 22:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(22);
                else {
                    goToState(23);
                }
                break;
            case 23:
                goToState(0);
                return NUMFLOAT;
            case 24:
                goToState(0);
                return NUMINT;
            case 25:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(26);
                else if(isLetter(currentInput))
                    getNextCharAndGoTo(18);
                else
                    goToState(23);
                break;
            case 26:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(26);
                else if (currentInput == 'E' || currentInput == 'e')
                    getNextCharAndGoTo(20);
                else if (isLetter(currentInput))
                    getNextCharAndGoTo(18);
                else
                    goToState(23);
                break;
            case 27:
                goToState(0);
                return PLUS;
            case 28:
                goToState(0);
                return RPARENT;
            case 29:
                goToState(0);
                return MINUS;
            case 30:
                goToState(0);
                return LPARENT;
            case 53:
                goToState(0);
                return DOT;
            case 55:
                if(isDigit(currentInput))
                    getNextCharAndGoTo(55);
                else if (currentInput == 'e' || currentInput == 'E') //TODO consertar o automato para inserir esse estado
                    getNextCharAndGoTo(20);
                else if (isLetter(currentInput)) {
                    getNextCharAndGoTo(18);
                } else
                    //TODO Retornar o valor do número
                    goToState(23);
                break;
        }

    }
}