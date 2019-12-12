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
    ~Translator() = default;

    inline Fragment *getFragmentList() const { return fragmentList; }

    void visit(ProgramNode *programNode);

    void visit(TypeNode *typeNode);

    void visit(FunctionListNode *functionListNode);

    void visit(PointerNode *pointerNode);

    void visit(StmtListNode *stmtListNode);

    void visit(IfNode *ifNode);

    void visit(WhileNode *whileNode);

    void visit(SwitchNode *switchNode);

    void visit(BreakNode *breakNode);

    void visit(PrintNode *printNode);

    void visit(ReadLnNode *readLnNode);

    void visit(ReturnNode *returnNode);

    void visit(CaseBlockNode *caseBlockNode);

    void visit(ThrowNode *throwNode);

    void visit(ExpListNode *expListNode);

    void visit(TryNode *tryNode);

    void visit(TokenNode *tokenNode);

    void visit(AssignNode *assignNode);

    void visit(ArrayNode *arrayNode);

    void visit(CallNode *callNode);

    void visit(AdditionOPNode *additionOPNode);

    void visit(MultiplicationOPNode *multiplicationOPNode);

    void visit(BooleanOPNode *booleanOPNode);

    void visit(NotNode *notNode);

    void visit(SignNode *signNode);

    void visit(ArrayCallNode *arrayCallNode);

    void visit(FormalListNode *formalListNode);

    void visit(IdListNode *idListNode);

    void visit(PrimaryNode *primaryNode);

    void visit(VarStmtNode *varStmtNode);

    void visit(FunctionNode *functionNode);

    void visit(PointerExpNode *pointerExpNode);

    void visit(NameExpNode *nameExpNode);

    void visit(VarDeclNode *varDeclNode);

    void visit(TypeDeclNode *typeDeclNode);

    void visit(AddressValNode *addressValNode);

    void visit(PointerValNode *pointerValNode);

    void visit(StmtNode *stmtNode);
};


#endif //COMPILADOR_2019_3_TRANSLATOR_H
