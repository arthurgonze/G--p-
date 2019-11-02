//
// Created by jorao on 17/10/2019.
//

#include <iostream>
#include "Visitor.h"
#include "AST.h"
#include "../lexical/analyzer.h"

PrintAST::PrintAST()
{
    level = 0;
}

void PrintAST::printAST(const char *node_name)
{
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name << std::endl;
}

void PrintAST::printLexemeAST(const char *node_name, const char *aux )
{
    for (unsigned int i = 0; i < this->level; i++)
        std::cout << "    ";
    std::cout << "->" << node_name <<"."<< aux << std::endl;
}


void PrintAST::up_level()
{
    this->level++;

}

void PrintAST::down_level()
{
    this->level--;

}

/*
 * PrintAST functions
 */

void PrintAST::visit(ProgramNode *node)
{
    std::cout << "\n------------------------------" << std::endl;
    std::cout << "---- ABSTRACT SYNTAX TREE ----" << std::endl;
    std::cout << "------------------------------\n" << std::endl;
    this->printAST("PROGRAM");
    up_level();
    if (node->getFunctions()!=nullptr)
    {
        node->getFunctions()->accept(this);
    }
    if (node->getTypeList()!=nullptr)
    {
        node->getTypeList()->accept(this);
    }
    if (node->getVarList()!=nullptr)
    {
        node->getVarList()->accept(this);
    }
    down_level();
}

