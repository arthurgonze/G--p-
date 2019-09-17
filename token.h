#ifdef __cplusplus
extern "C" {
#endif
#ifndef COMPILADOR_2019_3_TOKEN_H
#define COMPILADOR_2019_3_TOKEN_H

/// TOKEN NAMES
// <
#define LT 1
// <=
#define LE 2
// ==
#define EQ 3
// !=
#define NE 4
// >
#define GT 5
// >=
#define GE 6
// IF
#define IF 7
// BOOL
#define BOOL 8
// ELSE
#define ELSE 9
// ID
#define ID 10
// INT
#define INT 11
// FLOAT
#define FLOAT 12
// ,
#define COMMA 13
// (
#define LPARENT 14
// )
#define RPARENT 15
// =
#define ASSIGN 16
// ;
#define SEMICOLON 17
// EOF
#define ENDOFFILE 0
// WHILE
#define WHILE 19
// [
#define LBRACKET 21
// ]
#define RBRACKET 22
// SWITCH
#define SWITCH 23
// BREAK
#define BREAK 24
// RETURN
#define RETURN 25
// PRINT
#define PRINT 26
// READLN
#define READLN 27
// THROW
#define THROW 28
// TRY
#define TRY 29
// CATCH
#define CATCH 30
// CASE
#define CASE 31
// LITERAL
#define LITERAL 32
// TRUE
#define TRUE 33
// FALSE
#define FALSE 34
// &
#define ADDRESS 35
// *
#define STAR 36
// .
#define DOT 37
// {
#define LBRACE 39
// }
#define RBRACE 40
// !
#define NOT 41
// CHAR
#define CHAR 42
// "
#define QUOTE 43
// '
#define SIMPLEQUOTE 44
// '\'
#define BACKSLASH 45
// :
#define COLON 46
// +
#define PLUS 47
// -
#define MINUS 48
// |
#define PIPE 49
// /
#define SLASH 50
// %
#define PERCENT 51
// &&
#define AND 52
// ||
#define OR 53
// ->
#define POINTER 54
// typedef
#define TYPEDEF 55
// struct
#define STRUCT 56
// NUMINT
#define NUMINT 57
// NUMFLOAT
#define NUMFLOAT 58
// LITERAL CHAR
#define LITERALCHAR 59
char *token_id_to_name(int id);

#endif //COMPILADOR_2019_3_TOKEN_H
#ifdef __cplusplus
}
#endif