%{
	#include <cstdio>
    #include <cstdlib>
	#include <iostream>
	#include <vector>
	#include "fay_compiler_ast.h"

	using namespace fay::lang;
	using namespace fay::compiler;

	#define YYDEBUG 1
	
	extern FILE *yyin;
	extern char *yytext;	
	extern int yylex(void);

	int yyerror(std::string err)
	{
		std::cout<<"Error : "<<err<<" at line:"<<ASTBus::LineNum<<std::endl;
		std::cout<<"    "<<yytext<<std::endl;
		std::exit(1);
	}	
%}

%union {
	fay::compiler::ASTNode *node;
}

//基础类型
%token VAR
%token <node> ID TYPE OP1 OP2 OP3 INTEGER FLOATING STRING
%token <node> SHIFT_LEFT SHIFT_RIGHT GT_EQ LT_EQ EQ NOT_EQ OR AND SUB_SUB ADD_ADD

//////////////////////////////////////////

%type <node> Expr
%type <node> '-'
%type <node> '!'
%type <node> '~'
%type <node> '*'
%type <node> '/'
%type <node> '%'
%type <node> '+'
%type <node> '>'
%type <node> '<'
%type <node> '&'
%type <node> '^'
%type <node> '|'
%type <node> '='

%type <node> Expr0
%type <node> Expr1
%type <node> Expr2
%type <node> Expr3
%type <node> Expr4
%type <node> Expr5
%type <node> Expr6
%type <node> Expr7
%type <node> Expr8
%type <node> Expr9
%type <node> Expr10
%type <node> Expr11
%type <node> Expr12

%type <node> VarStmt
%type <node> Call
%type <node> CallStmt
%type <node> Param
%type <node> Params
%type <node> AssignStmt
%type <node> Stmt

%%
//入口
fay 
	: Stmt { ASTBus::ASTRootNode=new ASTNode(AST_File, $1); }
	| fay Stmt { ASTBus::ASTRootNode->addChildNode($2); }
	;

//___________________________________________________________

//表达式的规则
Expr0
	: ID { $$=$1; }
	| INTEGER { $$=$1; }
	| STRING { $$=$1; }
	| Call { $$=nullptr; }
	;
Expr1
	: Expr0 { $$=$1; }
	| '[' Expr ']' { $$=$2; }
	| '(' Expr ')' { $$=$2; }
	;
Expr2 
	: Expr1 { $$=$1; } 
	| Expr1 SUB_SUB { $$=new ASTNode(AST_Expr1Right, $1, $2); }
	| SUB_SUB Expr1 { $$=new ASTNode(AST_Expr1Left, $1, $2); }
	| Expr1 ADD_ADD { $$=new ASTNode(AST_Expr1Right, $1, $2); }
	| ADD_ADD Expr1 { $$=new ASTNode(AST_Expr1Left, $1, $2); }
	| '-' Expr2 %prec '*' { $$=new ASTNode(AST_Expr1Left, $1, $2); }
	| '!' Expr2 { $$=new ASTNode(AST_Expr1Left, $1, $2); }
	| '~' Expr2 { $$=new ASTNode(AST_Expr1Left, $1, $2); }
	;
Expr3 
	: Expr2 { $$=$1; } 
	| Expr3 '/' Expr2 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	| Expr3 '%' Expr2 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	| Expr3 '*' Expr2 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	;
Expr4 
	: Expr3 { $$=$1; } 
	| Expr4 '+' Expr3 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	| Expr4 '-' Expr3 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	;
Expr5 
	: Expr4 { $$=$1; } 
	| Expr4 SHIFT_LEFT INTEGER { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	| Expr4 SHIFT_RIGHT INTEGER { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	;
Expr6 
	: Expr5 { $$=$1; } 
	| Expr6 '>' Expr5 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	| Expr6 GT_EQ Expr5 { $$=new ASTNode(AST_Expr2, $1, $2, $3); }
	;
Expr7 
	: Expr6 { $$=$1; } 
	;
Expr8 : Expr7 { $$=$1; } ;
Expr9 : Expr8 { $$=$1; } ;
Expr10 : Expr9 { $$=$1; } ;
Expr11 : Expr10 { $$=$1; } ;
Expr12 : Expr11 { $$=$1; } ;

//表达式
Expr
	: Expr12  { $$=$1; }
	;

//___________________________________________________________

//参数
Param :  Expr { $$=$1; }
	| CallStmt { $$=$1; }
	;

//参数组
Params : Param { $$=new ASTNode(AST_Params, $1); }
	| Params ',' Param { $$=$1->addChildNode($3); }
	;

//变量定义语句
VarStmt
	: VAR ID ':' TYPE ';' { $$=new ASTNode(AST_VarStmt, $2, $4); }
	| VAR ID '=' Expr ';' { $$=new ASTNode(AST_VarStmt, $2, $4); }
	;

//函数调用
Call
	: ID '(' ')' { $$=new ASTNode(AST_Call, $1); }
	| ID '(' Params ')' { $$=new ASTNode(AST_Call, $1, $3); }
	;

//函数调用语句
CallStmt
	: Call ';' { $$=new ASTNode(AST_CallStmt, $1); }
	;

//赋值语句
AssignStmt
	: ID '=' Expr ';' { $$=new ASTNode(AST_AssignStmt, $1, $3); }
	;

//句语
Stmt
	: VarStmt { $$=$1; }
	| CallStmt { $$=$1; }
	| AssignStmt { $$=$1; }
	;


%%





