#pragma once

#include <string>
#include <mirror_sys_const.h>

namespace mirror
{
	namespace utils
	{
		//�������е�ѡ���
		class CmdlineOptionDef
		{
		public:
			std::string name;
			std::string shortName;
			bool isSign = false;  //����Ǳ�ʶ����û�в���
			std::string desc;

			//name : ѡ�������
			//shortName : ѡ��Ķ�����
			//isSign : �Ƿ��Ǳ�־������Ǳ�־�Ļ����治������
			//desc : ˵����Ϣ
			CmdlineOptionDef(const std::string &name, const std::string &shortName, bool isSign, const std::string &desc)
				: name(name), shortName(shortName), isSign(isSign), desc(desc) {}
		};

		//�������еĲ�������
		class CmdlineParamDef
		{
		public:
			std::string name;
			std::string desc;

			//name : ����������
			//desc : ������˵����Ϣ
			CmdlineParamDef(const std::string &name, const std::string &desc)
				: name(name), desc(desc) {}
		};

		//�����еĽ�����
		//COMMAND OPTIONS PARAMETERS
		class CmdlineParser
		{
		private:
			//ѡ���
			MAP<std::string, PTR(CmdlineOptionDef)> optionDefs;
			//��������
			std::vector<PTR(CmdlineParamDef)> paramDefs;
			//ѡ��ֵ
			MAP<std::string, std::string> options;
			//����ֵ
			std::vector<std::string> params;

			//ȡ�ò��������У�û���ҵ�����0
			size_t getParamIndex(const std::string &name);

		public:
			//����������
			size_t paramCount() { return this->params.size() - 1; }

			//����ѡ���ʽ
			void set(PTR(CmdlineOptionDef) param);
			//���ò�����ʽ
			void set(PTR(CmdlineParamDef) def);
			//��������н���
			void parse(int argc, char** argv);
			//��������ȡ��ֵ
			std::string get(const std::string &name);
			//���ݲ�����λ��ȡ��ֵ
			//ע�⣬����ֵ�Ǵ�1��ʼ��
			std::string get(size_t index);
			//���������Ϣ
			std::string help();
			//���������Ϣ
			void debug();
		};
	}
}