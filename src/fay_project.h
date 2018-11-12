#pragma once
#include <fay_lang.h>
#include <fay_ast.h>
#include <fay_lexer.h>

namespace fay
{
	//源代码
	class FaySource : FayObject
	{
	private:
		PTR(FayFile) _file;
		PTR(std::vector<PTR(Token)>) _tokens;
		PTR(AstNode) _ast;

	public:
		FaySource(PTR(FayFile) file) : _file(file) {}

		//对内容进行解析
		void parse(PTR(Lexer) lexer);
	};

	//项目
	class FayProject : FayObject
	{
	private:
		MAP<std::string, PTR(FaySource)> _files;

	public:
		void addFiles(std::vector<std::string> &files);
		void parse();
	};
}