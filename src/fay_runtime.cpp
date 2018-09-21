#include "fay_runtime.h"


fay::FayClass * fay::FayLib::findClass(const std::string & name)
{
	auto it=this->classes.find(name);
	if (it != this->classes.end())
	{
		return it->second;
	}

	return nullptr;
}

void fay::FayLib::readFromBytes(FayBinData* data)
{
	data->pos += mirage::DataUtils::Read(&this->name, &data->buffer, data->pos);
	data->pos += mirage::DataUtils::Read(&this->major, &data->buffer, data->pos);
	data->pos += mirage::DataUtils::Read(&this->minor, &data->buffer, data->pos);

	int count;
	data->pos += mirage::DataUtils::Read(&count, &data->buffer, data->pos);

	for (auto i = 0; i < count; ++i)
	{
		FayClass* clazz = new FayClass();
		clazz->readFromBytes(data);
		this->classes[clazz->name] = clazz;
	}
}

void fay::FayClass::readFromBytes(FayBinData* data)
{
	data->pos += mirage::DataUtils::Read(&this->name, &data->buffer, data->pos);

	int count;
	data->pos += mirage::DataUtils::Read(&count, &data->buffer, data->pos);

	for (auto i = 0; i < count; ++i)
	{
		FayFun* fun = new FayFun();
		fun->readFromBytes(data);
		this->funs.push_back(fun);
		LOG_DEBUG(fun->name);
	}
}

fay::FayFun * fay::FayClass::findFun(const std::string & name)
{
	for (auto it = this->funs.begin(); it != this->funs.end(); ++it)
	{
		if ((*it)->name == name)
		{
			return *it;
		}
	}

	return nullptr;
}

void fay::FayFun::Invoke()
{

}

void fay::FayFun::readFromBytes(FayBinData* data)
{
	data->pos += mirage::DataUtils::Read(&this->name, &data->buffer, data->pos);

	int count;
	data->pos += mirage::DataUtils::Read(&count, &data->buffer, data->pos);

	for (auto i = 0; i < count; ++i)
	{
		int instType;
		data->pos += mirage::DataUtils::Read(&instType, &data->buffer, data->pos);

		switch (instType)
		{
			case 1:
			{
				fay::InstPushString* inst = new fay::InstPushString();
				inst->readFromBytes(data);
				LOG_DEBUG("String3 : " << inst->str);
				this->insts.push_back(inst);
			}
			break;
			case 2:
			{
				fay::InstStaticCall* inst = new fay::InstStaticCall();
				inst->readFromBytes(data);
				this->insts.push_back(inst);
			}
			break;
		}
	}
}

void fay::InstPushString::readFromBytes(FayBinData* data)
{
	data->pos += mirage::DataUtils::Read(&this->str, &data->buffer, data->pos);

	LOG_DEBUG("String : " << this);
	LOG_DEBUG("String : " << this->str);
}

void fay::InstStaticCall::readFromBytes(FayBinData* data)
{
	data->pos += mirage::DataUtils::Read(&this->funName, &data->buffer, data->pos);
	data->pos += mirage::DataUtils::Read(&this->paramCount, &data->buffer, data->pos);
}
