//
// Created by souzajbr on 10/09/2019.
//
//#ifdef __cplusplus
//extern "C" {
//#endif

#ifndef COMPILADOR_2019_3_ANALYZER_H
#define COMPILADOR_2019_3_ANALYZER_H

struct token_info {
    int token;
    char* lexeme;
};

struct token_info lexical_analyzer_next_token();

void lexical_analyzer_init();

void lexical_analyzer_dispose();

#endif //COMPILADOR_2019_3_ANALYZER_H
//#ifdef __cplusplus
//}
//#endif