#pragma once

#include "mirage/mirage_data.h"

namespace fay
{
	class FayLib;
	class FayClass;
	class FayFun;
	class FayInst;

	class FayBinData
	{
	public:
		int version;  //数据的版本
		int pos;  //当前读到的位置
		mirage::ByteBuffer buffer;  //数据

		FayBinData(mirage::ByteData* data) : pos(0), buffer(data) {}
	};

	//二进制对象
	class IBinObj
	{
		//从数据中解析对象
		//返回值是读取到的数据长度
		virtual void readFromBytes(FayBinData* data) = 0;
	};

	class FayDomain
	{
	public:
	};

	class FayLib : public IBinObj
	{
	public:
		std::string name;
		int major;
		int minor;
		MAP<std::string, FayClass*> classes;

		FayLib() : name(""), major(0), minor(0) {}

		FayClass* findClass(const std::string &name);

		// 通过 IBinObj 继承
		virtual void readFromBytes(FayBinData* data) override;

	};

	class FayClass : public IBinObj
	{
	public:
		std::string name;
		std::vector<FayFun*> funs;

		// 通过 IBinObj 继承
		virtual void readFromBytes(FayBinData* data) override;

		FayFun* findFun(const std::string &name);
	};

	class FayInst : public IBinObj
	{
	public:
		int type;
		std::string name;

		FayInst(int type) : type(type) {}
	};

	class FayFun : public  IBinObj
	{
	public:
		std::string name;
		std::vector<FayInst*> insts;

		void Invoke();

		// 通过 IBinObj 继承
		virtual void readFromBytes(FayBinData* data) override;

	};

	class InstPushString : public  FayInst
	{
	public:
		std::string str;

		InstPushString() : FayInst(1) {}

		// 通过 FayInst 继承
		virtual void readFromBytes(FayBinData* data) override;
	};

	class InstStaticCall : public  FayInst
	{
	public:
		std::string funName;
		int paramCount;

		InstStaticCall() : FayInst(2) {}

		// 通过 FayInst 继承
		virtual void readFromBytes(FayBinData* data) override;
	};
}

