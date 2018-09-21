%{
//解决Windows的依赖问题
#ifdef WIN32
#define YY_NO_UNISTD_H
#include <io.h>
#endif

#include <iostream>
#include <string>
#include "fay_compiler_ast.h"
#include "fay_compiler_parser.h"

using namespace fay::lang;
using namespace fay::compiler;

%}

%option noyywrap

intsuffix				([uU][lL]?[lL]?)|([lL][lL]?[uU]?)
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
chartext				([^\\\'])|(\\.)
stringtext				([^\\\"])|(\\.)

%%

#可忽略的字符
<INITIAL>"\n"					{ ASTBus::LineNum++; }
<INITIAL>[\t\f\v\r ]+			{ }
<INITIAL>"//".*					{ }

#关键字
<INITIAL>";" { return ';'; }
<INITIAL>":" { return ':'; }
<INITIAL>"[" { return '['; }
<INITIAL>"]" { return ']'; }
<INITIAL>"(" { return '('; }
<INITIAL>")" { return ')'; }
<INITIAL>"." { return '.'; }
<INITIAL>"-" { yylval.node=new ASTNode(yytext); return '-'; }
<INITIAL>"!" { yylval.node=new ASTNode(yytext); return '!'; }
<INITIAL>"~" { yylval.node=new ASTNode(yytext); return '~'; }
<INITIAL>"*" { yylval.node=new ASTNode(yytext); return '*'; }
<INITIAL>"/" { yylval.node=new ASTNode(yytext); return '/'; }
<INITIAL>"%" { yylval.node=new ASTNode(yytext); return '%'; }
<INITIAL>"+" { yylval.node=new ASTNode(yytext); return '+'; }
<INITIAL>"--" { yylval.node=new ASTNode(yytext); return SUB_SUB; }
<INITIAL>"++" { yylval.node=new ASTNode(yytext); return ADD_ADD; }
<INITIAL>"<<" { yylval.node=new ASTNode(yytext); return SHIFT_LEFT; }
<INITIAL>">>" { yylval.node=new ASTNode(yytext); return SHIFT_RIGHT; }
<INITIAL>">" { yylval.node=new ASTNode(yytext); return '>'; }
<INITIAL>"<" { yylval.node=new ASTNode(yytext); return '<'; }
<INITIAL>"==" { yylval.node=new ASTNode(yytext); return EQ; }
<INITIAL>"!=" { yylval.node=new ASTNode(yytext); return NOT_EQ; }
<INITIAL>"&" { yylval.node=new ASTNode(yytext); return '&'; }
<INITIAL>"^" { yylval.node=new ASTNode(yytext); return '^'; }
<INITIAL>"|" { yylval.node=new ASTNode(yytext); return '|'; }
<INITIAL>"&&" { yylval.node=new ASTNode(yytext); return AND; }
<INITIAL>"||" { yylval.node=new ASTNode(yytext); return OR; }

<INITIAL>"," { return ','; }
<INITIAL>"=" { return '='; }

<INITIAL>"var" { return VAR; }

#基础数类型
<INITIAL>"void"   { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"int"    { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"long"   { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"float"  { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"double" { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"bool"   { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"char"   { yylval.node  = new ASTNode(yytext); return TYPE; }
<INITIAL>"string" { yylval.node  = new ASTNode(yytext); return TYPE; }

#标识符
<INITIAL>[a-zA-Z_][a-zA-Z_0-9\.]*              { yylval.node=new ASTNode(AST_ID, yytext); return ID; }

#整数
<INITIAL>[1-9][0-9]*			{ yylval.node=new ASTNode(AST_IntValue, yytext); return INTEGER; }

#浮点数
<INITIAL>[0-9][0-9\.]*		{ yylval.node=new ASTNode(AST_DoubleValue, yytext); return FLOATING; }

#字符串
<INITIAL>\".*\"             { yylval.node=new ASTNode(AST_StringValue, yytext); return STRING; }

%%


