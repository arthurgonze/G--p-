//
// Created by souzajbr on 10/09/2019.
//

#include "ctype.h"
#include "../token.h"
#include "stdbool.h"

#define INITIAL_STATE 0;

int currentState = INITIAL_STATE;

char currentInput;

int count = 0;
char* input = "< <= == >= > = != ! asda /";
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
            case 15:
                goToState(0);
                return ID;
        }

    }
}