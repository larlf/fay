#include "fay_compiler.h"
#include "fay_compiler_ast.h"

using namespace fay::compiler;


fay::compiler::Compiler::Compiler()
{
	this->currentScope = new Scope();
}

void fay::compiler::Compiler::startScope()
{
	this->currentScope = new Scope(this->currentScope);
}

void fay::compiler::Compiler::endScope()
{
	if (this->currentScope->parentScope != nullptr)
	{
		this->currentScope = this->currentScope->parentScope;
	}
}

void fay::compiler::Compiler::addVar(const std::string &name, const std::string &type)
{
	ValueType vt = FayUtils::StringToValueType(type);
	this->currentScope->addNewVar(name, vt);
}

void fay::compiler::Compiler::addInstruction(Instruction* inst)
{
	this->program.push_back(inst);
}

int fay::compiler::Scope::addNewVar(const std::string &name, ValueType type)
{
	for (auto it = this->vars.begin(); it != this->vars.end(); ++it)
	{
		if ((*it)->name == name)
		{
			LOG_ERROR("Repert var : " << name);
			return (*it)->index;
		}
	}

	VarDefine* var = new VarDefine();
	var->name = name;
	var->index = this->vars.size();
	var->type = type;
	this->vars.push_back(var);
	return var->index;
}

VarDefine* fay::compiler::Scope::getVarDefine(const std::string &name)
{
	for (auto it = this->vars.begin(); it != this->vars.end(); ++it)
	{
		if ((*it)->name == name)
			return *it;
	}

	return nullptr;
}

fay::compiler::AppDomain::AppDomain()
{
	this->_scope = new Scope;
}

fay::compiler::AppDomain::~AppDomain()
{
	delete this->_scope;
	this->_scope = nullptr;
}

fay::compiler::Scope* fay::compiler::AppDomain::currentScope()
{
	return this->_scope;
}

void fay::compiler::AppDomain::run()
{

}
