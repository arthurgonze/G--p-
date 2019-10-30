#include "Parser.h"
#include "Visitor.h"

PrintAST *visitor;
ProgramNode *program;

ProgramNode *Parser::StartParser()
{
    advance();
    program = Program(nullptr, nullptr, nullptr);
    visitor = new PrintAST();
    program->accept(visitor);
    delete (visitor);
    return program;

}

void Parser::eat(int t)
{
    if (tok==t)
    {
        if (t==ID || t==NUMINT || t==NUMFLOAT)
        {
            fprintf(stdout, "MATCH - %s.%s\n", token_id_to_name(t), lexical_analyzer_last_lexeme());
        }
        else
        {
            fprintf(stdout, "MATCH - %s\n", token_id_to_name(t));
        }
        advance();
    }
    else
    {
        fprintf(stderr, "[SYNTACTICAL ERROR] error(eat), Token error: Esperado: %s, Processado: %s \n", token_id_to_name(t), token_id_to_name(tok));
    }

}

int Parser::programFollowSet[] = {-1};
ProgramNode *Parser::Program(FunctionListNode *functionList, TypeDeclNode *typeList, VarDeclNode *varList) // TODO 3param = VarDeclNode *varlist?
{
    switch (tok)
    {
        case TYPEDEF:
        {
            eat(TYPEDEF);
            eat(STRUCT);
            eat(LBRACE);
            TypeNode *type = Type();
            IdListNode *idList = IdList();
            eat(SEMICOLON);
            VarDeclNode *varListNode = new VarDeclNode(type, idList, VarDecl());

            eat(RBRACE);

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            eat(SEMICOLON);
            typeList = new TypeDeclNode(varListNode, id, typeList);
            typeList = TypeDecl(typeList);
            return Program(functionList, typeList, varList);
            break;
        }
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            ASTNode *ast = ProgramAUX(type, pointer, id, varList);
            return ProgramList(functionList, typeList, varList);
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
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Program), Token error: %s \n", token_id_to_name(tok));
            return new ProgramNode(functionList, typeList, varList);
        }
    }
}

