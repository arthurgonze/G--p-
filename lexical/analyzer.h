//
// Created by souzajbr on 10/09/2019.
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

#include "../SymbolTable.h"
#include "ctype.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "../token.h"
#include "../SymbolTable.h"

#define IO_BUFFER_SIZE 4096
#define BUFFER_SIZE 32
#define INITIAL_STATE 0

struct token_info {
    int token;
    char *lexeme;
};

SymbolTable  get_literals_table();

SymbolTable  get_reserved_words_table();

SymbolTable  get_identifiers_table();

struct token_info lexical_analyzer_next_token();

void lexical_analyzer_init(FILE* fp);

void lexical_analyzer_dispose();

char const* token_id_to_name(int id);

#endif //COMPILADOR_2019_3_ANALYZER_H
//#ifdef __cplusplus
//}
//#endif