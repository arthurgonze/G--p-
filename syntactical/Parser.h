/*
#ifndef COMPILADOR_2019_3_PARSER_H
#define COMPILADOR_2019_3_PARSER_H

#include <cstdio>

class Parser
{
    enum token
    {
        ENDOFILE, LT, LE, EQ, NE, GT, GE, IF, BOOL, ELSE, ID,
        INT, FLOAT, COMMA, LPARENT, RPARENT, ASSIGN, SEMICOLON,
        WHILE, LBRACKET, RBRACKET, SWITCH, BREAK, RETURN, PRINT,
        READLN, THROW, TRY, CATCH, CASE, LITERAL, TRUE, FALSE,
        ADDRESS, STAR, DOT, LBRACE, RBRACE, NOT, CHAR, QUOTE,
        SIMPLEQUOTE, BACKSLASH, COLON, PLUS, MINUS, PIPE, SLASH,
        PERCENT, AND, OR, POINTER, TYPEDEF, STRUCT, NUMINT,
        NUMFLOAT, LITERALCHAR, PLACEHOLDER, PLACEHOLDER1, PLACEHOLDER2, PLACEHOLDER3, PLACEHOLDER4,
        PLACEHOLDER5, PLACEHOLDER6, PLACEHOLDER7, PLACEHOLDER8, PLACEHOLDER9, PLACEHOLDER10
    };
    extern enum token getToken(void);
    enum token tok;

    inline void advance() { tok = getToken(); }
    inline void eat(enum token t)
    {
        if (tok==t)
            advance();
        else
            printf("error();");
    }

    void Program(void);
    void ProgramAUX(void);
    void ProgramList(void);

    void TypeDecl(void);
    void VarDecl(void);

    void IdList(void);
    void IdListAUX(void);
    void IdExpr(void);

    void Pointer(void);
    void Array(void);

    void FormalList(void);
    void FormalRest(void);

    void Type(void);

    void StmtList(void);
    void StmtListAUX(void);
    void Stmt(void);
    void StmtAUX(void);
    char IFExpr(void);
    void CaseBlock(void);
    void CaseBlockAUX(void);

    void ExprList(void);
    void ExprListTail(void);
    void ExprListTailAUX(void);
    void ExprAssign(void);
    void ExprAssignAUX(void);
    void ExprOr(void);
    void ExprOrAUX(void);
    void ExprAnd(void);
    void ExprAndAUX(void);
    void ExprEquality(void);
    void ExprEqualityAUX(void);
    void ExprRelational(void);
    void ExprRelationalAUX(void);
    void ExprAdditive(void);
    void ExprAdditiveAUX(void);
    void ExprMultiplicative(void);
    void ExprMultiplicativeAUX(void);
    void ExprUnary(void);

    void Primary(void);
    void PostFixExpr(void);
    void PostFixExprAUX(void);
};

#endif //COMPILADOR_2019_3_PARSER_H
*/
