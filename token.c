//
// Created by souzajbr on 10/09/2019.
//
#include "token.h"

char const* token_id_to_name(int id) {
    char const * tokens [] = {"ENDOFILE","LT","LE","EQ","NE","GT","GE","IF","BOOL","ELSE","ID",
                 "INT","FLOAT","COMMA","LPARENT","RPARENT","ASSIGN","SEMICOLON",
                 "WHILE","LBRACKET","RBRACKET","SWITCH","BREAK","RETURN","PRINT",
                 "READLN","THROW","TRY","CATCH","CASE","LITERAL","TRUE","FALSE",
                 "ADDRESS","STAR","DOT","LBRACE","RBRACE","NOT","CHAR","QUOTE",
                 "SIMPLEQUOTE","BACKSLASH","COLON","PLUS","MINUS","PIPE","SLASH",
                 "PERCENT","AND","OR","POINTER","TYPEDEF","STRUCT","NUMINT",
                 "NUMFLOAT","LITERALCHAR"};

    return tokens[id];
}
