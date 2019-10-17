//
// Created by jorao on 17/10/2019.
//

#include "Visitor.h"

PrintAST::PrintAST() {

}

void PrintAST::visit(ProgramNode *node) {
    Visitor::visit(node);
}
