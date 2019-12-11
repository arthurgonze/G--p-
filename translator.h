#ifndef COMPILADOR_2019_3_TRANSLATOR_H
#define COMPILADOR_2019_3_TRANSLATOR_H

#include "intermediate_code.h"
// AST NODES

class ASTNode;

class TokenNode;

class ProgramNode;

class VarDeclNode;

class IdListNode;

class FunctionListNode;

class FunctionNode;

class FormalListNode;

class TypeDeclNode;

class TypeNode;

class PointerNode;

class StmtListNode;

class StmtNode;

class IfNode;

class WhileNode;

class SwitchNode;

class BreakNode;

class PrintNode;

class ReadLnNode;

class ReturnNode;

class CaseBlockNode;

class ThrowNode;

class ExpListNode;

class TryNode;

class ExpNode;

class AssignNode;

class NameExpNode;

class PointerExpNode;

class AddressValNode;

class PointerValNode;

class ArrayNode;

class ArrayCallNode;

class CallNode;

class AdditionOPNode;

class MultiplicationOPNode;

class BooleanOPNode;

class NotNode;

class SignNode;

class VarStmtNode;

class PrimaryNode;

void startTranslator(ProgramNode *ast);
void endTranslator();
class Translator {
private:
    Fragment* fragmentList;
public:
    Translator() = default;
    virtual ~Translator() = default;

    inline Fragment *getFragmentList() const { return fragmentList; }

    virtual void visit(ProgramNode *programNode);

    virtual void visit(TypeNode *typeNode);

    virtual void visit(FunctionListNode *functionListNode);

    virtual void visit(PointerNode *pointerNode);

    virtual void visit(StmtListNode *stmtListNode);

    virtual void visit(IfNode *ifNode);

    virtual void visit(WhileNode *whileNode);

    virtual void visit(SwitchNode *switchNode);

    virtual void visit(BreakNode *breakNode);

    virtual void visit(PrintNode *printNode);

    virtual void visit(ReadLnNode *readLnNode);

    virtual void visit(ReturnNode *returnNode);

    virtual void visit(CaseBlockNode *caseBlockNode);

    virtual void visit(ThrowNode *throwNode);

    virtual void visit(ExpListNode *expListNode);

    virtual void visit(TryNode *tryNode);

    virtual void visit(TokenNode *tokenNode);

    virtual void visit(AssignNode *assignNode);

    virtual void visit(ArrayNode *arrayNode);

    virtual void visit(CallNode *callNode);

    virtual void visit(AdditionOPNode *additionOPNode);

    virtual void visit(MultiplicationOPNode *multiplicationOPNode);

    virtual void visit(BooleanOPNode *booleanOPNode);

    virtual void visit(NotNode *notNode);

    virtual void visit(SignNode *signNode);

    virtual void visit(ArrayCallNode *arrayCallNode);

    virtual void visit(FormalListNode *formalListNode);

    virtual void visit(IdListNode *idListNode);

    virtual void visit(PrimaryNode *primaryNode);

    virtual void visit(VarStmtNode *varStmtNode);

    virtual void visit(FunctionNode *functionNode);

    virtual void visit(PointerExpNode *pointerExpNode);

    virtual void visit(NameExpNode *nameExpNode);

    virtual void visit(VarDeclNode *varDeclNode);

    virtual void visit(TypeDeclNode *typeDeclNode);

    virtual void visit(AddressValNode *addressValNode);

    virtual void visit(PointerValNode *pointerValNode);

    virtual void visit(StmtNode *stmtNode);
};


#endif //COMPILADOR_2019_3_TRANSLATOR_H
