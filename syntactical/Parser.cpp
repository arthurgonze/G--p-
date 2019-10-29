#include "Parser.h"

ProgramNode *Parser::StartParser()
{
    advance();
    return Program(nullptr, nullptr, nullptr);
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
ProgramNode *Parser::Program(FunctionListNode *functionList, TypeListNode *typeList, VarFuncListNode *varList)
{
    TypeNode *type = nullptr;
    TokenNode *id = nullptr;
    switch (tok.token)
    {
        case TYPEDEF:
        {
            eat(TYPEDEF);
            eat(STRUCT);
            eat(LBRACE);
            type = Type();
            IdListNode *idList = IdList();

            eat(SEMICOLON);
            VarListNode *varListNode = new VarListNode(type, idList, VarDecl()); // TODO VarList or VarDecl

            eat(RBRACE);
            eat(ID);
            id = new TokenNode(ID, NULL);// TODO NULL->GET_LEXEME()?
            eat(SEMICOLON);
            typeList = new TypeListNode(varListNode, id, typeList);
            return Program(functionList, typeList, varList);
            break;
        }
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            type = Type();
            PointerNode *pointer = Pointer();
            eat(ID);
            id = new TokenNode(ID, NULL);// TODO NULL->GET_LEXEME()?
            return Program(functionList, typeList, ProgramAUX(type, pointer, id, varList));
            break;
        }
        case ENDOFFILE:
        {
            eat(ENDOFFILE);
            return new ProgramNode(functionList, typeList, varList);
            break;
        }
        default:
        {
            printf("error(Program), Token error: %s \n", token_id_to_name(tok.token));
            return Program(functionList, typeList, varList);
        }
    }
}

int Parser::programAUXFollowSet[] = {-1};
VarFuncListNode *Parser::ProgramAUX(TypeNode *type, PointerNode *pointer, TokenNode *id, VarFuncListNode *varFunc)
{
    switch (tok.token)
    {
        case LPARENT:
        {
            eat(LPARENT);
            FormalListNode *formalListNode = nullptr;// TODO FormalList();
            eat(RPARENT);
            eat(LBRACE);
            //VarDecl();
            //StmtList();
            VarStmtNode *varStmtNode = nullptr; // TODO VarStmt();
            eat(RBRACE);
            //ProgramList();
            return new VarFuncListNode(new FunctionListNode(type, pointer, id, formalListNode, varStmtNode), varFunc);
            break;
        }
        case COMMA:
        case LBRACE:
        {
            ArrayNode *array = Array();
            IdListNode *listNode = IdListAUX();
            //FormalRest();
            return new VarFuncListNode(new VarListNode(type, new IdListNode(pointer, id, array, listNode), nullptr), varFunc);
            break;
        }
//        default:
//            printf("error(ProgramAUX), Token error: %s \n", token_id_to_name(tok.token));
    }
}

