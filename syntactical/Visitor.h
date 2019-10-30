#ifndef COMPILADOR_2019_3_VISITOR_H
#define COMPILADOR_2019_3_VISITOR_H

#ifndef VISITOR_H_INCLUDED
#define VISITOR_H_INCLUDED

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

class Visitor
{
public:
    virtual ~Visitor() = default;;
    virtual void visit(ProgramNode *node) {};
    virtual void visit(TypeNode *node) {};
    virtual void visit(FunctionListNode *node) {};
    virtual void visit(PointerNode *node) {};
    virtual void visit(StmtListNode *node) {};
    virtual void visit(IfNode *node) {};
    virtual void visit(WhileNode *node) {};
    virtual void visit(SwitchNode *node) {};
    virtual void visit(BreakNode *node) {};
    virtual void visit(PrintNode *node) {};
    virtual void visit(ReadLnNode *node) {};
    virtual void visit(ReturnNode *node) {};
    virtual void visit(CaseBlockNode *node) {};
    virtual void visit(ThrowNode *node) {};
    virtual void visit(ExpListNode *node) {};
    virtual void visit(TryNode *node) {};
    virtual void visit(TokenNode *node) {};
    virtual void visit(AssignNode *node) {};
    virtual void visit(ArrayNode *node) {};
    virtual void visit(CallNode *node) {};
    virtual void visit(AdditionOPNode *node) {};
    virtual void visit(MultiplicationOPNode *node) {};
    virtual void visit(BooleanOPNode *node) {};
    virtual void visit(NotNode *node) {};
    virtual void visit(SignNode *node) {};
    virtual void visit(ArrayCallNode *node) {};
    virtual void visit(FormalListNode *node) {};
    virtual void visit(IdListNode *node) {};
    virtual void visit(PrimaryNode *node) {};
    virtual void visit(VarStmtNode *node) {};
    virtual void visit(FunctionNode *node) {};
    virtual void visit(PointerExpNode *node) {};
    virtual void visit(NameExpNode *node) {};
    virtual void visit(VarDeclNode *node) {};
    virtual void visit(TypeDeclNode *node) {};
    virtual void visit(AddressValNode *node) {};
    virtual void visit(PointerValNode *node) {};
    virtual void visit(StmtNode *node) {};

};

class PrintAST : public Visitor
{
private:
    unsigned int level;
    void printAST(const char *node_name, const char * aux);
    void up_level();
    void down_level();

public:
    PrintAST();

    void visit(ProgramNode *node) override;
    void visit(TypeNode *node) override;
    void visit(FunctionListNode *node) override;
    void visit(PointerNode *node) override;
    void visit(StmtListNode *node) override;
    void visit(IfNode *node) override;
    void visit(WhileNode *node) override;
    void visit(SwitchNode *node) override;
    void visit(BreakNode *node) override;
    void visit(PrintNode *node) override;
    void visit(ReadLnNode *node) override;
    void visit(ReturnNode *node) override;
    void visit(CaseBlockNode *node) override;
    void visit(ThrowNode *node) override;
    void visit(ExpListNode *node) override;
    void visit(TryNode *node) override;
    void visit(TokenNode *node) override;
    void visit(AssignNode *node) override;
    void visit(ArrayNode *node) override;
    void visit(CallNode *node) override;
    void visit(AdditionOPNode *node) override;
    void visit(MultiplicationOPNode *node) override;
    void visit(BooleanOPNode *node) override;
    void visit(NotNode *node) override;
    void visit(SignNode *node) override;
    void visit(ArrayCallNode *node)override;
    void visit(FormalListNode *node)override;
    void visit(IdListNode *node)override;
    void visit(PrimaryNode *node)override;
    void visit(VarStmtNode *node)override;
    void visit(FunctionNode *node) override;
    void visit(PointerExpNode *node) override;
    void visit(NameExpNode *node) override;

    // TODO
    void visit(VarDeclNode *node) override;
    void visit(TypeDeclNode *node) override;
    void visit(AddressValNode *node) override;
    void visit(PointerValNode *node) override;
    void visit(StmtNode *node) override;
};

#endif //VISITOR_H_INCLUDED

#endif //COMPILADOR_2019_3_VISITOR_H
