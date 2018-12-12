#include <fay_builder.h>
#include <mirror_utils_log.h>
#include <string>

using namespace fay;

fay::FayBuilder::~FayBuilder()
{
	//清除临时的指令
	if(this->_insts.size() > 0)
	{
		for each(auto it in this->_insts)
		{
			if(it != nullptr)
				delete it;
		}
	}
}

void fay::FayBuilder::log(BuildLogLevel level, int line, int col, const std::string & msg)
{
}

std::string fay::FayBuilder::makeLabel()
{
	std::string label = "__label__" + std::to_string(this->labelIndex++);
	this->_fun->labels()->addLabel(label);
	return label;
}

void fay::FayBuilder::useLabel(const std::string & label, int32_t * target)
{
	this->_fun->labels()->addTarget(label, target);
}

void fay::FayBuilder::fixedLabel(const std::string & label)
{
	this->_fun->labels()->setPos(label, this->instsSize());
}

void fay::FayBuilder::beginFile(const std::string &filename)
{
	this->_filename = filename;
	this->_usings.clear();
}

void fay::FayBuilder::endFile()
{
	this->_filename = "";
}

void fay::FayBuilder::addUsing(const std::string &packageName)
{
	this->_usings.push_back(packageName);
}

void fay::FayBuilder::beginLib(const std::string &name)
{
	this->_lib = MKPTR(FayLib)(name, 0, 0);
	FayLang::Domain.addLib(this->_lib);
}

pos_t fay::FayBuilder::addClass(PTR(FayInstClass) clazz)
{
	this->_class = clazz;
	return this->_lib->addClass(clazz);
}

void fay::FayBuilder::bindClass(pos_t index)
{
	this->_class = TOPTR(FayInstClass, FayLang::Domain.findClass(index));
}

std::vector<PTR(FayClass)> fay::FayBuilder::findClass(const std::string & name)
{
	return FayLang::Domain.findClass(this->_usings, name);
}

pos_t fay::FayBuilder::addFun(PTR(FayInstFun) fun)
{
	this->_fun = fun;
	return this->_class->addFun(this->_fun);
}

void fay::FayBuilder::bindFun(pos_t index, bool isStatic)
{
	if(index < 0)
	{
		LOG_ERROR("Bad fun index : " << index);
		return;
	}

	this->_fun = TOPTR(FayInstFun, this->_class->findFun(index, isStatic));

	//清空指令集
	if(this->_insts.size() > 0)
		this->_insts.clear();
}

void fay::FayBuilder::addInst(FayInst* inst)
{
	this->_insts.push_back(inst);
}

void fay::FayBuilder::optimizeInsts()
{
	//TODO 这里有对代码进行优化的过程
	this->_fun->insts(this->_insts);
	this->_insts.clear();
}

PTR(FayVarDef) fay::FayBuilder::findVar(const std::string &name)
{
	auto var = this->_fun->findVar(name);

	if (var) return var;



	return var;
}

pos_t fay::FayBuilder::findVarIndex(const std::string &name)
{
	return this->_fun->getVarIndex(name);
}

