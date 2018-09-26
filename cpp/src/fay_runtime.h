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
		int version;  //���ݵİ汾
		int pos;  //��ǰ������λ��
		mirage::ByteBuffer buffer;  //����

		FayBinData(mirage::ByteData* data) : pos(0), buffer(data) {}
	};

	//�����ƶ���
	class IBinObj
	{
		//�������н�������
		//����ֵ�Ƕ�ȡ�������ݳ���
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

		// ͨ�� IBinObj �̳�
		virtual void readFromBytes(FayBinData* data) override;

	};

	class FayClass : public IBinObj
	{
	public:
		std::string name;
		std::vector<FayFun*> funs;

		// ͨ�� IBinObj �̳�
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

		// ͨ�� IBinObj �̳�
		virtual void readFromBytes(FayBinData* data) override;

	};

	class InstPushString : public  FayInst
	{
	public:
		std::string str;

		InstPushString() : FayInst(1) {}

		// ͨ�� FayInst �̳�
		virtual void readFromBytes(FayBinData* data) override;
	};

	class InstStaticCall : public  FayInst
	{
	public:
		std::string funName;
		int paramCount;

		InstStaticCall() : FayInst(2) {}

		// ͨ�� FayInst �̳�
		virtual void readFromBytes(FayBinData* data) override;
	};
}

