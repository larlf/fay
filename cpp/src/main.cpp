#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <mirage/mirage_utils.h>
#include <mirage/mirage_data.h>
#include "fay_compiler.h"
#include "fay_compiler_nodes.h"
#include "fay_compiler_parser.h"
#include "fay_runtime.h"
#include "fay_vm.h"

using namespace fay::compiler;

extern FILE* yyin, *yyout;
extern char* yytext;
extern int yyparse();

class ClassA
{
public:
	int FILE;
};

void test1()
{
	//显示文件内容
	std::string filename = "../script/test3.fay";
	std::string text = mirage::utils::SystemUtils::ReadTextFile(filename);
	LOG_DEBUG("Read Source File : ");
	PRINT(text);
	PRINT("---- End ----");

	//打开文件
	FILE* fp = std::fopen(filename.c_str(), "r");
	if (fp != nullptr)
	{
		//初始化环境
		//CompileBus::ASTRootNode = new ASTNode("Root");

		//进行语法解析
		yyin = fp;
		yyparse();

		//关闭文件
		std::fclose(fp);
	}
	else
		std::cout << "Cannot find file : " << filename << std::endl;

	TextBuilder tb;
	ASTBus::ASTRootNode->dump(&tb);
	LOG_DEBUG("AST : ");
	PRINT(tb.toString());

	LOG_DEBUG("Format : ");
	std::string formatText;
	ASTBus::ASTRootNode->format(formatText);
	PRINT(formatText);
	tb.clear();

	//生成中间代码
	Compiler* compiler = new Compiler();
	ASTBus::ASTRootNode->compile(compiler);
}

void test2(std::string filename)
{
	if (!mirage::utils::FileUtils::IsExist(filename.c_str()))
	{
		LOG_ERROR("Cannot find file : " << filename);
		return;
	}

	//读取文件
	mirage::ByteData* data = mirage::utils::FileUtils::ReadFile(filename.c_str());
	fay::FayBinData* libData = new fay::FayBinData(data);

	fay::FayLib* lib = new fay::FayLib();
	lib->readFromBytes(libData);

	delete libData;
	delete data;

	fay::FayFun* fun = lib->findClass("MyTest")->findFun("main");
	LOG_DEBUG("Fun : "<<fun);

	fay::FayVM::Execute(fun->insts);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		LOG_ERROR("[Format] fay <filename>");
		return 0;
	}

	//test1();
	test2(std::string(argv[1]));

	//system("PAUSE");
	return 0;
}
