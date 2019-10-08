//
// Created by souzajbr on 10/09/2019.
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

#include "../SymbolTable.h"

struct token_info {
    int token;
    char *lexeme;
};

struct SymbolTable get_literals_table();

struct SymbolTable get_reserved_words_table();

struct SymbolTable get_identifiers_table();

struct token_info lexical_analyzer_next_token();

void lexical_analyzer_init(FILE* fp);

void lexical_analyzer_dispose();

#endif //COMPILADOR_2019_3_ANALYZER_H
//#ifdef __cplusplus
//}
//#endif