void PrintAST::visit(FunctionListNode *node)
{
    this->printAST("FUNCTION_LIST");
    up_level();
    if (node->getFunction()!=nullptr)
    {
        node->getFunction()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}

void PrintAST::visit(IdListNode *node)
{
    this->printAST("ID_LIST");
    up_level();
    if (node->getPointer()!=nullptr)
    {
        node->getPointer()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    if (node->getArray()!=nullptr)
    {
        node->getArray()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}

void PrintAST::visit(StmtListNode *node)
{
    this->printAST("STATEMENT_LIST");
    up_level();
    if (node->getStmt()!=nullptr)
    {
        node->getStmt()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();;
}

void PrintAST::visit(VarStmtNode *node)
{
    this->printAST("VAR_STMT");
    up_level();
    if (node->getDecl()!=nullptr)
    {
        node->getDecl()->accept(this);
    }
    if (node->getBody()!=nullptr)
    {
        node->getBody()->accept(this);
    }
    down_level();
}

void PrintAST::visit(IfNode *node)
{
    this->printAST("IF");
    up_level();
    if (node->getHead()!=nullptr)
    {
        node->getHead()->accept(this);
    }
    if (node->getTrueStmt()!=nullptr)
    {
        node->getTrueStmt()->accept(this);
    }
    if (node->getFalseStmt()!=nullptr)
    {
        node->getFalseStmt()->accept(this);
    }
    down_level();
}

void PrintAST::visit(WhileNode *node)
{
    this->printAST("WHILE");
    up_level();
    if (node->getHead()!=nullptr)
    {
        node->getHead()->accept(this);
    }
    if (node->getBody()!=nullptr)
    {
        node->getBody()->accept(this);
    }
    down_level();
}

void PrintAST::visit(SwitchNode *node)
{
    this->printAST("SWITCH");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    if (node->getBlock()!=nullptr)
    {
        node->getBlock()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PrintNode *node)
{
    this->printAST("PRINT_LN");
    up_level();
    if (node->getExpList()!=nullptr)
    {
        node->getExpList()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ReadLnNode *node)
{
    this->printAST("READ");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ReturnNode *node)
{
    this->printAST("RETURN");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(CaseBlockNode *node)
{
    this->printAST("CASE_BLOCK");
    up_level();
    if (node->getNum()!=nullptr)
    {
        node->getNum()->accept(this);
    }
    if (node->getStmt()!=nullptr)
    {
        node->getStmt()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ExpListNode *node)
{
    this->printAST("EXP_LIST");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}

void PrintAST::visit(TryNode *node)
{
    this->printAST("TRY");
    up_level();
    if (node->getTry()!=nullptr)
    {
        node->getTry()->accept(this);
    }
    if (node->getCatch()!=nullptr)
    {
        node->getCatch()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PrimaryNode *node)
{
    this->printAST("PRIMARY");
    up_level();
    if (node->getToken()!=nullptr)
    {
        node->getToken()->accept(this);
    }
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(CallNode *node)
{
    this->printAST("CALL");
    up_level();
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    if (node->getParameters()!=nullptr)
    {
        node->getParameters()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ArrayCallNode *node)
{
    this->printAST("ARRAY_ACCESS");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    if (node->getIndex()!=nullptr)
    {
        node->getIndex()->accept(this);
    }

    down_level();
}

void PrintAST::visit(ArrayNode *node)
{
    this->printAST("ARRAY");
    up_level();
    if (node->getNumInt()!=nullptr)
    {
        node->getNumInt()->accept(this);
    }
    down_level();
}

void PrintAST::visit(AssignNode *node)
{
    this->printAST("ASSIGN");
    up_level();
    if (node->getExp1()!=nullptr)
    {
        node->getExp1()->accept(this);
    }
    if (node->getExp2()!=nullptr)
    {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(AdditionOPNode *node)
{
    this->printAST(token_id_to_name(node->getOp()->getToken()));
    up_level();
    if (node->getExp1()!=nullptr)
    {
        node->getExp1()->accept(this);
    }
    if (node->getExp2()!=nullptr)
    {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(MultiplicationOPNode *node)
{
    this->printAST(token_id_to_name(node->getOp()->getToken()));
    up_level();
    if (node->getExp1()!=nullptr)
    {
        node->getExp1()->accept(this);
    }
    if (node->getExp2()!=nullptr)
    {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(BooleanOPNode *node)
{
    this->printAST(token_id_to_name(node->getOp()->getToken()));
    up_level();
    if (node->getExp1()!=nullptr)
    {
        node->getExp1()->accept(this);
    }
    if (node->getExp2()!=nullptr)
    {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(NotNode *node)
{
    this->printAST("NOT");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(SignNode *node)
{
    this->printAST("SIGN");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(FormalListNode *node)
{
    this->printAST("FORMAL_LIST");
    up_level();
    if (node->getType()!=nullptr)
    {
        node->getType()->accept(this);
    }
    if (node->getPointer()!=nullptr)
    {
        node->getPointer()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    if (node->getArray()!=nullptr)
    {
        node->getArray()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}

void PrintAST::visit(FunctionNode *node)
{
    this->printAST("FUNCTION");
    up_level();
    if (node->getType()!=nullptr)
    {
        node->getType()->accept(this);
    }
    if (node->getPointer()!=nullptr)
    {
        node->getPointer()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    if (node->getParameters()!=nullptr)
    {
        node->getParameters()->accept(this);
    }
    if (node->getLocal()!=nullptr)
    {
        node->getLocal()->accept(this);
    }
    if (node->getBody()!=nullptr)
    {
        node->getBody()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PointerExpNode *node)
{
    this->printAST("POINTER_EXP");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    down_level();
}

void PrintAST::visit(NameExpNode *node)
{
    this->printAST("NAME_EXP");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    down_level();
}

void PrintAST::visit(VarDeclNode *node)
{
    this->printAST("VAR_DECL");
    up_level();
    if (node->getType()!=nullptr)
    {
        node->getType()->accept(this);
    }
    if (node->getIdList()!=nullptr)
    {
        node->getIdList()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}
void PrintAST::visit(TypeDeclNode *node)
{
    this->printAST("TYPE_DECL");
    up_level();
    if (node->getDecl()!=nullptr)
    {
        node->getDecl()->accept(this);
    }
    if (node->getId()!=nullptr)
    {
        node->getId()->accept(this);
    }
    if (node->getNext()!=nullptr)
    {
        node->getNext()->accept(this);
    }
    down_level();
}
void PrintAST::visit(AddressValNode *node)
{
    this->printAST("ADDRESS_VAL");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}
void PrintAST::visit(PointerValNode *node)
{
    this->printAST("POINTER_VAL");
    up_level();
    if (node->getExp()!=nullptr)
    {
        node->getExp()->accept(this);
    }
    down_level();
}
void PrintAST::visit(StmtNode *node)
{
    this->printAST("STMT");
    up_level();
    if (node->getStmt()!=nullptr)
    {
        node->getStmt()->accept(this);
    }
    down_level();
}


/*
 * TERMINAL NODES
 */
void PrintAST::visit(PointerNode *node)
{
    this->printAST("POINTER");
}

void PrintAST::visit(BreakNode *node)
{
    this->printAST("BREAK");
}

void PrintAST::visit(ThrowNode *node)
{
    this->printAST("THROW");
}

void PrintAST::visit(TypeNode *node)
{
    if (node->getId()->getLex()!=nullptr)
    {
        this->printLexemeAST(token_id_to_name(node->getId()->getToken()), node->getId()->getLex());
    }
    else
    {
        this->printAST(token_id_to_name(node->getId()->getToken()));
    }

}

void PrintAST::visit(TokenNode *node)
{
    this->printLexemeAST(token_id_to_name(node->getToken()), node->getLex());
}




