#include "Parser.h"

void Parser::StartParser()
{
    advance();
    return Program();
}

void Parser::eat(int t)
{
    if (tok.token==t)
    {
        if (t==ID || t==NUMINT || t==NUMFLOAT)
        {
            fprintf(stdout, "MATCH - %s.%s\n", token_id_to_name(t), tok.lexeme);
        }
        else
        {
            fprintf(stdout, "MATCH - %s\n", token_id_to_name(t));
        }
        advance();
    }
    else
    {
        printf("error(eat), Token error: Esperado: %s, Processado: %s \n", token_id_to_name(t), token_id_to_name(tok.token));
    }

}

int Parser::programFollowSet[] = {-1};
void Parser::Program(void)
{
    switch (tok.token)
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
        case ENDOFFILE:eat(ENDOFFILE);
            break;
        default: printf("error(Program), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::programAUXFollowSet[] = {-1};
void Parser::ProgramAUX(void)
{
    switch (tok.token)
    {
        case LPARENT:eat(LPARENT);
            FormalList();
            eat(RPARENT);
            eat(LBRACE);
            VarDecl();
            StmtList();
            eat(RBRACE);
            ProgramList();
            break;
        case COMMA:
        case LBRACE:Array();
            FormalRest();
            break;
        default: printf("error(ProgramAUX), Token error: %s \n",token_id_to_name(tok.token) );
    }
}

int Parser::programListFollowSet[] = {-1};
void Parser::ProgramList(void)
{
    switch (tok.token)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        case TYPEDEF:Program();
            break;
            // TODO EPSILON
            // Epsilon default: printf("errorProgramList(), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
void Parser::TypeDecl(void)
{
    switch (tok.token)
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
        default: printf("error(TypeDecl), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::varDeclFollowSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, RBRACKET};
void Parser::VarDecl(void)
{
    switch (tok.token)
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
            // TODO EPSILON
            // Epsilon default: printf("error(VarDecl), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT};
void Parser::IdList(void)
{
    switch (tok.token)
    {
        case LPARENT:
        case STAR:IdExpr();
            IdListAUX();
            break;
        default: printf("error(IdList), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT};
void Parser::IdListAUX(void)
{
    switch (tok.token)
    {
        case COMMA:eat(COMMA);
            IdExpr();
            break;
            // TODO EPSILON
            // Epsilon default: printf("error(IdListAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::idExprFollowSet[] = {COMMA, SEMICOLON, RPARENT};
void Parser::IdExpr(void)
{
    switch (tok.token)
    {
        case STAR:Pointer();
            eat(ID);
            Array();
            break;
        case LPARENT:eat(LPARENT);
            IdList();
            eat(RPARENT);
            break;
        default: printf("error(IdExpr), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::pointerFollowSet[] = {RPARENT, ID, LBRACE, COMMA};
void Parser::Pointer(void)
{
    switch (tok.token)
    {
        case STAR:eat(STAR);
            break;
            // TODO EPSILON
        // Epsilon default: printf("error(Pointer), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT};
void Parser::Array(void)
{
    switch (tok.token)
    {
        case LBRACE:eat(LBRACE);
            eat(NUMINT);
            eat(RBRACE);
            break;
            // TODO EPSILON
            // Epsilon default: printf("error(Array), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::formalListFollowSet[] = {RPARENT};
void Parser::FormalList(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(FormalList), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::formalRestFollowSet[] = {RPARENT};
void Parser::FormalRest(void)
{
    switch (tok.token)
    {
        case COMMA:eat(COMMA);
            Type();
            Pointer();
            eat(ID);
            Array();
            FormalRest();
            break;
            // TODO EPSILON
            // Epsilon default: printf("error(FormalRest), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::typeFollowSet[] = {LPARENT, STAR, ID};
void Parser::Type(void)
{
    switch (tok.token)
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
        default: printf("error(Type), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::stmtListFollowSet[] = {RBRACKET, CASE};
void Parser::StmtList(void)
{
    switch (tok.token)
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
        // Epsilon default: printf("error(StmtList), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE};
void Parser::StmtListAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(StmtListAux), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::stmtFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                               ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
void Parser::Stmt(void)
{
    switch (tok.token)
    {
        case IF:eat(IF);
            eat(LPARENT);
            ExprAssign();
            eat(RPARENT);
            IFExpr();
            break;
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
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:StmtAUX();
            break;
        default: printf("error(Stmt), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::stmtAUXFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
void Parser::StmtAUX(void)
{
    switch (tok.token)
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
        case LBRACE:eat(LBRACE);
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
            // TODO 3pontos
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

        default: printf("error(StmtAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::ifExprFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                 ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
char Parser::IFExpr(void)
{
    switch (tok.token)
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
        case WHILE:
        case SWITCH:
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
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case CHAR:
        case TRUE:
        case FALSE:
        case LPARENT:StmtAUX();
            IFExpr();
            break;
        default: printf("error(IFExpr), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::caseBlockFollowSet[] = {RBRACKET};
void Parser::CaseBlock(void)
{
    switch (tok.token)
    {
        case CASE:eat(CASE);
            eat(NUMINT);
            eat(COLON);
            CaseBlockAUX();
            break;
        default: printf("error(caseBlock), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::caseBlockAUXFollowSet[] = {RBRACKET};
void Parser::CaseBlockAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(caseBlockAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprListFollowSet[] = {RPARENT};
void Parser::ExprList(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(ExprList), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprListTailFollowSet[] = {RPARENT};
void Parser::ExprListTail(void)
{
    switch (tok.token)
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
        default: printf("error(ExprListTail), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT};
void Parser::ExprListTailAUX(void)
{
    switch (tok.token)
    {
        case COMMA:eat(COMMA);
            ExprListTail();
            break;
            // TODO EPSILON
            // Epsilon default: printf("error(ExprListTailAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAssign(void)
{
    switch (tok.token)
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
        default: printf("error(ExprAssign), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAssignAUX(void)
{
    switch (tok.token)
    {
        case EQ:
        case ASSIGN:eat(ASSIGN);
            ExprOr();
            break;
            // Epsilon default: printf("error(ExprAssignAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprOr(void)
{
    switch (tok.token)
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
        default: printf("error(ExprOr), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprOrAUXFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprOrAUX(void)
{
    switch (tok.token)
    {
        case OR:eat(OR);
            ExprAnd();
            break;
            // Epsilon default: printf("error(ExprOrAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAnd(void)
{
    switch (tok.token)
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
        default: printf("error(ExprAnd), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAndAUX(void)
{
    switch (tok.token)
    {
        case AND:eat(AND);
            ExprEquality();
            break;
            // Epsilon default: printf("error(ExprAndAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprEquality(void)
{
    switch (tok.token)
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
        default: printf("error(ExprEquality), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprEqualityAUX(void)
{
    switch (tok.token)
    {
        case EQ:eat(EQ);
            ExprRelational();
            break;
        case NE:eat(NE);
            ExprRelational();
            break;
            // Epsilon default: printf("error(ExprEqualityAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprRelational(void)
{
    switch (tok.token)
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
        default: printf("error(ExprRelational), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprRelationalAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(ExprRelationalAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAdditive(void)
{
    switch (tok.token)
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
        default: printf("error(ExprAdditive), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprAdditiveAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(ExprAdditiveAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON,
                                             COMMA, RPARENT, RBRACE};
void Parser::ExprMultiplicative(void)
{
    switch (tok.token)
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
        default: printf("error(ExprMultiplicative), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprMultiplicativeAUXFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                                EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprMultiplicativeAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(ExprMultiplicativeAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                    EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::ExprUnary(void)
{
    switch (tok.token)
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
        default: printf("error(ExprUnary), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::Primary(void)
{
    switch (tok.token)
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
        default: printf("error(Primary), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::PostFixExpr(void)
{
    switch (tok.token)
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
        default: printf("error(PostFixExpr), Token error: %s \n",token_id_to_name(tok.token));
    }
}

int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
void Parser::PostFixExprAUX(void)
{
    switch (tok.token)
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
            // Epsilon default: printf("error(PostFixExprAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}
