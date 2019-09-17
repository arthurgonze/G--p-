//
// Created by souzajbr on 10/09/2019.
//
#include "token.h"

char* token_id_to_name(int id) {

    switch(id) {
        case LT : return  "LT";
        case LE : return  "LE";
        case EQ : return  "EQ";
        case NE : return  "NE";
        case GT : return  "GT";
        case GE : return  "GE";
        case IF : return  "IF";
        case BOOL : return  "BOOL";
        case ELSE : return  "ELSE";
        case ID : return  "ID";
        case INT : return  "INT";
        case FLOAT : return  "FLOAT";
        case COMMA : return  "COMMA";
        case LPARENT : return  "LPARENT";
        case RPARENT : return  "RPARENT";
        case ASSIGN : return  "ASSIGN";
        case SEMICOLON : return  "SEMICOLON";
        case ENDOFFILE : return  "ENDOFFILE";
        case WHILE : return  "WHILE";
        case FOR : return  "FOR";
        case LBRACKET : return  "LBRACKET";
        case RBRACKET : return  "RBRACKET";
        case SWITCH : return  "SWITCH";
        case BREAK : return  "BREAK";
        case RETURN : return  "RETURN";
        case PRINT : return  "PRINT";
        case READLN : return  "READLN";
        case THROW : return  "THROW";
        case TRY : return  "TRY";
        case CATCH : return  "CATCH";
        case CASE : return  "CASE";
        case LITERAL : return  "LITERAL";
        case TRUE : return  "TRUE";
        case FALSE : return  "FALSE";
        case ADDRESS : return  "ADDRESS";
        case STAR : return  "STAR";
        case DOT : return  "DOT";
        case DO : return  "DO";
        case LBRACE : return  "LBRACE";
        case RBRACE : return  "RBRACE";
        case NOT : return  "NOT";
        case CHAR : return  "CHAR";
        case QUOTE : return  "QUOTE";
        case SIMPLEQUOTE : return  "SIMPLEQUOTE";
        case BACKSLASH : return  "BACKSLASH";
        case COLON : return  "COLON";
        case PLUS : return  "PLUS";
        case MINUS : return  "MINUS";
        case PIPE : return  "PIPE";
        case SLASH : return  "SLASH";
        case PERCENT : return  "PERCENT";
        case AND : return  "AND";
        case OR : return  "OR";
        case POINTER : return  "POINTER";
        case TYPEDEF : return  "TYPEDEF";
        case STRUCT : return  "STRUCT";
        case NUMINT : return  "NUMINT";
        case NUMFLOAT : return  "NUMFLOAT";
        case LITERALCHAR: return "LITERALCHAR";
        default: return "UNKNOWN";
    }
}