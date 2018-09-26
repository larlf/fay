#include "fay_vm.h"

using namespace fay;

fay::FayStack* fay::FayVM::stack = nullptr;

void fay::FayVM::Execute(std::vector<FayInst*> insts)
{
	FayVM::stack = new fay::FayStack();

	for (auto i = 0; i < insts.size(); ++i)
	{
		FayInst* inst = insts[i];

		switch (inst->type)
		{
			case 1 :
			{
				FayVM::stack->push(fay::FayStackItem(&((InstPushString*)inst)->str));
				break;
			}
			case 2:
			{
				InstStaticCall* inst2 = static_cast<InstStaticCall*>(inst);
				fay::InnerFun::Print(FayVM::stack, inst2->paramCount);
				break;
			}
		}
	}
}

void fay::FayStack::push(FayStackItem item)
{
	if (this->pos < this->items.size())
	{
		this->items[this->pos] = item;
		this->pos++;
	}
	else
	{
		this->items.push_back(item);
		this->pos++;
	}
}

fay::FayStackItem fay::FayStack::pop()
{
	if (this->pos >= 0)
	{
		this->pos--;
		return this->items[this->pos];
	}
}

void fay::InnerFun::Print(FayStack * stack, int argc)
{
	const std::string* str = stack->pop().value.strValue;
	LOG_DEBUG(*str);
	std::cout << *str << std::endl;
}
