#include <fay_vm.h>

using namespace fay;

void fay::FayVM::_run(std::vector<FayInst*>* insts)
{
	for each(FayInst* inst in *insts)
	{
		switch (inst->type())
		{
			case InstType::PushString:
				this->stack.push(new FayValue(((InstPushString*)inst)->p1()));
				break;
			case InstType::Call:
			{
				InstCall* i = (InstCall*)inst;
				PTR(FayFun) fun=this->_domain->findFun(i->v1, i->v2);
				this->run(fun);
				break;
			}
		}
	}
}

void fay::FayVM::run(PTR(FayFun) fun)
{
	switch (fun->type())
	{
		case FunType::Code:
		{
			PTR(FayInstFun) f = TOPTR(FayInstFun, fun);
			this->_run(f->getPreparedInsts());
			break;
		}
		case FunType::Internal:
		{
			PTR(FayInternalFun) f = TOPTR(FayInternalFun, fun);
			f->Invoke(&this->stack);
		}
		default:
			LOG_ERROR("Unknow function type : " << (int)fun->type());
			break;
	}
}
