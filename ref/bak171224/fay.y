%{
	#include "fay_nodes.hpp"
	#include <cstdio>
    #include <cstdlib>
	#include <iostream>
	#include <vector>

	#define YYDEBUG 1
	
	extern FILE *yyin;
	extern int lineno;
	extern char *yytext;	
	extern int yylex(void);

	int yyerror(std::string err)
	{
		std::cout<<"Error : "<<err<<" at line:"<<lineno<<std::endl;
		std::cout<<"    "<<yytext<<std::endl;
		std::exit(1);
	}	
%}

//数据结构
%union {
	std::string *string;
	FayNode *node;
}

//基础类型
%token TYPE CTYPE ID VAR
%token INT FLOAT CHAR STRING LIST MAP

//特别的操作符
%token ADDEQ SUBEQ MULEQ DIVEQ MODEQ XOREQ ANDEQ OREQ SIZEOF
%token EQ NQ LTEQ GTEQ
%token PLUS2 SUB2 AND2 OR2 ARROW SL SR SLEQ SREQ

//关键字
%token VAR PACKAGE IMPORT CLASS INTERFACE EXTENDS IMPLEMENTS 
%token STATIC PUBLIC PRIVATE PROTECTED CONST
%token IF FOR RETURN WHILE DO CONTINUE CASE SWITCH BREAK

//////////////////////////////////////////

//字符串类型
%type <string> id lid var memberScope
%type <string> basicType shortClassType longClassType classType
%type <string> unaryOP assignOP

//代码基本结构
%type <node> fay
%type <node> type clazz className classBody field method methodName

//表达式
%type <node> primaryExpr
%type <node> postfixExpr
%type <node> unaryExpr
%type <node> castExpr
%type <node> multiplicativeExpr
%type <node> additiveExpr
%type <node> shiftExpr
%type <node> relationalExpr
%type <node> equalityExpr
%type <node> andExpr
%type <node> exclusive_orExpr
%type <node> inclusive_orExpr
%type <node> logical_andExpr
%type <node> logical_orExpr
%type <node> conditionalExpr
%type <node> expr

//语句
%type <node> decl

%%

fay
	: clazz 
		{
			FileNode *file=new FileNode();
			FayBus::File=file;
			file->classes.push_back((ClassNode*)$1);
			$$=file;
		}
	| fay clazz 
		{ 
			FileNode *file=(FileNode*)$1;
			file->classes.push_back((ClassNode*)$2);
			$$=file;
		}
	;

//class定义
clazz
	: className 
		'{' 
		classBody 
		'}' 
		{ $$=$1; }
	| className '{' '}' {}
	;

//类的定义
className 
	: CLASS shortClassType 
		{ 
			ClassNode *clazz=new ClassNode();
			clazz->name=*$2;
			delete $2;
			FayBus::File->classes.push_back(clazz);
			$$=clazz;
		}
	| INTERFACE shortClassType
		{
			ClassNode *clazz=new ClassNode();
			clazz->name=*$2;
			delete $2;
			FayBus::File->classes.push_back(clazz);
			$$=clazz;
		}
	| className EXTENDS classType 
		{
			ClassNode *clazz=(ClassNode*)$1;
			clazz->parent=*$3;
			delete $3;
			$$=clazz; 
		}
	| className IMPLEMENTS classType
		{
			ClassNode *clazz=(ClassNode*)$1;
			clazz->interfaces.push_back(*$3);
			delete $3;
			$$=clazz;
		}
	| className ',' classType
		{ 
			ClassNode *clazz=(ClassNode*)$1;
			clazz->interfaces.push_back(*$3);
			delete $3;
			$$=clazz;
		}
	;

