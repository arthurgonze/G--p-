#include "Parser.h"

void Parser::Program(void)
{
    switch (tok)
    {
        case PLACEHOLDER:Type();
            Pointer();
            eat(ID);
            ProgramAUX();
            break;
        case PLACEHOLDER1:TypeDecl();
            Program();
            break;
        case PLACEHOLDER2:
            eat(ENDOFILE);
            break;
        default: printf("error();");
    }
}

void Parser::ProgramAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:eat(LPARENT);
            FormalList();
            eat(RPARENT);
            eat(LBRACKET);
            VarDecl();
            StmtList();
            eat(RBRACKET);
            ProgramList();
            break;
        case PLACEHOLDER1:
            Array();
            FormalRest();
            break;
        default: printf("error();");
    }
}

void Parser::ProgramList(void)
{
    switch (tok)
    {
        case PLACEHOLDER:Program();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::TypeDecl(void)
{
    switch (tok)
    {
        case PLACEHOLDER:eat(TYPEDEF);
            eat(STRUCT);
            eat(LBRACKET);
            Type();
            IdList();
            eat(SEMICOLON);
            VarDecl();
            eat(RBRACKET);
            eat(ID);
            eat(SEMICOLON);
            TypeDecl();
            break;
        default: printf("error();");
    }
}

void Parser::VarDecl(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            Type();
            IdList();
            eat(SEMICOLON);
            VarDecl();
            break;
        default: printf("error();");
    }
}

void Parser::IdList(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            IdExpr();
            IdListAUX();
            break;
        default: printf("error();");
    }
}

void Parser::IdListAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(COMMA);
            IdExpr();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::IdExpr(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            Pointer();
            eat(ID);
            Array();
            break;
        case PLACEHOLDER1:
            eat(LPARENT);
            IdList();
            eat(RPARENT);
            break;
        default: printf("error();");
    }
}

void Parser::Pointer(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(STAR);
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::Array(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(LBRACE);
            eat(NUMINT);
            eat(RBRACE);
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::FormalList(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            Type();
            Pointer();
            eat(ID);
            Array();
            FormalRest();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::FormalRest(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(COMMA);
            Type();
            Pointer();
            eat(ID);
            Array();
            FormalRest();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::Type(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(INT);
            break;
        case PLACEHOLDER1:
            eat(FLOAT);
            break;
        case PLACEHOLDER2:
            eat(BOOL);
            break;
        case PLACEHOLDER3:
            eat(ID);
            break;
        case PLACEHOLDER4:
            eat(CHAR);
            break;
        default: printf("error();");
    }
}

void Parser::StmtList(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            Stmt();
            StmtListAUX();
            break;
        default: printf("error();");
    }
}

void Parser::StmtListAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            StmtList();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::Stmt(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            StmtAUX();
            break;
        case PLACEHOLDER1:
            eat(IF);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            IFExpr();
            break;
        default: printf("error();");
    }
}

void Parser::StmtAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(WHILE);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            Stmt();
            break;
        case PLACEHOLDER1:
            eat(SWITCH);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            eat(LBRACKET);
            CaseBlock();
            eat(RBRACKET);
            break;
        case PLACEHOLDER2:
            eat(BREAK);
            eat(SEMICOLON);
            break;
        case PLACEHOLDER3:
            eat(PRINT);
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case PLACEHOLDER4:
            eat(READLN);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case PLACEHOLDER5:
            eat(RETURN);
            ExprAssign();
            eat(SEMICOLON);
            break;
        case PLACEHOLDER6:
            eat(THROW);
            eat(SEMICOLON);
            break;
        case PLACEHOLDER7:
            eat(LBRACKET);
            StmtList();
            eat(RBRACKET);
            break;
        case PLACEHOLDER8:
            eat(ID);
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case PLACEHOLDER9:
            eat(TRY);
            Stmt();
            eat(CATCH);
            eat(LPARENT);
            // TODO IDK
            eat(RPARENT);
            Stmt();
            break;
        case PLACEHOLDER10:
            ExprAssign();
            eat(SEMICOLON);
            break;

        default: printf("error();");
    }
}

char Parser::IFExpr(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(IF);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            IFExpr();
            break;
        case PLACEHOLDER1:
            eat(ELSE);
            IFExpr();
            break;
        case PLACEHOLDER2:
            StmtAUX();
            IFExpr();
            break;
        default: printf("error();");
    }
}

