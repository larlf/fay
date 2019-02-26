#pragma once
#include <fay_lang.h>
#include <fay_ast.h>
#include <fay_lexer.h>

namespace fay
{
	//源代码
	class FaySource : BaseObject
	{
	public:
		PTR(FayFile) file;
		PTR(std::vector<PTR(Token)>) tokens;
		PTR(AstNode) ast;

		FaySource(PTR(FayFile) file) : file(file) {}

		const std::string filename() { return this->file->filename; }
		const std::string text() { return this->file->text; }

		//取得Token的内容用于显示
		std::string tokensStr();
		//取得用于显示AST的内容
		std::string astStr();
	};

	//项目
	class FayProject : BaseObject
	{
	private:
		std::string _path;  //项目所在的目录
		std::string _name;
		int _marjor = 0;
		int _minjor = 0;

		MAP<std::string, PTR(FaySource)> _files;
		//PTR(FayBuilder) _builder;
		PTR(FayLib) _lib;

		//检查所有需要处理的文件
		void step1CheckFiles();
		//词法分析
		void step2Lexical();
		//构建语法树
		void step3AST();
		//解析所有的类型
		void step4ParseClass();
		//解析所有的字段和方法
		void step5ParseFieldAndMethod();
		//确定每个节点的类型
		//如果有找不到类之类的错误，基本都是在这个过程中发现的
		void step6FixedNodeType();
		//生成中间代码
		void step7GenerateIL();

		//用于进行词法分析的工作线程
		static void lexicalWorkThread(BuildTaskQueue<FaySource>* queue);
		//用于生成AST的工作线程
		static void astWorkThread(BuildTaskQueue<FaySource>* queue);
		//Dig3工作线程的问题
		static void Dig3WorkThread(BuildTaskQueue<FaySource>* queue, PTR(FayLib) lib);
		//Dig4工作线程的问题
		static void Dig4WorkThread(BuildTaskQueue<FaySource>* queue, PTR(FayLib) lib);

	public:
		FayProject(const std::string &projectPath);
		FayProject(const std::string &name, int marjor, int minjor);

		PTR(FaySource) findSource(const std::string &name);

		void build();

	};

	class BuildTask
	{
	public:
	};

	//词法分析的任务
	class LexicalTask
	{
	public:
		PTR(FayFile) file;
		PTR(std::vector<PTR(Token)>) tokens;

		LexicalTask(PTR(FayFile) file) : file(file) {}
		std::string debugInfo();
	};


}