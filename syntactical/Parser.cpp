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
ProgramNode *Parser::Program(FunctionListNode *functionList, TypeDeclNode *typeList, VarDeclNode *varList) // OK
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
        case CHAR:
        case ID:
        {
            TypeNode *type = Type();
            PointerNode *pointer = Pointer();

            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);

            ASTNode *ast = ProgramAUX(type, pointer, id, varList);

            return ProgramList(functionList, typeList, varList);
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
ASTNode *Parser::ProgramAUX(TypeNode *type, PointerNode *pointer, TokenNode *id, VarDeclNode *varFunc) //OK
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
            IdListNode *idList = new IdListNode(pointer, id, array, IdListAUX());
            eat(SEMICOLON);
            VarDeclNode *list = new VarDeclNode(type, idList, varFunc);
            return list;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ProgramAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::programListFollowSet[] = {-1};
ProgramNode *Parser::ProgramList(FunctionListNode *functions, TypeDeclNode *typelist, VarDeclNode *varlist)// OK
{
    switch (tok)
    {
        case ENDOFFILE:
        {
            return new ProgramNode(functions, typelist, varlist);
        }
        default:
        {
            //fprintf(stderr, "[SYNTACTICAL ERROR] errorProgramList(), Token error: %s \n", token_id_to_name(tok));
            return Program(functions, typelist, varlist);
        }
    }
}

int Parser::typeDeclFollowSet[] = {INT, FLOAT, BOOL, ID, CHAR, TYPEDEF};
TypeDeclNode *Parser::TypeDecl(TypeDeclNode *typeListNode)//OK
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
VarDeclNode *Parser::VarDecl()//ok
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

VarStmtNode *Parser::VarStmt(VarDeclNode *varList)//ok
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
        case INT: //TODO NUMINT?
        case FLOAT: //TODO NUMFLOAT?
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
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case LPARENT:
        case ADDRESS:
        case STAR:
        case TRUE:
        case FALSE:
        {
            return new VarStmtNode(varList, StmtList());
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(VarStmt), Token error: %s \n", token_id_to_name(tok));
            return new VarStmtNode(varList, nullptr);
        }
    }
}

VarStmtNode *Parser::VarStmtAux(TokenNode *id, VarDeclNode *varList) //ok
{
    switch (tok)
    {
        case STAR:
        case ID:
        {
            IdListNode *idListNode = IdList();
            eat(SEMICOLON);
            varList = new VarDeclNode(new TypeNode(id), idListNode, varList);
            return VarStmt(varList);
        }
        case LPARENT:
        case DOT:
        case POINTER:
        case LBRACE:
        case PERCENT:
        case SLASH:
        case ASSIGN:
        case SEMICOLON:
        case RPARENT:
        case PIPE:
        case PLUS:
        case MINUS:
        case NOT:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        case ADDRESS:
        case TRUE:
        case FALSE:
        case EQ:
        case NE:
        case AND:
        case OR:
        {
            ExpNode *exp = PostFixExpr(id);
            exp = ExprMultiplicativeAUX(exp);
            exp = ExprAdditiveAUX(exp);
            exp = ExprRelationalAUX(exp);
            exp = ExprEqualityAUX(exp);
            exp = ExprAndAUX(exp);
            exp = ExprOrAUX(exp);
            exp = ExprAssignAUX(exp);

            eat(SEMICOLON);
            StmtListNode *stmtList = new StmtListNode(new StmtNode(exp), StmtListAUX());
            VarStmtNode *varStmtNode = new VarStmtNode(varList, stmtList);
            return varStmtNode;
        }
        default:
        {
            return new VarStmtNode(varList, nullptr);
        }
    }
}