int Parser::programListFollowSet[] = {-1};
ProgramNode *Parser::ProgramList(FunctionListNode *functions, TypeListNode *typelist, VarFuncListNode *varlist)
{
    switch (tok.token)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        case TYPEDEF:
        {
            return Program(functions, typelist, varlist);
            break;
        }
        default:
        {
            printf("errorProgramList(), Token error: %s \n", token_id_to_name(tok.token));
            return new ProgramNode(functions, typelist, varlist);
            break;
        }
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
TypeListNode *Parser::TypeDecl(TypeListNode *typeListNode)
{
    switch (tok.token)
    {
        case TYPEDEF:
        {
            eat(TYPEDEF);
            eat(STRUCT);
            eat(LBRACE);
            TypeNode *typeNode = Type();
            IdListNode *idListNode = IdList();
            eat(SEMICOLON);
            VarListNode *varListNode = new VarListNode(typeNode, idListNode, VarDecl());
            eat(RBRACKET);
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO NULL->GET_LEXEME()?
            eat(SEMICOLON);
            typeListNode = new TypeListNode(varListNode, id, typeListNode);
            return TypeDecl(typeListNode);
            break;
        }
        default:
        {
            printf("error(TypeDecl), Token error: %s \n", token_id_to_name(tok.token));
            return typeListNode;
        }
    }
}

int Parser::varDeclFollowSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, RBRACKET};
VarListNode *Parser::VarDecl()
{
    switch (tok.token)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            IdListNode *idListNode = IdList();
            eat(SEMICOLON);
            return new VarListNode(type, idListNode, VarDecl());
            break;
        }
        default:
        {
            printf("error(VarDecl), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdList()
{
    switch (tok.token)
    {
        case LPARENT:
        case STAR:
        {
            PointerNode *pointer = Pointer();
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO NULL->GET_LEXEME()?
            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
            break;
        }
        default:
        {
            printf("error(IdList), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdListAUX()
{
    switch (tok.token)
    {
        case COMMA:
        {
            eat(COMMA);
            PointerNode *pointer = Pointer();
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO NULL->GET_LEXEME()?
            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
            break;
        }
        default:
        {
            printf("error(IdListAUX), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::idExprFollowSet[] = {COMMA, SEMICOLON, RPARENT};
void Parser::IdExpr() // TODO OBSOLETO?
{
    switch (tok.token)
    {
        case STAR:
        {
            Pointer();
            eat(ID);
            Array();
            break;
        }
        case LPARENT:
        {
            eat(LPARENT);
            IdList();
            eat(RPARENT);
            break;
        }
        default:
        {
            printf("error(IdExpr), Token error: %s \n", token_id_to_name(tok.token));
        }
    }
}

int Parser::pointerFollowSet[] = {RPARENT, ID, LBRACE, COMMA};
PointerNode *Parser::Pointer()
{
    switch (tok.token)
    {
        case STAR:
        {
            eat(STAR);
            return new PointerNode();
            break;
        }
        default:
        {
            printf("error(Pointer), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT};
ArrayNode *Parser::Array()
{
    switch (tok.token)
    {
        case LBRACE:
        {
            eat(LBRACE);
            eat(NUMINT);
            TokenNode *numInt = new TokenNode(NUMINT, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            eat(RBRACE);
            return new ArrayNode(numInt);
            break;
        }
        default:
        {
            printf("error(Array), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::formalListFollowSet[] = {RPARENT};
FormalListNode *Parser::FormalList()
{
    switch (tok.token)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            ArrayNode *array = Array();
            //FormalRest();
            return new FormalListNode(type, pointer, id, array, FormalRest());
            break;
        }
        default:
        {
            printf("error(FormalList), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::formalRestFollowSet[] = {RPARENT};
FormalListNode *Parser::FormalRest()
{
    switch (tok.token)
    {
        case COMMA:
        {
            eat(COMMA);
            TypeNode *typeNode = Type();
            PointerNode *pointer = Pointer();
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr);// TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            ArrayNode *array = Array();
            return new FormalListNode(typeNode, pointer, id, array, FormalRest());
            break;
        }
        default:
        {
            printf("error(FormalRest), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::typeFollowSet[] = {LPARENT, STAR, ID};
TypeNode *Parser::Type()
{
    TypeNode *type = nullptr;
    switch (tok.token)
    {
        case INT:
        {
            eat(INT);
            type = new TypeNode(new TokenNode(INT, nullptr));
            return type;
            break;
        }
        case FLOAT:
        {
            eat(FLOAT);
            type = new TypeNode(new TokenNode(FLOAT, nullptr));
            return type;
            break;
        }
        case BOOL:
        {
            eat(BOOL);
            type = new TypeNode(new TokenNode(BOOL, nullptr));
            return type;
            break;
        }
        case ID:
        {
            eat(ID);
            type = new TypeNode(new TokenNode(FLOAT, nullptr)); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            return type;
            break;
        }
        case CHAR:
        {
            eat(CHAR);
            type = new TypeNode(new TokenNode(CHAR, nullptr));
            return type;
            break;
        }
        default:
        {
            printf("error(Type), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::stmtListFollowSet[] = {RBRACKET, CASE};
StmtListNode *Parser::StmtList()
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
        case LPARENT:
        {
            StmtListNode *stmtListNode = new StmtListNode(Stmt(), StmtListAUX());
            return stmtListNode;
            break;
        }
        default:
        {
            printf("error(StmtList), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE};
StmtListNode *Parser::StmtListAUX()
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
        case LPARENT:
        {
            return StmtList();
            break;
        }
        default:
        {
            printf("error(StmtListAux), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::stmtFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                               ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
StmtNode *Parser::Stmt()
{
    switch (tok.token)
    {
        case IF: // TODO CONFERIR IFEXPR
        {
            eat(IF);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            //ExprAssign();
            eat(RPARENT);
            StmtNode *ifStmt = Stmt();
            StmtNode *elseStmt = ElseStmt();
            return new IfNode(exp, ifStmt, elseStmt);
            break;
        }
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
        case LPARENT:
        {
            return StmtAUX();
            break;
        }
        default:
        {
            printf("error(Stmt), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::stmtAUXFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
StmtNode *Parser::StmtAUX()
{
    ExpNode *exp = nullptr;
    switch (tok.token)
    {
        case WHILE:
        {
            eat(WHILE);
            eat(LPARENT);
            exp = ExprAssign();
            eat(RPARENT);
            return new WhileNode(exp, Stmt());
            //Stmt();
            break;
        }
        case SWITCH:
        {
            eat(SWITCH);
            eat(LPARENT);
            exp = ExprAssign();
            eat(RPARENT);
            eat(LBRACKET);
            CaseBlockNode *cbNode = CaseBlock();
            eat(RBRACKET);
            return new SwitchNode(exp, cbNode);
            break;
        }
        case BREAK:
        {
            eat(BREAK);
            eat(SEMICOLON);
            return new BreakNode();
            break;
        }
        case PRINT:
        {
            eat(PRINT);
            eat(LPARENT);
            ExpListNode *expList = ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            return new PrintLnNode(expList);
            break;
        }
        case READLN:
        {
            eat(READLN);
            eat(LPARENT);
            exp = ExprAssign();
            eat(RPARENT);
            eat(SEMICOLON);
            return new ReadNode(exp);
            break;
        }
        case RETURN:
        {
            eat(RETURN);
            exp = ExprAssign();
            eat(SEMICOLON);
            return new ReturnNode(exp);
            break;
        }
        case THROW:
        {
            eat(THROW);
            eat(SEMICOLON);
            return new ThrowNode();
            break;
        }
        case LBRACE:
        {
            eat(LBRACE);
            StmtListNode *stmtList = StmtList();
            eat(RBRACKET);
            return stmtList;
            break;
        }
        case ID:
        {
            eat(ID);
            eat(LPARENT);
            ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            break;
        }
        case TRY:
        {
            eat(TRY);
            StmtNode *tryStmt = Stmt();
            eat(CATCH);
            eat(LPARENT);
            eat(DOT);
            eat(DOT);
            eat(DOT);
            eat(RPARENT);
            return new TryNode(tryStmt, Stmt());
            break;
        }
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
        case LPARENT:
        {
            exp = ExprAssign();
            eat(SEMICOLON);
            return exp;
            break;
        }
        default:
        {
            printf("error(StmtAUX), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

StmtNode *Parser::ElseStmt()
{
    switch (tok.token)
    {
        case ELSE:
        {
            eat(ELSE);
            return Stmt();
        }
        default:
        {
            printf("error(elseStmt), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::ifExprFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                 ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
StmtNode *Parser::IFExpr() // TODO OBSOLETO?
{
    switch (tok.token)
    {
        case IF:
        {
            eat(IF);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            StmtNode *ifStmt = Stmt();
            //IFExpr();
            break;
        }
        case ELSE:
        {
            eat(ELSE);
            StmtNode *elseStmt = Stmt();
            return Stmt();
            break;
        }
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
        case LPARENT:
        {
            StmtAUX();
            IFExpr();
            break;
        }
        default:
        {
            printf("error(IFExpr), Token error: %s \n", token_id_to_name(tok.token));
        }
    }
}

int Parser::caseBlockFollowSet[] = {RBRACKET};
CaseBlockNode *Parser::CaseBlock()
{
    switch (tok.token)
    {
        case CASE:
        {
            eat(CASE);
            eat(NUMINT);
            TokenNode *token = new TokenNode(NUMINT, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            eat(COLON);
            return CaseBlockAUX(token);
            break;
        }
        default:
        {
            printf("error(caseBlock), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::caseBlockAUXFollowSet[] = {RBRACKET};
CaseBlockNode *Parser::CaseBlockAUX(TokenNode *num)
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
        case LPARENT:
        {
            return new CaseBlockNode(num, StmtList(), CaseBlock());
            //StmtList();
            //CaseBlock();
            break;
        }
        case CASE:
        {
            return CaseBlock();
            break;
        }
        default:
        {
            printf("error(caseBlockAUX), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprListFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprList()
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
        case LPARENT:
        {
            return ExprListTail();
            break;
        }
        default:
        {
            printf("error(ExprList), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprListTailFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprListTail()
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
        case LPARENT:
        {
            return new ExpListNode(ExprAssign(), ExprListTailAUX());
            break;
        }
        default:
        {
            printf("error(ExprListTail), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprListTailAUX()
{
    switch (tok.token)
    {
        case COMMA:
        {
            eat(COMMA);
            return ExprListTail();
            break;
        }
        default:
        {
            printf("error(ExprListTailAUX), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAssign()
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
        case LPARENT:
        {
            //ExprOr();
            return ExprAssignAUX(ExprOr());
            break;
        }
        default:
        {
            printf("error(ExprAssign), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAssignAUX(ExpNode *expr)
{
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case EQ:
        case ASSIGN:
        {
            eat(ASSIGN);
            exp2 = ExprOr();
            node = new AssignNode(expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprAssignAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprOr()
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
        case LPARENT:
        {
            //ExprAnd();
            return ExprOrAUX(ExprAnd());
            break;
        }
        default:
        {
            printf("error(ExprOr), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprOrAUXFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprOrAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case OR:
        {
            eat(OR);
            token = new TokenNode(OR, nullptr);
            exp2 = ExprAnd();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprOrAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAnd()
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
        case LPARENT:
        {
            //ExprEquality();
            return ExprAndAUX(ExprEquality());
            break;
        }
        default:
        {
            printf("error(ExprAnd), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAndAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case AND:
        {
            eat(AND);
            token = new TokenNode(AND, nullptr);
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprAndAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprEquality()
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
        case LPARENT:
        {
            //ExprRelational();
            return ExprEqualityAUX(ExprRelational());
            break;
        }
        default:
        {
            printf("error(ExprEquality), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprEqualityAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case EQ:
        {
            eat(EQ);
            token = new TokenNode(EQ, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case NE:
        {
            eat(NE);
            token = new TokenNode(NE, nullptr);
            exp2 = ExprRelational();;
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprEqualityAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprRelational()
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
        case LPARENT:
        {
            //ExprAdditive();
            return ExprRelationalAUX(ExprAdditive());
            break;
        }
        default:
        {
            printf("error(ExprRelational), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprRelationalAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case LT:
        {
            eat(LT);
            token = new TokenNode(LT, nullptr);
            exp2 = ExprAdditive();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GT:
        {
            eat(GT);
            token = new TokenNode(GT, nullptr);
            exp2 = ExprAdditive();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case LE:
        {
            eat(LE);
            token = new TokenNode(LE, nullptr);
            exp2 = ExprAdditive();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GE:
        {
            eat(GE);
            token = new TokenNode(GE, nullptr);
            exp2 = ExprAdditive();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprRelationalAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAdditive()
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
        case LPARENT:
        {
            //ExprMultiplicative();
            return ExprAdditiveAUX(ExprMultiplicative());
            break;
        }
        default:
        {
            printf("error(ExprAdditive), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAdditiveAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case PIPE:
        {
            eat(PIPE);
            token = new TokenNode(PIPE, nullptr);
            exp2 = ExprMultiplicative();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case PLUS:
        {
            eat(PLUS);
            token = new TokenNode(PLUS, nullptr);
            exp2 = ExprMultiplicative();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case MINUS:
        {
            eat(MINUS);
            token = new TokenNode(MINUS, nullptr);
            exp2 = ExprMultiplicative();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprAdditiveAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON,
                                             COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprMultiplicative()
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
        case LPARENT:
        {
            //ExprUnary();
            return ExprMultiplicativeAUX(ExprUnary());
            break;
        }
        default:
        {
            printf("error(ExprMultiplicative), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::exprMultiplicativeAUXFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                                EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprMultiplicativeAUX(ExpNode *expr)
{
    TokenNode *token = nullptr;
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok.token)
    {
        case ADDRESS:
        {
            eat(ADDRESS);
            token = new TokenNode(ADDRESS, nullptr);
            exp2 = ExprUnary();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case STAR:
        {
            eat(STAR);
            token = new TokenNode(STAR, nullptr);
            exp2 = ExprUnary();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case SLASH:
        {
            eat(SLASH);
            token = new TokenNode(SLASH, nullptr);
            exp2 = ExprUnary();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: printf("error(ExprMultiplicativeAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
    return node;
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                    EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprUnary()
{
    switch (tok.token)
    {
        case NOT:
        {
            eat(NOT);
            return new NotNode(PostFixExpr());
            break;
        }
        case PLUS:
        {
            eat(PLUS);
            return new SignNode(PostFixExpr());
            break;
        }
        case MINUS:
        {
            eat(MINUS);
            return new SignNode(PostFixExpr());
            break;
        }
        case STAR:
        {
            eat(STAR);
            return new PointerValueNode(PostFixExpr());
            break;
        }
        case ADDRESS:
        {
            eat(ADDRESS);
            return new AdressValueNode(PostFixExpr());
            break;
        }
        case ID:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LITERALCHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return PostFixExpr();
            break;
        }
        default:
        {
            printf("error(ExprUnary), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::Primary()
{
    switch (tok.token)
    {
        case ID:
        {
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            break;
        }
        case NUMINT:
        {
            eat(NUMINT);
            TokenNode *numInt = new TokenNode(NUMINT, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            break;
        }
        case NUMFLOAT:
        {
            eat(NUMFLOAT);
            TokenNode *numFloat = new TokenNode(NUMFLOAT, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            break;
        }
        case LITERAL:
        {
            eat(LITERAL);
            TokenNode *literalString = new TokenNode(LITERAL, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            break;
        }
        case LITERALCHAR:
        {
            eat(LITERALCHAR);
            TokenNode *literalChar = new TokenNode(LITERALCHAR, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            break;
        }
        case TRUE:
        {
            eat(TRUE);
            return new TrueNode();
            break;
        }
        case FALSE:
        {
            eat(FALSE);
            return new FalseNode();
            break;
        }
        case LPARENT:
        {
            eat(LPARENT);
            ExpNode *expr = ExprAssign();
            eat(RPARENT);
            return PostFixExprAUX();
            break;
        }
        default:
        {
            printf("error(Primary), Token error: %s \n", token_id_to_name(tok.token));
            return nullptr;
        }
    }
}

int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::PostFixExpr()
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
        case LPARENT:
        {
            Primary();
            return nullptr; // TODO PostFixExprAUX();
            break;
        }
        default:
        {
            printf("error(PostFixExpr), Token error: %s \n", token_id_to_name(tok.token));
        }
    }
}

int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::PostFixExprAUX() // PRIMARY_AUX || FIELD_ACCESS
{
    switch (tok.token)
    {
        case DOT:
        {
            eat(DOT);
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            return new PointerValueExpNode(nullptr, Primary());
            break;
        }
        case POINTER:
        {
            eat(POINTER);
            eat(ID);
            TokenNode *id = new TokenNode(ID, nullptr); // TODO SUBSTITUIR nullptr PELA FUNCAO GET_LEXEME();
            return new PointerValueExpNode(nullptr, Primary());
            break;
        }
        case LBRACKET:
        {
            eat(LBRACKET);
            //ExprAssign();
            ExpNode *index = ExprAssign();
            eat(RBRACKET);
            return new ArrayAccessNode(index, PostFixExprAUX());
            break;
        }
        case LPARENT:
        {
            eat(LPARENT);
            //ExprList();
            ExpListNode *expList = ExprList();
            eat(RPARENT);
            return new CallNode(expList, PostFixExprAUX());
            break;
        }

            // TODO EPSILON
            // Epsilon default: printf("error(PostFixExprAUX), Token error: %s \n",token_id_to_name(tok.token));
    }
}
