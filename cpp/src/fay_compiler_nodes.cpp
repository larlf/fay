#include "fay_compiler_nodes.h"
#include "fay_compiler.h"
#include <iostream>
#include <fstream>

using namespace fay::compiler;

fay::compiler::VarStmtNode::VarStmtNode(ASTNode *ast)
{
	this->id = ast->childNode(0)->text();
	this->type = FayConst::ValueTypeValue[ast->childNode(1)->text()];
}

void fay::compiler::VarStmtNode::dump(TextBuilder *tb)
{

}
