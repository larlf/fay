#pragma once
#include <fay_lang.h>
#include <fay_ast.h>
#include <fay_lexer.h>

namespace fay
{
	//源代码
	class BuildTask : BaseObject
	{
	public:
		std::string filename;
		PTR(FayFile) file;
		PTR(std::vector<PTR(Token)>) tokens;
		PTR(AstNode) ast;

		BuildTask(const std::string &filename) : filename(filename) {}

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

		MAP<std::string, PTR(BuildTask)> _tasks;
		PTR(FayLib) _lib;

		//对线程内处理逻辑的封装
		static void WorkThread(BuildTaskQueue<BuildTask>* queue, std::function<void(PTR(BuildTask))> fun, const std::string &errorMsg);
		//读取文件的工具线程
		static void ReadFileThread(BuildTaskQueue<BuildTask>* queue);
		//用于进行词法分析的工作线程
		static void LexicalWorkThread(BuildTaskQueue<BuildTask>* queue);
		//用于生成AST的工作线程
		static void ASTWorkThread(BuildTaskQueue<BuildTask>* queue);
		//Dig3工作线程
		static void Dig3WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib);
		//Dig4工作线程
		static void Dig4WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib);

		//在线程中运行
		void executeThreads(std::function<void(BuildTaskQueue<BuildTask>*)> fun);

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

	public:
		FayProject(const std::string &projectPath);
		FayProject(const std::string &name, int marjor, int minjor);

		PTR(FayLib) lib() { return this->_lib; }

		//根据文件名称查找代码
		PTR(BuildTask) findSource(const std::string &name);

		//进行编译
		void build();
	};

}