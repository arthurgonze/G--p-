//
// Created by jorao on 17/10/2019.
//

#include <iostream>
#include "Visitor.h"
#include "AST.h"
#include "../lexical/analyzer.h"


/*
 * TO DO:
 *
 * MUDAR TOKEN -> token_id_name
 * MUDAR LEXEME para node->get_lexeme
 *
 * AJUSTAR CADA NO PARA FAZER A FUNCAO
 */

PrintAST::PrintAST() {
    level = 0;
}

void PrintAST::printAST(const char *node_name, const char *aux = "") {
    for (unsigned int i = 0; i < this->level; i++) std::cout << "\t";
    std::cout << "->" << node_name << aux << std::endl;
}

void PrintAST::up_level() {
    this->level++;

}

void PrintAST::down_level() {
    this->level--;

}

/*
 * PrintAST functions
 */

void PrintAST::visit(ProgramNode *node) {
    std::cout << "\n------------------------------" << std::endl;
    std::cout << "---- ABSTRACT SYNTAX TREE ----" << std::endl;
    std::cout << "------------------------------\n" << std::endl;
    this->printAST("PROGRAM");//override; //TODO ARRUMAR O CHAR AUX
    up_level();
    if (node->getFuncList() != NULL) {
        node->getFuncList()->accept(this);
    }
    if (node->getTypeList() != NULL) {
        node->getTypeList()->accept(this);
    }
    if (node->getVarList() != NULL) {
        node->getVarList()->accept(this);
    }
    down_level();
}