//类的内容
classBody
	: field 
		{
			ClassNode *clazz=FayBus::File->getLastClass();
			clazz->fields.push_back((FieldNode*)$1);
			$$=clazz;
		}
	| method
		{
			ClassNode *clazz=FayBus::File->getLastClass();
			clazz->methods.push_back((MethodNode*)$1);
			$$=clazz;
		}
	| classBody field
		{
			ClassNode *clazz=FayBus::File->getLastClass();
			clazz->fields.push_back((FieldNode*)$2);
			$$=clazz;
		}
	| classBody method
		{
			ClassNode *clazz=FayBus::File->getLastClass();
			clazz->methods.push_back((MethodNode*)$2);
			$$=clazz;
		}
	| classBody statement {}
	;

//字段
field
	: type var ';' 
		{
			FieldNode *field=new FieldNode();
			field->name=*$2;
			delete $2;
			$$=field;
		}
	| memberScope field 
		{ $$=$2; }
	| STATIC field 
		{ $$=$2; }
	;

//方法
method
	: methodName ';'
		{
			//纯虚函数
			MethodNode *method=(MethodNode*) $1;
			method->isPure=true;
			$$=$1; 
		} 
	| methodName statement 
		{
			$$=$1; 
		}
	;

//方法名称
methodName
	: type var '(' ')'
		{
			MethodNode *method=new MethodNode();
			method->name=*$2;
			delete $2;
			$$=method;
		}
	| type var '(' paramDecl ')'
		{
			MethodNode *method=new MethodNode();
			method->name=*$2;
			delete $2;
			$$=method;
		}
	| memberScope methodName 
		{
			MethodNode *method=(MethodNode*)$2;
			method->scope=*$1;
			delete $1; 
			$$=$2; 
		}
	| STATIC methodName
		{ 
			MethodNode *method=(MethodNode*)$2;
			method->isStatic=true;
			$$=$2; 
		}
	;

//参数定义
paramDecl
	: type id {}
	| paramDecl ',' type id {}
	;

//_______________________________________________________________________________

statement
	: '{' '}'  {}
	| '{' block '}'  {}
	;

block
	: decl {}
	| block decl {}
	;


//定义
decl
	: basicType id '=' expr ';' 
		{ 
		}
	| basicType id ';'
		{

		}
	| expr ';' { }
	;

//_______________________________________________________________________________
//表达式相关内容

//一元操作符
unaryOP
	: '*'  { $$=new std::string(yytext); }
	| '+'  { $$=new std::string(yytext); }
	| '-'  { $$=new std::string(yytext); }
	| '~'  { $$=new std::string(yytext); }
	| '!'  { $$=new std::string(yytext); }
	| '&'  { $$=new std::string(yytext); }
	;

//赋值操作符
assignOP
	: '='   { $$=new std::string(yytext); }
	| MULEQ { $$=new std::string(yytext); }
	| DIVEQ { $$=new std::string(yytext); }
	| MODEQ { $$=new std::string(yytext); }
	| ADDEQ { $$=new std::string(yytext); }
	| SUBEQ { $$=new std::string(yytext); }
	| SLEQ  { $$=new std::string(yytext); }
	| SREQ  { $$=new std::string(yytext); }
	| ANDEQ { $$=new std::string(yytext); }
	| XOREQ { $$=new std::string(yytext); }
	| OREQ  { $$=new std::string(yytext); }
	;

//表达式中的基础项
primaryExpr
	: lid 
		{ 
			$$=new IDExprNode(*$1); 
			delete $1; 
		}
	| INT { $$=new IntExprNode(yytext); }
	| FLOAT { $$=new FloatExprNode(yytext); }
	| STRING { $$=new StringExprNode(yytext); }
	| '(' expr ')'  
		{
			$$=new BracketExprNode($2);
		}
	;


//后置操作符
postfixExpr
	: primaryExpr  {}
	| postfixExpr '[' expr ']'  {}
	| postfixExpr '(' exprList ')'  {}
	| postfixExpr '(' ')'  {}
	//| postfixExpr '.' identifier_or_typedef_name  {}
	//| postfixExpr ARROW identifier_or_typedef_name  {}
	| postfixExpr PLUS2  {}
	| postfixExpr SUB2  {}
	;

