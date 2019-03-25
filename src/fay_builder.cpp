#include <fay_builder.h>
#include <mirror_utils_log.h>
#include <string>

using namespace fay;

fay::FayBuilder::~FayBuilder()
{
	//清除临时的指令
	if(!this->_insts.empty())
	{
		for each(auto it in this->_insts)
		{
			if(it != nullptr)
				delete it;
		}
	}
}

void fay::FayBuilder::log(BuildLogLevel level, int line, int col, const std::string &msg)
{
}

std::string fay::FayBuilder::makeLabel()
{
	std::string label = "__label__" + std::to_string(this->labelIndex++);
	this->fun->labels()->addLabel(label);
	return label;
}

void fay::FayBuilder::useLabel(const std::string &label, int32_t* target)
{
	this->fun->labels()->addTarget(label, target);
}

void fay::FayBuilder::fixedLabel(const std::string &label)
{
	this->fun->labels()->setPos(label, this->instsSize());
}

void fay::FayBuilder::addUsing(const std::string &packageName)
{
	this->_usings.push_back(packageName);
}

void fay::FayBuilder::addInst(FayInst* inst)
{
	this->_insts.push_back(inst);
}

void fay::FayBuilder::optimizeInsts()
{
	//TODO 这里有对代码进行优化的过程
	this->fun->insts(this->_insts);
	this->_insts.clear();
}

PTR(FayVarDef) fay::FayBuilder::findVar(const std::string &name)
{
	auto var = this->fun->findVar(name);

	if(var) return var;



	return var;
}

pos_t fay::FayBuilder::findVarIndex(const std::string &name)
{
	return this->fun->getVarIndex(name);
}