int Parser::idListFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdList() //ok
{
    PointerNode *pointer = Pointer();
    switch (tok)
    {
        case ID:
        {
            TokenNode *id = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
            ArrayNode *array = Array();
            IdListNode *idListNode = new IdListNode(pointer, id, array, IdListAUX());
            return idListNode;
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(IdList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::idListAUXFollowSet[] = {SEMICOLON, RPARENT};
IdListNode *Parser::IdListAUX()//ok
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
PointerNode *Parser::Pointer()//ok
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
ArrayNode *Parser::Array()//ok
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
FormalListNode *Parser::FormalList()//ok
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
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(FormalList), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::formalRestFollowSet[] = {RPARENT};
FormalListNode *Parser::FormalRest()//ok
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
TypeNode *Parser::Type()//ok
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
        case ID:
        {
            TokenNode *type = new TokenNode(ID, lexical_analyzer_last_lexeme());
            eat(ID);
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
            fprintf(stderr, "[SYNTACTICAL ERROR] error(Type), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

TypeNode *Parser::TypeAux()//ok
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
StmtListNode *Parser::StmtList()//ok
{
    switch (tok)
    {
        //TODO case LITERALCHAR:?
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
StmtListNode *Parser::StmtListAUX()//OK
{
    switch (tok)
    {
        //TODO case LITERALCHAR:?
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
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return StmtList();
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
StmtNode *Parser::Stmt() //OK
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

            //TODO case LITERALCHAR:?
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
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return StmtAUX();
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
StmtNode *Parser::StmtAUX()//OK
{
    switch (tok)
    {
        case WHILE:
        {
            eat(WHILE);
            eat(LPARENT);
            ExpNode *exp = ExprAssign();
            eat(RPARENT);
            WhileNode *aux = new WhileNode(exp, Stmt());
            return new StmtNode(aux);
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
            BreakNode* breakNode = new BreakNode();
            eat(SEMICOLON);
            return new StmtNode(breakNode);
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
        case ID:
        case STAR:
        case ADDRESS:
        case NUMINT:
        case NUMFLOAT:
        case LITERAL:
        // TODO case LITERALCHAR:?
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

StmtNode *Parser::ElseStmt()//ok
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
CaseBlockNode *Parser::CaseBlock()//OK
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
CaseBlockNode *Parser::CaseBlockAUX(TokenNode *num)//OK
{
    switch (tok)
    {
        case CASE:
        {
            return CaseBlock();
        }

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
        //TODO case LITERALCHAR: ?
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return new CaseBlockNode(num, StmtList(), CaseBlock());
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(caseBlockAUX), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprList()//OK
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
        //TODO case LITERALCHAR:?
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
ExpListNode *Parser::ExprListTail()//OK
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
        // TODO case LITERALCHAR:?
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return new ExpListNode(ExprAssign(), ExprListTailAUX());
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprListTail), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprListTailAUXFollowSet[] = {RPARENT};
ExpListNode *Parser::ExprListTailAUX()//OK
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
        // TODO case LITERALCHAR:?
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprAssignAUX(ExprOr());
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAssign), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAssignAUXFollowSet[] = {SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAssignAUX(ExpNode *expr)//ok
{
    ExpNode *exp2 = nullptr;
    ExpNode *node = expr;
    switch (tok)
    {
        case ASSIGN:
        {
            eat(ASSIGN);
            exp2 = ExprAssign();
            node = new AssignNode(expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAssignAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprOrFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprOr()//ok
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
        // TODO case LITERALCHAR:?
        case TRUE:
        case FALSE:
        case LPARENT:
        {
            return ExprOrAUX(ExprAnd());
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprOr), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprOrAUXFollowSet[] = {EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprOrAUX(ExpNode *expr)//ok
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
            exp2 = ExprOr();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprOrAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAndFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAnd()//ok
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
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAnd), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAndAUXFollowSet[] = {OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAndAUX(ExpNode *expr)//ok
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
            exp2 = ExprAnd();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAndAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprEqualityFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprEquality()//ok
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
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprEquality), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprEqualityAUXFollowSet[] = {AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprEqualityAUX(ExpNode *expr)//ok
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
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case NE:
        {
            eat(NE);
            token = new TokenNode(NE, nullptr);
            exp2 = ExprEquality();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprEqualityAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprRelationalFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprRelational()//ok
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
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprRelational), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprRelationalAUXFollowSet[] = {EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprRelationalAUX(ExpNode *expr)//ok
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
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GT:
        {
            eat(GT);
            token = new TokenNode(GT, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case LE:
        {
            eat(LE);
            token = new TokenNode(LE, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
        case GE:
        {
            eat(GE);
            token = new TokenNode(GE, nullptr);
            exp2 = ExprRelational();
            node = new BooleanOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprRelationalAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprAdditiveFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAdditive()//ok
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
        }
        default:
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAdditive), Token error: %s \n", token_id_to_name(tok));
            return nullptr;
        }
    }
}

int Parser::exprAdditiveAUXFollowSet[] = {LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprAdditiveAUX(ExpNode *expr)//ok
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
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case PLUS:
        {
            eat(PLUS);
            token = new TokenNode(PLUS, nullptr);
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
        case MINUS:
        {
            eat(MINUS);
            token = new TokenNode(MINUS, nullptr);
            exp2 = ExprAdditive();
            node = new AdditionOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprAdditiveAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprMultiplicativeFollowSet[] = {PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR, EQ, SEMICOLON,
                                             COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprMultiplicative()//ok
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
ExpNode *Parser::ExprMultiplicativeAUX(ExpNode *expr)//ok
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
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case STAR:
        {
            eat(STAR);
            token = new TokenNode(STAR, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case SLASH:
        {
            eat(SLASH);
            token = new TokenNode(SLASH, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
        case PERCENT:
        {
            eat(PERCENT);
            token = new TokenNode(PERCENT, nullptr);
            exp2 = ExprMultiplicative();
            node = new MultiplicationOPNode(token, expr, exp2);
            break;
        }
            // Epsilon default: fprintf(stderr, "[SYNTACTICAL ERROR] error(ExprMultiplicativeAUX), Token error: %s \n",token_id_to_name(tok));
    }
    return node;
}

int Parser::exprUnaryFollowSet[] = {ADDRESS, STAR, SLASH, PIPE, PLUS, MINUS, LT, GT, LE, GE, EQ, NE, AND, OR,
                                    EQ, SEMICOLON, COMMA, RPARENT, RBRACE};
ExpNode *Parser::ExprUnary()//ok
{
    switch (tok)
    {
        case NOT:
        {
            eat(NOT);
            return new SignNode(ExprUnary());
            break;
        }
        case PLUS:
        {
            eat(PLUS);
            return ExprUnary();
            break;
        }
        case MINUS:
        {
            eat(MINUS);
            return new SignNode(ExprUnary());
            break;
        }
        case STAR:
        {
            eat(STAR);
            return new PointerValNode(ExprUnary());
            break;
        }
        case ADDRESS:
        {
            eat(ADDRESS);
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
            return PostFixExprAUX(new AddressValNode(ExprAssign()));
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
        {
            fprintf(stderr, "[SYNTACTICAL ERROR] error(PostFixExprAUX), Token error: %s \n", token_id_to_name(tok));
            return exp;
        }
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