//一元操作符的处理
unaryExpr
	: postfixExpr  {}
	| PLUS2 unaryExpr  
		{
			$$=new UnaryExprNode("++", $2);
		}
	| SUB2 unaryExpr  {}
	| unaryOP castExpr  {}
	| SIZEOF unaryExpr  {}
	//| SIZEOF '(' type_name ')'  {}
	;

//强致类型转换
castExpr
	: unaryExpr  {}
	| '(' type ')' castExpr  {}
	;

multiplicativeExpr
	: castExpr  {}
	| multiplicativeExpr '*' castExpr  
		{
			$$=new OPExprNode($1, $3, "*");
		}
	| multiplicativeExpr '/' castExpr  {}
	| multiplicativeExpr '%' castExpr  {}
	;

additiveExpr
	: multiplicativeExpr  {}
	| additiveExpr '+' multiplicativeExpr  
		{
			$$=new OPExprNode($1, $3, "+");
		}
	| additiveExpr '-' multiplicativeExpr  {}
	;

shiftExpr
	: additiveExpr  {}
	| shiftExpr SL additiveExpr  {}
	| shiftExpr SR additiveExpr  {}
	;

relationalExpr
	: shiftExpr  {}
	| relationalExpr '<' shiftExpr  {}
	| relationalExpr '>' shiftExpr  {}
	| relationalExpr LTEQ shiftExpr  {}
	| relationalExpr GTEQ shiftExpr  {}
	;

equalityExpr
	: relationalExpr  {}
	| equalityExpr EQ relationalExpr  {}
	| equalityExpr NQ relationalExpr  {}
	;

andExpr
	: equalityExpr  {}
	| andExpr '&' equalityExpr  {}
	;

exclusive_orExpr
	: andExpr  {}
	| exclusive_orExpr '^' andExpr  {}
	;

inclusive_orExpr
	: exclusive_orExpr  {}
	| inclusive_orExpr '|' exclusive_orExpr  {}
	;

logical_andExpr
	: inclusive_orExpr  {}
	| logical_andExpr AND2 inclusive_orExpr  {}
	;

logical_orExpr
	: logical_andExpr  {}
	| logical_orExpr OR2 logical_andExpr  {}
	;

conditionalExpr
	: logical_orExpr  {}
	| logical_orExpr '?' expr ':' conditionalExpr  {}
	;

expr
	: conditionalExpr  {}
	| unaryExpr assignOP expr  {}
	;

exprList
	: expr {}
	| exprList ',' expr {}
	;

//_______________________________________________________________________________

//变量
id : ID { $$=new std::string(yytext); }

//长变量，中间可以有.
lid
	: id { $$=$1; }
	| lid '.' id
		{ 
			(*$1)+="."+(*$3); 
			delete $3;
			$$=$1; 
		}
	;

//基本类型
basicType 
	: TYPE { $$=new std::string(yytext); }

shortClassType 
	: CTYPE { $$=new std::string(yytext); }

longClassType 
	: lid '.' shortClassType
		{ 
			(*$1)+="."+(*$3); 
			delete $3;
		}
	;

//对象类型
classType
	: shortClassType {}
	| longClassType {}
	;

//变量
var
	: id {}
	| shortClassType {}
	;

//变量类型
type
	: basicType 
		{ 
			$$=new BasicTypeNode(*$1); 
			delete $1; 
		}
	| classType 
		{
			$$=new ClassTypeNode(*$1);
			delete $1;
		}
	| LIST '<' type '>' 
		{
			$$=new ListTypeNode((TypeNode*)$3);
		}
	| MAP '<' type '>' 
		{
			$$=new MapTypeNode((TypeNode*)$3);
		}
	;

//变量的修饰符
memberScope
	: PUBLIC 
		{ $$=new std::string(yytext); }
	| PRIVATE
		{ $$=new std::string(yytext); }
	| PROTECTED
		{ $$=new std::string(yytext); }
	;

%%