void PrintAST::visit(VarListNode *node) {
    this->printAST("VAR_LIST", "c");
    up_level();
    if (node->get_type() != NULL) {
        node->get_type()->accept(this);
    }
    if (node->get_id_list() != NULL) {
        node->get_id_list()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

void PrintAST::visit(FunctionListNode *node) {
    this->printAST("FUNCTION_LIST", "aux");
    up_level();
    if (node->get_type() != NULL) {
        node->get_type()->accept(this);
    }
    if (node->get_pointer() != NULL) {
        node->get_pointer()->accept(this);
    }
    if (node->get_id() != NULL) {
        node->get_id()->accept(this);
    }
    if (node->get_formal_list() != NULL) {
        node->get_formal_list()->accept(this);
    }
    if (node->getVar_stmt() != NULL) {
        node->getVar_stmt()->accept(this);
    }
    down_level();
}

void PrintAST::visit(IdListNode *node) {
    this->printAST("ID_LIST", "aux");
    up_level();
    if (node->get_pointer() != NULL) {
        node->get_pointer()->accept(this);
    }
    if (node->get_id() != NULL) {
        node->get_id()->accept(this);
    }
    if (node->get_array() != NULL) {
        node->get_array()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

void PrintAST::visit(TypeListNode *node) {
    this->printAST("TYPE_LIST", "aux");
    up_level();
    if (node->getVarList() != NULL) {
        node->getVarList()->accept(this);
    }
    if (node->getId() != NULL) {
        node->getId()->accept(this);
    }
    if (node->getTypeList() != NULL) {
        node->getTypeList()->accept(this);
    }
    down_level();
}


void PrintAST::visit(StmtListNode *node) {
    this->printAST("STATEMENT_LIST", "aux");
    up_level();
    if (node->getStmtList() != NULL) {
        node->getStmtList()->accept(this);
    }
    if (node->getStmtNode() != NULL) {
        node->getStmtNode()->accept(this);
    }
    down_level();;
}

void PrintAST::visit(VarStmtNode *node) {
    this->printAST("VAR_STMT", "aux");
    up_level();
    if (node->get_decl() != NULL) {
        node->get_decl()->accept(this);
    }
    if (node->getStmt_list() != NULL) {
        node->getStmt_list()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

void PrintAST::visit(VarFuncListNode *node) {
    this->printAST("VAR_FUNC_LIST", "aux");
    up_level();
    if (node->get_decl() != NULL) {
        node->get_decl()->accept(this);
    }
    if (node->getFunc_list() != NULL) {
        node->getFunc_list()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

void PrintAST::visit(IfNode *node) {
    this->printAST("IF", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    if (node->getIf() != NULL) {
        node->getExp()->accept(this);
    }
    if (node->getElse() != NULL) {
        node->getElse()->accept(this);
    }
    down_level();
}

void PrintAST::visit(WhileNode *node) {
    this->printAST("WHILE", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    if (node->getStmt() != NULL) {
        node->getStmt()->accept(this);
    }
    down_level();
}

void PrintAST::visit(SwitchNode *node) {
    this->printAST("SWITCH", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getCaseBlock()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PrintLnNode *node) {
    this->printAST("PRINT_LN", "aux");
    up_level();
    if (node->getExpList() != NULL) {
        node->getExpList()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ReadNode *node) {
    this->printAST("READ", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ReturnNode *node) {
    this->printAST("RETURN", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(CaseBlockNode *node) {
    this->printAST("CASE_BLOCK", "aux");
    up_level();
    if (node->getNum() != NULL) {
        node->getNum()->accept(this);
    }
    if (node->getStmtList() != NULL) {
        node->getStmtList()->accept(this);
    }
    if (node->getCaseBlock() != NULL) {
        node->getCaseBlock()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ExpListNode *node) {
    this->printAST("EXP_LIST", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExpList()->accept(this);
    }
    down_level();
}

void PrintAST::visit(TryNode *node) {
    this->printAST("TRY", "aux");
    up_level();
    if (node->getTry() != NULL) {
        node->getTry()->accept(this);
    }
    if (node->getCatch() != NULL) {
        node->getCatch()->accept(this);
    }
    down_level();
}


void PrintAST::visit(PrimaryNode *node) {
    this->printAST("PRIMARY", "aux");
    up_level();
    if (node->get_exp() != NULL) {
        node->get_exp()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

void PrintAST::visit(CallNode *node) {
    this->printAST("CALL", "aux");
    up_level();
    if (node->get_exp() != NULL) {
        node->get_exp()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}


void PrintAST::visit(AdressValueNode *node) {
    this->printAST("ADDRESS_VALUE", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PointerValueNode *node) {
    this->printAST("POINTER_VALUE", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(PointerValueExpNode *node) {
    this->printAST("POINTER_VALUE_EXP", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(ArrayAccessNode *node) {
    this->printAST("ARRAY_ACCESS", "aux");
    up_level();
    if (node->get_index_exp() != NULL) {
        node->get_index_exp()->accept(this);
    }
    if (node->get_exp() != NULL) {
        node->get_exp()->accept(this);
    }

    down_level();
}

void PrintAST::visit(ArrayNode *node) {
    this->printAST("ARRAY", "aux");
    up_level();
    if (node->get_index_exp() != NULL) {
        node->get_index_exp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(AssignNode *node) {
    this->printAST("ASSIGN", "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(RelationalOPNode *node) {
    this->printAST(token_id_to_name(node->getOp()->getToken()), "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(AdditionOPNode *node) {
    this->printAST(token_id_to_name(node->getOp()->getToken()), "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(MultiplicationOPNode *node) {
    this->printAST(token_id_to_name(node->getOp()->getToken()), "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(BooleanOPNode *node) {
    this->printAST(token_id_to_name(node->getOp()->getToken()), "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}

void PrintAST::visit(BitwiseOPNode *node) {
    this->printAST(token_id_to_name(node->getOp()->getToken()), "aux");
    up_level();
    if (node->getExp1() != NULL) {
        node->getExp1()->accept(this);
    }
    if (node->getExp2() != NULL) {
        node->getExp2()->accept(this);
    }
    down_level();
}


void PrintAST::visit(NotNode *node) {

    this->printAST("NOT", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(SignNode *node) {

    this->printAST("SIGN", "aux");
    up_level();
    if (node->getExp() != NULL) {
        node->getExp()->accept(this);
    }
    down_level();
}

void PrintAST::visit(FormalListNode *node) {
    this->printAST("FORMAL_LIST", "aux");
    up_level();
    if (node->get_type() != NULL) {
        node->get_type()->accept(this);
    }
    if (node->get_pointer() != NULL) {
        node->get_pointer()->accept(this);
    }
    if (node->get_id() != NULL) {
        node->get_id()->accept(this);
    }
    if (node->get_array() != NULL) {
        node->get_array()->accept(this);
    }
    if (node->get_next() != NULL) {
        node->get_next()->accept(this);
    }
    down_level();
}

/*
 * TERMINAL NODES
 */
void PrintAST::visit(PointerNode *node) {
    this->printAST("POINTER", "aux");
}


void PrintAST::visit(BreakNode *node) {
    this->printAST("BREAK", "aux");
}


void PrintAST::visit(TrueNode *node) {

    this->printAST("TRUE", "aux");
}

void PrintAST::visit(FalseNode *node) {
    this->printAST("FALSE", "aux");
}

void PrintAST::visit(TypeNode *node) {
    if (node->getId()->getLex() != NULL) {
        this->printAST(token_id_to_name(node->getId()->getToken()), node->getId()->getLex());
    } else this->printAST(token_id_to_name(node->getId()->getToken()), "aux");

}

void PrintAST::visit(TokenNode *node) {
    this->printAST("ID.", node->getLex());

}

void PrintAST::visit(ThrowNode *node) {
    this->printAST("THROW", "aux");
}

