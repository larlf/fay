#include <fay_vm.h>

using namespace fay;

void fay::FayVM::run(std::vector<fay::FayInst *> *insts, PTR(FayLib) lib)
{
	for each(FayInst* inst in *insts)
	{
		switch (inst->type())
		{
			case InstType::PushString:
				this->stack.push(new FayValue(((InstPushString *)inst)->p1()));
				break;
			case InstType::Call:
			{
				break;
			}
		}
	}
}