void Parser::CaseBlock(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(CASE);
            eat(NUMINT);
            eat(COLON);
            CaseBlockAUX();
            break;
        default: printf("error();");
    }
}

void Parser::CaseBlockAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            StmtList();
            CaseBlock();
            break;
        case PLACEHOLDER1:
            CaseBlock();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::ExprList(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprListTail();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::ExprListTail(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprAssign();
            ExprListTailAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprListTailAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(COMMA);
            ExprListTail();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

void Parser::ExprAssign(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprOr();
            ExprAssignAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprAssignAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(ASSIGN);
            ExprOr();
            break;
        default: printf("error();");
    }
}

void Parser::ExprOr(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprAnd();
            ExprOrAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprOrAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(OR);
            ExprAnd();
            break;
        default: printf("error();");
    }
}

void Parser::ExprAnd(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprEquality();
            ExprAndAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprAndAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(AND);
            ExprEquality();
            break;
        default: printf("error();");
    }
}

void Parser::ExprEquality(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprRelational();
            ExprEqualityAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprEqualityAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(EQ);
            ExprRelational();
            break;
        case PLACEHOLDER1:
            eat(NE);
            ExprRelational();
            break;
        default: printf("error();");
    }
}

void Parser::ExprRelational(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprAdditive();
            ExprRelationalAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprRelationalAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(LT);
            ExprAdditive();
            break;
        case PLACEHOLDER1:
            eat(GT);
            ExprAdditive();
            break;
        case PLACEHOLDER2:
            eat(LE);
            ExprAdditive();
            break;
        case PLACEHOLDER3:
            eat(GE);
            ExprAdditive();
            break;
        default: printf("error();");
    }
}

void Parser::ExprAdditive(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprMultiplicative();
            ExprAdditiveAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprAdditiveAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(PIPE);
            ExprMultiplicative();
            break;
        case PLACEHOLDER1:
            eat(PLUS);
            ExprMultiplicative();
            break;
        case PLACEHOLDER2:
            eat(MINUS);
            ExprMultiplicative();
            break;
        default: printf("error();");
    }
}

void Parser::ExprMultiplicative(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            ExprUnary();
            ExprMultiplicativeAUX();
            break;
        default: printf("error();");
    }
}

void Parser::ExprMultiplicativeAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(ADDRESS);
            ExprUnary();
            break;
        case PLACEHOLDER1:
            eat(STAR);
            ExprUnary();
            break;
        case PLACEHOLDER2:
            eat(SLASH);
            ExprUnary();
            break;
        default: printf("error();");
    }
}

void Parser::ExprUnary(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(NOT);
            PostFixExpr();
            break;
        case PLACEHOLDER1:
            eat(PLUS);
            PostFixExpr();
            break;
        case PLACEHOLDER2:
            eat(MINUS);
            PostFixExpr();
            break;
        case PLACEHOLDER3:
            eat(STAR);
            PostFixExpr();
            break;
        case PLACEHOLDER4:
            eat(ADDRESS);
            PostFixExpr();
            break;
        case PLACEHOLDER5:
            PostFixExpr();
            break;
        default: printf("error();");
    }
}

void Parser::Primary(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(ID);
            break;
        case PLACEHOLDER1:
            eat(NUMINT);
            break;
        case PLACEHOLDER2:
            eat(NUMFLOAT);
            break;
        case PLACEHOLDER3:
            eat(LITERAL);
            break;
        case PLACEHOLDER4:
            eat(CHAR);
            break;
        case PLACEHOLDER5:
            eat(TRUE);
            break;
        case PLACEHOLDER6:
            eat(FALSE);
            break;
        case PLACEHOLDER7:
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            break;
        default: printf("error();");
    }
}

void Parser::PostFixExpr(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            Primary();
            PostFixExprAUX();
            break;
        default: printf("error();");
    }
}

void Parser::PostFixExprAUX(void)
{
    switch (tok)
    {
        case PLACEHOLDER:
            eat(DOT);
            eat(ID);
            PostFixExprAUX();
            break;
        case PLACEHOLDER1:
            eat(POINTER);
            eat(ID);
            PostFixExprAUX();
            break;
        case PLACEHOLDER2:
            eat(LBRACE);
            ExprAssign();
            eat(RBRACE);
            PostFixExprAUX();
            break;
        case PLACEHOLDER3:
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            PostFixExprAUX();
            break;

            // TODO EPSILON
        default: printf("error();");
    }
}