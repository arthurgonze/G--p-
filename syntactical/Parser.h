#ifndef COMPILADOR_2019_3_PARSER_H
#define COMPILADOR_2019_3_PARSER_H

#include <cstdio>
#include "../token.h"
#include "../lexical/analyzer.h"

class Parser
{
    token_info tok;
    inline void advance() { tok = lexical_analyzer_next_token(); }
    void eat(int t);

    static int programFollowSet [];
    void Program(void);

    static int programAUXFollowSet [];
    void ProgramAUX(void);

    static int programListFollowSet [];
    void ProgramList(void);

    static int typeDeclFollowSet [];
    void TypeDecl(void);

    static int varDeclFollowSet [];
    void VarDecl(void);

    static int idListFollowSet [];
    void IdList(void);

    static int idListAUXFollowSet [];
    void IdListAUX(void);

    static int idExprFollowSet [];
    void IdExpr(void);

    static int pointerFollowSet [];
    void Pointer(void);

    static int arrayFollowSet [];
    void Array(void);

    static int formalListFollowSet [];
    void FormalList(void);

    static int formalRestFollowSet [];
    void FormalRest(void);

    static int typeFollowSet [];
    void Type(void);

    static int stmtListFollowSet [];
    void StmtList(void);

    static int stmtListAUXFollowSet [];
    void StmtListAUX(void);

    static int stmtFollowSet [];
    void Stmt(void);

    static int stmtAUXFollowSet [];
    void StmtAUX(void);

    static int ifExprFollowSet [];
    char IFExpr(void);

    static int caseBlockFollowSet [];
    void CaseBlock(void);

    static int caseBlockAUXFollowSet [];
    void CaseBlockAUX(void);

    static int exprListFollowSet [];
    void ExprList(void);

    static int exprListTailFollowSet [];
    void ExprListTail(void);

    static int exprListTailAUXFollowSet [];
    void ExprListTailAUX(void);

    static int exprAssignFollowSet [];
    void ExprAssign(void);

    static int exprAssignAUXFollowSet [];
    void ExprAssignAUX(void);

    static int exprOrFollowSet [];
    void ExprOr(void);

    static int exprOrAUXFollowSet [];
    void ExprOrAUX(void);

    static int exprAndFollowSet [];
    void ExprAnd(void);

    static int exprAndAUXFollowSet [];
    void ExprAndAUX(void);

    static int exprEqualityFollowSet [];
    void ExprEquality(void);

    static int exprEqualityAUXFollowSet [];
    void ExprEqualityAUX(void);

    static int exprRelationalFollowSet [];
    void ExprRelational(void);

    static int exprRelationalAUXFollowSet [];
    void ExprRelationalAUX(void);

    static int exprAdditiveFollowSet [];
    void ExprAdditive(void);

    static int exprAdditiveAUXFollowSet [];
    void ExprAdditiveAUX(void);

    static int exprMultiplicativeFollowSet [];
    void ExprMultiplicative(void);

    static int exprMultiplicativeAUXFollowSet [];
    void ExprMultiplicativeAUX(void);

    static int exprUnaryFollowSet [];
    void ExprUnary(void);

    static int primaryFollowSet [];
    void Primary(void);

    static int postFixExprFollowSet [];
    void PostFixExpr(void);

    static int postFixExprAUXFollowSet [];
    void PostFixExprAUX(void);
public:
    void StartParser();
};

#endif //COMPILADOR_2019_3_PARSER_H
