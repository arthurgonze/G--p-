#ifndef COMPILADOR_2019_3_PARSER_H
#define COMPILADOR_2019_3_PARSER_H

#include <cstdio>

class Parser
{
    // PlaceHolders para tokens e funcoes de IO e no lugar de chamar a funcao de erro esta so imprimindo a chamada
    // da funcao
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

    int programFirstSet [];
    int programFollowSet [];
    void Program(void);

    int programAUXFirstSet [];
    int programAUXFollowSet [];
    void ProgramAUX(void);

    int programListFirstSet [];
    int programListFollowSet [];
    void ProgramList(void);

    int typeDeclFirstSet [];
    int typeDeclFollowSet [];
    void TypeDecl(void);

    int varDeclFirstSet [];
    int varDeclFollowSet [];
    void VarDecl(void);

    int idListFirstSet [];
    int idListFollowSet [];
    void IdList(void);

    int idListAUXFirstSet [];
    int idListAUXFollowSet [];
    void IdListAUX(void);

    int idExprFirstSet [];
    int idExprFollowSet [];
    void IdExpr(void);

    int pointerFirstSet [];
    int pointerFollowSet [];
    void Pointer(void);

    int arrayFirstSet [];
    int arrayFollowSet [];
    void Array(void);

    int formalListFirstSet [];
    int formalListFollowSet [];
    void FormalList(void);

    int formalRestFirstSet [];
    int formalRestFollowSet [];
    void FormalRest(void);

    int typeFirstSet [];
    int typeFollowSet [];
    void Type(void);

    int stmtListFirstSet [];
    int stmtListFollowSet [];
    void StmtList(void);

    int stmtListAUXFirstSet [];
    int stmtListAUXFollowSet [];
    void StmtListAUX(void);

    int stmtFirstSet [];
    int stmtFollowSet [];
    void Stmt(void);

    int stmtAUXFirstSet [];
    int stmtAUXFollowSet [];
    void StmtAUX(void);

    int ifExprFirstSet [];
    int ifExprFollowSet [];
    char IFExpr(void);

    int caseBlockFirstSet [];
    int caseBlockFollowSet [];
    void CaseBlock(void);

    int caseBlockAUXFirstSet [];
    int caseBlockAUXFollowSet [];
    void CaseBlockAUX(void);

    int exprListFirstSet [];
    int exprListFollowSet [];
    void ExprList(void);

    int exprListTailFirstSet [];
    int exprListTailFollowSet [];
    void ExprListTail(void);

    int exprListTailAUXFirstSet [];
    int exprListTailAUXFollowSet [];
    void ExprListTailAUX(void);

    int exprAssignFirstSet [];
    int exprAssignFollowSet [];
    void ExprAssign(void);

    int exprAssignAUXFirstSet [];
    int exprAssignAUXFollowSet [];
    void ExprAssignAUX(void);

    int exprOrFirstSet [];
    int exprOrFollowSet [];
    void ExprOr(void);

    int exprOrAUXFirstSet [];
    int exprOrAUXFollowSet [];
    void ExprOrAUX(void);

    int exprAndFirstSet [];
    int exprAndFollowSet [];
    void ExprAnd(void);

    int exprAndAUXFirstSet [];
    int exprAndAUXFollowSet [];
    void ExprAndAUX(void);

    int exprEqualityFirstSet [];
    int exprEqualityFollowSet [];
    void ExprEquality(void);

    int exprEqualityAUXFirstSet [];
    int exprEqualityAUXFollowSet [];
    void ExprEqualityAUX(void);

    int exprRelationalFirstSet [];
    int exprRelationalFollowSet [];
    void ExprRelational(void);

    int exprRelationalAUXFirstSet [];
    int exprRelationalAUXFollowSet [];
    void ExprRelationalAUX(void);

    int exprAdditiveFirstSet [];
    int exprAdditiveFollowSet [];
    void ExprAdditive(void);

    int exprAdditiveAUXFirstSet [];
    int exprAdditiveAUXFollowSet [];
    void ExprAdditiveAUX(void);

    int exprMultiplicativeFirstSet [];
    int exprMultiplicativeFollowSet [];
    void ExprMultiplicative(void);

    int exprMultiplicativeAUXFirstSet [];
    int exprMultiplicativeAUXFollowSet [];
    void ExprMultiplicativeAUX(void);

    int exprUnaryFirstSet [];
    int exprUnaryFollowSet [];
    void ExprUnary(void);

    int primaryFirstSet [];
    int primaryFollowSet [];
    void Primary(void);

    int postFixExprFirstSet [];
    int postFixExprFollowSet [];
    void PostFixExpr(void);

    int postFixExprAUXFirstSet [];
    int postFixExprAUXFollowSet [];
    void PostFixExprAUX(void);
};

#endif //COMPILADOR_2019_3_PARSER_H
