#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

#include "symbol_table.h"
#include "ctype.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "token.h"
#include "symbol_table.h"

#define IO_BUFFER_SIZE 4096
#define BUFFER_SIZE 32
#define INITIAL_STATE 0

class LiteralsTable;

class ReservedWordsTable;

class IdsTable;

LiteralsTable get_literals_table();

ReservedWordsTable get_reserved_words_table();

IdsTable get_identifiers_table();

int lexical_analyzer_next_token();

char *lexical_analyzer_last_lexeme();

void lexical_analyzer_init(FILE *fp);

void lexical_analyzer_dispose();

char const *token_id_to_name(int id);

int lexical_analyzer_getLine();

#endif //COMPILADOR_2019_3_ANALYZER_H
