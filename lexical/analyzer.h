//
// Created by souzajbr on 10/09/2019.
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

#include "../SymbolTable.h"

SymbolTable  get_literals_table();

SymbolTable  get_reserved_words_table();

SymbolTable  get_identifiers_table();

int lexical_analyzer_next_token();

char* lexical_analyzer_last_lexeme();

void lexical_analyzer_init(FILE* fp);

void lexical_analyzer_dispose();

char const* token_id_to_name(int id);

#endif //COMPILADOR_2019_3_ANALYZER_H
//#ifdef __cplusplus
//}
//#endif