int Parser::programAUXFollowSet[] = {-1};
ASTNode *Parser::ProgramAUX(TypeNode *type, PointerNode *pointer, TokenNode *id, VarDeclNode *varFunc)
{
    switch (tok)
    {
        case LPARENT:
        {
            eat(LPARENT);
            FormalListNode *parameters = FormalList();
            eat(RPARENT);
            eat(LBRACE);
            VarStmtNode *varStmtNode = VarStmt(nullptr);
            FunctionNode *func = nullptr;
            eat(RBRACE);
            return func;
            break;
        }
        case COMMA:
        case LBRACE:
        {
            ArrayNode *array = Array();
            IdListNode *listNode = IdListAUX();
            IdListNode *idList = new IdListNode(pointer, id, array, IdListAUX());
            eat(SEMICOLON);
            VarDeclNode *list = new VarDeclNode(type, idList, varFunc);
            //return new VarFuncListNode(new VarDeclNode(type, new IdListNode(pointer, id, array, listNode), nullptr), varFunc);
            return list;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ProgramAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::programListFollowSet[] = {-1};
ProgramNode *Parser::ProgramList(FunctionListNode *functions, TypeDeclNode *typelist, VarDeclNode *varlist)
{
    switch (tok)
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
            fprintf(stderr, "[SYNTACTICAL ERROR] errorProgramList(), Token error: %s \n", token_id_to_name(tok));
            return new ProgramNode(functions, typelist, varlist);
            break;
        }
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
TypeDeclNode *Parser::TypeDecl(TypeDeclNode *typeListNode)
{
    switch (tok)
    {
        case TYPEDEF:
        {
            eat(TYPEDEF);
            eat(STRUCT);
            eat(LBRACE);
            TypeNode *typeNode = Type();
            IdListNode *idListNode = IdList();
            eat(SEMICOLON);
            VarDeclNode *varListNode = new VarDeclNode(typeNode, idListNode, VarDecl());
            eat(RBRACKET);

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            eat(SEMICOLON);
            typeListNode = new TypeDeclNode(varListNode, id, typeListNode);
            return TypeDecl(typeListNode);
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(TypeDecl), Token error: %s \n", token_id_to_name(tok));
            return typeListNode;
        }
    }
}

int Parser::varDeclFollowSet[] = {IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS, ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, RBRACKET};
VarDeclNode *Parser::VarDecl()
{
    switch (tok)
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
            VarDeclNode *varList = new VarDeclNode(type, idListNode, VarDecl());
            return varList;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(VarDecl), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

VarStmtNode *Parser::VarStmt(VarDeclNode *varList)
{
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            return VarStmtAux(id, varList);
            break;
        }
        case NUMINT:
        case NUMFLOAT:
        case BOOL:
        case CHAR:
        {
            TypeNode *type = TypeAux();
            IdListNode *idList = IdList();
            eat(SEMICOLON);
            varList = new VarDeclNode(type, idList, varList);
            return VarStmt(varList);
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(VarStmt), Token error: %s \n", token_id_to_name(tok));
            return new VarStmtNode(varList, nullptr);
        }
    }
}

VarStmtNode *Parser::VarStmtAux(TokenNode *id, VarDeclNode *varList)
{
    switch (tok)
    {
        // TODO PrimaryFAT first
        case LPARENT:
        case DOT:
        case POINTER:
        case LBRACKET:
        case RPARENT:
        case SEMICOLON:
        {
            ExpNode *exp = nullptr; // TODO primaryFAT(id)
            exp = ExprAssignAUX(exp);
            exp = ExprMultiplicativeAUX(exp);
            exp = ExprAdditiveAUX(exp);
            exp = ExprRelationalAUX(exp);
            exp = ExprEqualityAUX(exp);
            exp = ExprOrAUX(exp);
            exp = ExprAndAUX(exp);

            eat(SEMICOLON);
            StmtListNode *stmtList = nullptr; // TODO new StmtListNode(new StmtNode(exp), StmtListAUX());
            VarStmtNode *varStmt = nullptr; // TODO new VarStmtNode(varList, stmtList);
            return varStmt;
            break;
        }
            // TODO IDListFirst
        case STAR:
        {
            IdListNode *idList = IdList();
            eat(SEMICOLON);
            varList = nullptr;// TODO new VarDeclNode(new TypeNode(id, idList, varList));
            return VarStmt(varList);
            break;
        }
        default:
        {
            return new VarStmtNode(varList, nullptr);
            break;
        }
    }
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdList()
{
    switch (tok)
    {
        case LPARENT:
        case STAR:
        {
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(IdList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdListAUX()
{
    switch (tok)
    {
        case COMMA:
        {
            eat(COMMA);
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(IdListAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::pointerFollowSet[] = {RPARENT, ID, LBRACE, COMMA};
PointerNode *Parser::Pointer()
{
    switch (tok)
    {
        case STAR:
        {
            eat(STAR);
            return new PointerNode();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Pointer), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::arrayFollowSet[] = {SEMICOLON, COMMA, RPARENT};
ArrayNode *Parser::Array()
{
    switch (tok)
    {
        case LBRACKET:
        {
            eat(LBRACKET);

            TokenNode *numInt = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            eat(NUMINT);

            eat(RBRACKET);
            return new ArrayNode(numInt);
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Array), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::formalListFollowSet[] = {RPARENT};
FormalListNode *Parser::FormalList()
{
    switch (tok)
    {
        case INT:
        case FLOAT:
        case BOOL:
        case ID:
        case CHAR:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            ArrayNode *array = Array();
            FormalListNode *list = new FormalListNode(type, pointer, id, array, FormalRest());
            return list;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(FormalList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::formalRestFollowSet[] = {RPARENT};
FormalListNode *Parser::FormalRest()
{
    switch (tok)
    {
        case COMMA:
        {
            eat(COMMA);
            TypeNode *typeNode = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            ArrayNode *array = Array();
            FormalListNode *formalList = new FormalListNode(typeNode, pointer, id, array, FormalRest());
            return formalList;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(FormalRest), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::typeFollowSet[] = {LPARENT, STAR, ID};
TypeNode *Parser::Type()
{
    switch (tok)
    {
        case INT:
        {
            TypeNode *typeNode = new TypeNode(new TokenNode(INT, nullptr));
            eat(INT);
            return typeNode;
        }
        case FLOAT:
        {
            TypeNode *typeNode = new TypeNode(new TokenNode(FLOAT, nullptr));
            eat(FLOAT);
            return typeNode;
        }
        case BOOL:
        {
            TypeNode *typeNode = new TypeNode(new TokenNode(BOOL, nullptr));
            eat(BOOL);
            return typeNode;
        }
        case ID:
        {
            TypeNode *typeNode = new TypeNode(new TokenNode(ID, lexical_analyzer_last_lexeme()));
            eat(ID);
            return typeNode;
        }
        case CHAR:
        {
            TypeNode *typeNode = new TypeNode(new TokenNode(CHAR, nullptr));
            eat(CHAR);
            return typeNode;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Type), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

TypeNode *Parser::TypeAux()
{
    switch (tok)
    {
        case INT:
        {
            TokenNode *type = new TokenNode(INT, nullptr);
            eat(INT);
            return new TypeNode(type);
        }
        case FLOAT:
        {
            TokenNode *type = new TokenNode(FLOAT, nullptr);
            eat(FLOAT);
            return new TypeNode(type);
        }
        case BOOL:
        {
            TokenNode *type = new TokenNode(BOOL, nullptr);
            eat(BOOL);
            return new TypeNode(type);
        }
        case CHAR:
        {
            TokenNode *type = new TokenNode(CHAR, nullptr);
            eat(CHAR);
            return new TypeNode(type);
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(TypeAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtListFollowSet[] = {RBRACKET, CASE};
StmtListNode *Parser::StmtList()
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
        case LPARENT:
        {
            StmtListNode *stmtListNode = new StmtListNode(Stmt(), StmtListAUX());
            return stmtListNode;
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(StmtList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtListAUXFollowSet[] = {RBRACKET, CASE};
StmtListNode *Parser::StmtListAUX()
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
        case LPARENT:
        {
            return StmtList();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(StmtListAux), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                               ID, NUMINT, NUMFLOAT, LITERAL, LITERALCHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
StmtNode *Parser::Stmt()
{
    switch (tok)
    {
        case IF:
        {
            eat(IF);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            StmtNode *ifStmt = Stmt();
            StmtNode *elseStmt = ElseStmt();
            IfNode *stmt = new IfNode(exp, ifStmt, elseStmt);
            return new StmtNode(stmt);
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
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Stmt), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::stmtAUXFollowSet[] = {ELSE, IF, WHILE, SWITCH, BREAK, PRINT, READLN, RETURN, THROW, LBRACE, TRY, NOT, PLUS, MINUS, STAR, ADDRESS,
                                  ID, NUMINT, NUMFLOAT, LITERAL, CHAR, TRUE, FALSE, LPARENT, CATCH, RBRACKET, CASE};
StmtNode *Parser::StmtAUX()
{
    switch (tok)
    {
        case WHILE:
        {
            eat(WHILE);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            WhileNode* aux = new WhileNode(exp, Stmt());
        }
        case SWITCH:
        {
            eat(SWITCH);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            eat(LBRACE);
            CaseBlockNode *cbNode = CaseBlock();
            eat(RBRACE);
            SwitchNode *aux = new SwitchNode(exp, cbNode);
            return new StmtNode(aux);
        }
        case BREAK:
        {
            eat(BREAK);
            eat(SEMICOLON);
            return new StmtNode(new BreakNode());
        }
        case PRINT:
        {
            eat(PRINT);
            eat(LPARENT);
            ExpListNode *expList = ExprList();
            eat(RPARENT);
            eat(SEMICOLON);
            PrintNode *print = new PrintNode(expList);
            return new StmtNode(print);
        }
        case READLN:
        {
            eat(READLN);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            eat(SEMICOLON);
            return new StmtNode(new ReadLnNode(exp));
        }
        case RETURN:
        {
            eat(RETURN);
            ExpNode *exp = ExprAssign();
            eat(SEMICOLON);
            return new StmtNode(new ReturnNode(exp));
        }
        case THROW:
        {
            eat(THROW);
            eat(SEMICOLON);
            return new StmtNode(new ThrowNode);
            break;
        }
        case LBRACE:
        {
            eat(LBRACE);
            StmtListNode *stmtList = StmtList();
            eat(RBRACKET);
            return new StmtNode(stmtList);
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
            StmtNode *exception = Stmt();
            return new StmtNode(new TryNode(tryStmt, exception));
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
            ExpNode *exp = ExprAssign();
            eat(SEMICOLON);
            return new StmtNode(exp);
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(StmtAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

StmtNode *Parser::ElseStmt()
{
    switch (tok)
    {
        case ELSE:
        {
            eat(ELSE);
            return Stmt();
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(elseStmt), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::caseBlockFollowSet[] = {RBRACKET};
CaseBlockNode *Parser::CaseBlock()
{
    switch (tok)
    {
        case CASE:
        {
            eat(CASE);

            TokenNode *token = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            eat(NUMINT);
            eat(COLON);
            return CaseBlockAUX(token);
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(caseBlock), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::caseBlockAUXFollowSet[] = {RBRACKET};
CaseBlockNode *Parser::CaseBlockAUX(TokenNode *num)
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
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case CHAR:
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return new CaseBlockNode(num, StmtList(), CaseBlock());
            break;
        }
        case CASE:
        {
            return CaseBlock();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(caseBlockAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprList()
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
        case LPARENT:
        {
            return ExprListTail();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListTailFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprListTail()
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
        case LPARENT:
        {
            return new ExpListNode(ExprAssign(), ExprListTailAUX());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprListTail), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprListTailAUX()
{
    switch (tok)
    {
        case COMMA:
        {
            eat(COMMA);
            return ExprListTail();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprListTailAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAssignFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAssign()
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
        case LPARENT:
        {
            return ExprAssignAUX(ExprOr());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAssign), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAssignAUX(ExpNode *expr)
{
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case EQ:
        case ASSIGN:
        {
            eat(ASSIGN);
            exp2 = ExprOr();
            node = new AssignNode(expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAssignAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprOr()
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
        case LPARENT:
        {
            return ExprOrAUX(ExprAnd());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprOr), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
    {
        case OR:
        {
            eat(OR);
            token = new TokenNode(OR, nullptr);
            exp2 = ExprAnd();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprOrAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAnd()
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
        case LPARENT:
        {
            return ExprAndAUX(ExprEquality());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAnd), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
    {
        case AND:
        {
            eat(AND);
            token = new TokenNode(AND, nullptr);
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAndAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprEquality()
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
        case LPARENT:
        {
            return ExprEqualityAUX(ExprRelational());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprEquality), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
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
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprEqualityAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprRelational()
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
        case LPARENT:
        {
            return ExprRelationalAUX(ExprAdditive());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprRelational), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
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
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprRelationalAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAdditive()
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
        case LPARENT:
        {
            return ExprAdditiveAUX(ExprMultiplicative());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAdditive), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
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
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAdditiveAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON,
                                             COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprMultiplicative()
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
        case LPARENT:
        {
            return ExprMultiplicativeAUX(ExprUnary());
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprMultiplicative), Token error: %s \n", token_id_to_name(tok));
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
    switch (tok)
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
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprMultiplicativeAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                    EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprUnary()
{
    switch (tok)
    {
        case NOT:
        {
            //eat(NOT);
            return new NotNode(ExprUnary());
            break;
        }
        case PLUS:
        {
            //eat(PLUS);
            return ExprUnary();
            break;
        }
        case MINUS:
        {
            //eat(MINUS);
            return new SignNode(ExprUnary());
            break;
        }
        case STAR:
        {
            //eat(STAR);
            return new PointerValNode(ExprUnary());
            break;
        }
        case ADDRESS:
        {
            //eat(ADDRESS);
            return new AddressValNode(ExprUnary());
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
            return Primary();
            break;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprUnary), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::primaryFollowSet[] = {DOT, POINTER, LBRACE, LPARENT, ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE,
                                  EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::Primary()
{
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            return PostFixExpr(id);
        }
        case NUMINT:
        {
            TokenNode *numInt = new TokenNode(NUMINT, lexical_analyzer_last_lexeme());
            eat(NUMINT);
            return PostFixExprAUX(new PrimaryNode(numInt));
        }
        case NUMFLOAT:
        {
            TokenNode *numFloat = new TokenNode(NUMFLOAT, lexical_analyzer_last_lexeme());
            eat(NUMFLOAT);
            return PostFixExprAUX(new PrimaryNode(numFloat));
        }
        case LITERAL:
        {

            TokenNode *literalString = new TokenNode(LITERAL, lexical_analyzer_last_lexeme());
            eat(LITERAL);
            return PostFixExprAUX(new PrimaryNode(literalString));
        }
        case TRUE:
        {
            eat(TRUE);
            ExpNode *expNode = new PrimaryNode(new TokenNode(TRUE, nullptr));
            return PostFixExprAUX(expNode);
        }
        case FALSE:
        {
            eat(FALSE);
            ExpNode *expNode = new PrimaryNode(new TokenNode(FALSE, nullptr));
            return PostFixExprAUX(expNode);
            break;
        }
        case STAR:
        {
            eat(STAR);
            return PostFixExprAUX(new PointerValNode(ExprAssign()));
        }
        case ADDRESS:
        {
            eat(ADDRESS);
            return PostFixExprAUX(new PointerValNode(ExprAssign()));
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Primary), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::postFixExprAUXFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                         NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::PostFixExprAUX(ExpNode *exp)
{
    switch (tok)
    {
        case DOT:
        {
            eat(DOT);
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            return PostFixExprAUX(new NameExpNode(exp, id));
        }
        case POINTER:
        {
            eat(POINTER);
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            return PostFixExprAUX(new PointerExpNode(exp, id));
        }
        case LBRACKET:
        {
            eat(LBRACKET);
            ExpNode *index = ExprAssign();
            eat(RBRACKET);
            return PostFixExprAUX(new ArrayCallNode(exp, index));
        }
        default:
            fprintf(stderr, "[SYNTACTICAL ERROR] error(PostFixExprAUX), Token error: %s \n", token_id_to_name(tok));
            return exp;
    }
}

int Parser::postFixExprFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ,
                                      NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::PostFixExpr(TokenNode *id) //ok
{
    switch (tok)
    {
        case LPARENT:
        {
            eat(LPARENT);
            ExpListNode *expList = ExprList();
            eat(RPARENT);
            CallNode *callNode = new CallNode(id, expList);
            PrimaryNode *primary = new PrimaryNode(PostFixExprAUX(callNode));
            return primary;
        }
        case RPARENT:
        case SEMICOLON:
        case DOT:
        case POINTER:
        case LBRACKET:
        {
            return PostFixExprAUX(new PrimaryNode(id));
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(PostFixExpr), Token error: %s \n", token_id_to_name(tok));
            return new PrimaryNode(id);
        }
    }
}
