#include "Parser.h"

int Parser::programFirstSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
int Parser::programFollowSet[] = {-1};
void Parser::Program(void)
{
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:Type();
            Pointer();
            eat(ID);
            ProgramAUX();
            break;
        case TYPEDEF:TypeDecl();
            Program();
            break;
        case ENDOFILE:eat(ENDOFILE);
            break;
        default: printf("error();");
    }
}

int Parser::programAUXFirstSet[] = {LPARENT, LBRACE, COMMA};
int Parser::programAUXFollowSet[] = {-1};
void Parser::ProgramAUX(void)
{
    switch (tok)
    {

        case LPARENT:eat(LPARENT);
            FormalList();
            eat(RPARENT);
            eat(LBRACKET);
            VarDecl();
            StmtList();
            eat(RBRACKET);
            ProgramList();
            break;
        case COMMA:
        case LBRACE:Array();
            FormalRest();
            break;
        default: printf("error();");
    }
}

int Parser::programListFirstSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
int Parser::programListFollowSet[] = {-1};
void Parser::ProgramList(void)
{
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        case TYPEDEF:Program();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::typeDeclFirstSet[] = {TYPEDEF};
int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
void Parser::TypeDecl(void)
{
    switch (tok)
    {
        case TYPEDEF:eat(TYPEDEF);
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

int Parser::varDeclFirstSet[] = {INT, FLOAT, BOOL, ID, CHAR};
int Parser::varDeclFollowSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, RBRACKET};
void Parser::VarDecl(void)
{
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:Type();
            IdList();
            eat(SEMICOLON);
            VarDecl();
            break;
        default: printf("error();");
    }
}

int Parser::idListFirstSet[] = {LPARENT, STAR};
int Parser::idListFollowSet[] = {SEMICOLON, RPARENT};
void Parser::IdList(void)
{
    switch (tok)
    {
        case LPARENT:
        case STAR:IdExpr();
            IdListAUX();
            break;
        default: printf("error();");
    }
}

int Parser::idListAUXFirstSet[] = {COMMA};
int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT};
void Parser::IdListAUX(void)
{
    switch (tok)
    {
        case COMMA:eat(COMMA);
            IdExpr();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::idExprFirstSet[] = {LPARENT, STAR};
int Parser::idExprFollowSet[] = {COMMA, SEMICOLON, RPARENT};
void Parser::IdExpr(void)
{
    switch (tok)
    {
        case STAR:Pointer();
            eat(ID);
            Array();
            break;
        case LPARENT:eat(LPARENT);
            IdList();
            eat(RPARENT);
            break;
        default: printf("error();");
    }
}

int Parser::pointerFirstSet[] = {STAR};
int Parser::pointerFollowSet[] = {RPARENT, ID, LBRACE, COMMA};
void Parser::Pointer(void)
{
    switch (tok)
    {
        case STAR:eat(STAR);
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::arrayFirstSet[] = {LBRACE};
int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT};
void Parser::Array(void)
{
    switch (tok)
    {
        case LBRACE:eat(LBRACE);
            eat(NUMINT);
            eat(RBRACE);
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::formalListFirstSet[] = {INT, FLOAT, BOOL, ID, CHAR};
int Parser::formalListFollowSet[] = {RPARENT};
void Parser::FormalList(void)
{
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:Type();
            Pointer();
            eat(ID);
            Array();
            FormalRest();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::formalRestFirstSet[] = {COMMA};
int Parser::formalRestFollowSet[] = {RPARENT};
void Parser::FormalRest(void)
{
    switch (tok)
    {
        case COMMA:eat(COMMA);
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

int Parser::typeFirstSet[] = {INT, FLOAT, BOOL, ID, CHAR};
int Parser::typeFollowSet[] = {LPARENT, STAR, ID};
void Parser::Type(void)
{
    switch (tok)
    {
        case INT:eat(INT);
            break;
        case FLOAT:eat(FLOAT);
            break;
        case BOOL:eat(BOOL);
            break;
        case ID:eat(ID);
            break;
        case CHAR:eat(CHAR);
            break;
        default: printf("error();");
    }
}

int Parser::stmtListFirstSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY,
                                  NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMFLOAT, NUMINT, LITERAL,
                                  LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::stmtListFollowSet[] = {RBRACKET, CASE};
void Parser::StmtList(void)
{
    switch (tok)
    {
        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMFLOAT:
        case NUMINT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:Stmt();
            StmtListAUX();
            break;
        default: printf("error();");
    }
}

int Parser::stmtListAUXFirstSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY,
                                     NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMFLOAT, NUMINT, LITERAL,
                                     LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE};
void Parser::StmtListAUX(void)
{
    switch (tok)
    {
        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACE:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMFLOAT:
        case NUMINT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:StmtList();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::stmtFirstSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY,
                              NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::stmtFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                               ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
void Parser::Stmt(void)
{
    switch (tok)
    {
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACKET:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:StmtAUX();
            break;
        case IF:eat(IF);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            IFExpr();
            break;
        default: printf("error();");
    }
}

int Parser::stmtAUXFirstSet[] = {WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY,
                                 NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT};
int Parser::stmtAUXFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
void Parser::StmtAUX(void)
{
    switch (tok)
    {
        case WHILE:eat(WHILE);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            Stmt();
            break;
        case SWITCH:eat(SWITCH);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            eat(LBRACKET);
            CaseBlock();
            eat(RBRACKET);
            break;
        case BREAK:eat(BREAK);
            eat(SEMICOLON);
            break;
        case PRINT:eat(PRINT);
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case READLN:eat(READLN);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case RETURN:eat(RETURN);
            ExprAssign();
            eat(SEMICOLON);
            break;
        case THROW:eat(THROW);
            eat(SEMICOLON);
            break;
        case LBRACKET:eat(LBRACKET);
            StmtList();
            eat(RBRACKET);
            break;
        case ID:eat(ID);
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        case TRY:eat(TRY);
            Stmt();
            eat(CATCH);
            eat(LPARENT);
            // TODO IDK
            eat(RPARENT);
            Stmt();
            break;
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprAssign();
            eat(SEMICOLON);
            break;

        default: printf("error();");
    }
}

int Parser::ifExprFirstSet[] = {IF, ELSE, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT,
                                PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT};
int Parser::ifExprFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                 ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
char Parser::IFExpr(void)
{
    switch (tok)
    {
        case IF:eat(IF);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            IFExpr();
            break;
        case ELSE:eat(ELSE);
            IFExpr();
            break;
        case PLACEHOLDER2:StmtAUX();
            IFExpr();
            break;
        default: printf("error();");
    }
}

int Parser::caseBlockFirstSet[] = {CASE};
int Parser::caseBlockFollowSet[] = {RBRACKET};
void Parser::CaseBlock(void)
{
    switch (tok)
    {
        case CASE:eat(CASE);
            eat(NUMINT);
            eat(COLON);
            CaseBlockAUX();
            break;
        default: printf("error();");
    }
}

int Parser::caseBlockAUXFirstSet[] = {CASE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACKET, TRY, NOT,
                                      PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT};
int Parser::caseBlockAUXFollowSet[] = {RBRACKET};
void Parser::CaseBlockAUX(void)
{
    switch (tok)
    {
        case IF:
        case WHILE:
        case SWITCH:
        case BREAK:
        case PRINT:
        case READLN:
        case RETURN:
        case THROW:
        case LBRACKET:
        case TRY:
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case CHAR:
        case TRUE:
        case FALSE:
        case LPARENT:StmtList();
            CaseBlock();
            break;
        case CASE:CaseBlock();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::exprListFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprListFollowSet[] = {RPARENT};
void Parser::ExprList(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprListTail();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::exprListTailFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT};
int Parser::exprListTailFollowSet[] = {RPARENT};
void Parser::ExprListTail(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprAssign();
            ExprListTailAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprListTailAUXFirstSet[] = {COMMA};
int Parser::exprListTailAUXFollowSet[] = {RPARENT};
void Parser::ExprListTailAUX(void)
{
    switch (tok)
    {
        case COMMA:eat(COMMA);
            ExprListTail();
            break;
            // TODO EPSILON
        default: printf("error();");
    }
}

int Parser::exprAssignFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAssign(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprOr();
            ExprAssignAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprAssignAUXFirstSet[] = {EQ};
int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAssignAUX(void)
{
    switch (tok)
    {
        case EQ:
        case ASSIGN:eat(ASSIGN);
            ExprOr();
            break;
        default: printf("error();");
    }
}

int Parser::exprOrFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprOr(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprAnd();
            ExprOrAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprOrAUXFirstSet[] = {OR};
int Parser::exprOrAUXFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprOrAUX(void)
{
    switch (tok)
    {
        case OR:eat(OR);
            ExprAnd();
            break;
        default: printf("error();");
    }
}

int Parser::exprAndFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT};
int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAnd(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprEquality();
            ExprAndAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprAndAUXFirstSet[] = {AND};
int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAndAUX(void)
{
    switch (tok)
    {
        case AND:eat(AND);
            ExprEquality();
            break;
        default: printf("error();");
    }
}

int Parser::exprEqualityFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprEquality(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprRelational();
            ExprEqualityAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprEqualityAUXFirstSet[] = {EQ, NE};
int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprEqualityAUX(void)
{
    switch (tok)
    {
        case EQ:eat(EQ);
            ExprRelational();
            break;
        case NE:eat(NE);
            ExprRelational();
            break;
        default: printf("error();");
    }
}

int Parser::exprRelationalFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprRelational(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprAdditive();
            ExprRelationalAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprRelationalAUXFirstSet[] = {LT, GT, LE, GE};
int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprRelationalAUX(void)
{
    switch (tok)
    {
        case LT:eat(LT);
            ExprAdditive();
            break;
        case GT:eat(GT);
            ExprAdditive();
            break;
        case LE:eat(LE);
            ExprAdditive();
            break;
        case GE:eat(GE);
            ExprAdditive();
            break;
        default: printf("error();");
    }
}

int Parser::exprAdditiveFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAdditive(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprMultiplicative();
            ExprAdditiveAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprAdditiveAUXFirstSet[] = {PIPE, PLUS, MINUS};
int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAdditiveAUX(void)
{
    switch (tok)
    {
        case PIPE:eat(PIPE);
            ExprMultiplicative();
            break;
        case PLUS:eat(PLUS);
            ExprMultiplicative();
            break;
        case MINUS:eat(MINUS);
            ExprMultiplicative();
            break;
        default: printf("error();");
    }
}

int Parser::exprMultiplicativeFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON,
                                             COMMA, RPARENT, RBRACE};
void Parser::ExprMultiplicative(void)
{
    switch (tok)
    {
        case NOT:
        case PLUS:
        case MINUS:
        case STAR:
        case ADDRESS:
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:ExprUnary();
            ExprMultiplicativeAUX();
            break;
        default: printf("error();");
    }
}

int Parser::exprMultiplicativeAUXFirstSet[] = {ADDRESS, STAR, SLASH};
int Parser::exprMultiplicativeAUXFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                                EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprMultiplicativeAUX(void)
{
    switch (tok)
    {
        case ADDRESS:eat(ADDRESS);
            ExprUnary();
            break;
        case STAR:eat(STAR);
            ExprUnary();
            break;
        case SLASH:eat(SLASH);
            ExprUnary();
            break;
        default: printf("error();");
    }
}

int Parser::exprUnaryFirstSet[] = {NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                    EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprUnary(void)
{
    switch (tok)
    {
        case NOT:eat(NOT);
            PostFixExpr();
            break;
        case PLUS:eat(PLUS);
            PostFixExpr();
            break;
        case MINUS:eat(MINUS);
            PostFixExpr();
            break;
        case STAR:eat(STAR);
            PostFixExpr();
            break;
        case ADDRESS:eat(ADDRESS);
            PostFixExpr();
            break;
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:PostFixExpr();
            break;
        default: printf("error();");
    }
}

int Parser::primaryFirstSet[] = {ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::Primary(void)
{
    switch (tok)
    {
        case ID:eat(ID);
            break;
        case NUMINT:eat(NUMINT);
            break;
        case NUMFLOAT:eat(NUMFLOAT);
            break;
        case LITERAL:eat(LITERAL);
            break;
        case LITERALCHAR:eat(LITERALCHAR);
            break;
        case TRUE:eat(TRUE);
            break;
        case FALSE:eat(FALSE);
            break;
        case LPARENT:eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            break;
        default: printf("error();");
    }
}

int Parser::postFixExprFirstSet[] = {ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT};
int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::PostFixExpr(void)
{
    switch (tok)
    {
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:Primary();
            PostFixExprAUX();
            break;
        default: printf("error();");
    }
}

int Parser::postFixExprAUXFirstSet[] = {DOT, POINTER, LBRACE, LPARENT};
int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::PostFixExprAUX(void)
{
    switch (tok)
    {
        case DOT:eat(DOT);
            eat(ID);
            PostFixExprAUX();
            break;
        case POINTER:eat(POINTER);
            eat(ID);
            PostFixExprAUX();
            break;
        case LBRACE:eat(LBRACE);
            ExprAssign();
            eat(RBRACE);
            PostFixExprAUX();
            break;
        case LPARENT:eat(LPARENT);
            ExprList();
            eat(RPARENT);
            PostFixExprAUX();
            break;

            // TODO EPSILON
        default: printf("error();");
    }
}