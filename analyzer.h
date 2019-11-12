//
// Created by souzajbr on 10/09/2019.
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

#include "symboltable.h"
#include "ctype.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "token.h"
#include "symboltable.h"

#define IO_BUFFER_SIZE 4096
#define BUFFER_SIZE 32
#define INITIAL_STATE 0

LiteralsTable  get_literals_table();

ReservedWordsTable  get_reserved_words_table();

IdentifiersTable  get_identifiers_table();

int lexical_analyzer_next_token();

char* lexical_analyzer_last_lexeme();

void lexical_analyzer_init(FILE* fp);

void lexical_analyzer_dispose();

char const* token_id_to_name(int id);

#endif //COMPILADOR_2019_3_ANALYZER_H
//#ifdef __cplusplus
//}
//#endif