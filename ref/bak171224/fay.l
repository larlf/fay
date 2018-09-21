%{

//解决Windows的依赖问题
#ifdef WIN32
#define YY_NO_UNISTD_H
#include <io.h>
#endif

#include <iostream>
#include <string>
#include "fay_nodes.hpp"
#include "fay_parser.hpp"

int lineno=1;

//判断ID的类型
int idType()
{
	std::string id(yytext);

	if(FayVarBus::IsVar(id))
		return VAR;
	else if(FayClassBus::IsClass(id))
		return CTYPE;

	return ID;
}

%}

%option noyywrap

intsuffix				([uU][lL]?)|([lL][uU]?)
fracconst				([0-9]*\.[0-9]+)|([0-9]+\.)
exppart					[eE][-+]?[0-9]+
floatsuffix				[fFlL]
chartext				([^\'])|(\\.) 
stringtext				([^\"])|(\\.)

%%

#可忽略的字符
"\n"					{ ++lineno; }
[\t\f\v\r ]+			{ }
"//".*					{ }

#单个的操作符
"{"  { return '{'; }
"}"  { return '}'; }
"["  { return '['; }
"]"  { return ']'; }
"("  { return '('; }
")"  { return ')'; }
";"  { return ';'; }
":"  { return ':'; }
"?"  { return '?'; }
"."  { return '.'; }
"+"  { return '+'; }
"-"  { return '-'; }
"*"  { return '*'; }
"/"  { return '/'; }
"%"  { return '%'; }
"^"  { return '^'; }
"&"  { return '&'; }
"|"  { return '|'; }
"~"  { return '~'; }
"!"  { return '!'; }
"="  { return '='; }
"<"  { return '<'; }
">"  { return '>'; }
","  { return ','; }

#多个的操作符
"+="  { return ADDEQ; }
"-="  { return SUBEQ; }
"*="  { return MULEQ; }
"/="  { return DIVEQ; }
"%="  { return MODEQ; }
"^="  { return XOREQ; }
"&="  { return ANDEQ; }
"|="  { return OREQ;  }
"<<"  { return SL;    }
">>"  { return SR;    }
"<<=" { return SLEQ;  }
">>=" { return SREQ;  }
"=="  { return EQ;    }
"!="  { return NQ;    }
"<="  { return LTEQ;  }
">="  { return GTEQ;  }
"&&"  { return AND2;  }
"||"  { return OR2;   }
"++"  { return PLUS2; }
"--"  { return SUB2;  }
"->"  { return ARROW; }

#关键字
"var"		{ return VAR; }
"package"    { return PACKAGE;    }
"import"     { return IMPORT;     }
"class"      { return CLASS;      }
"extends"    { return EXTENDS;    }
"implements" { return IMPLEMENTS; }
"static"     { return STATIC;     }
"public"     { return PUBLIC;     }
"private"    { return PRIVATE;    }
"protected"  { return PROTECTED;  }
"const"      { return CONST;      }
"if"         { return IF;         }
"for"        { return FOR;        }
"return"     { return RETURN;     }
"while"      { return WHILE;      }
"do"         { return DO;         }
"continue"   { return CONTINUE;   }
"case"       { return CASE;       }
"switch"     { return SWITCH;     }
"break"      { return BREAK;      }
"interface"  { return INTERFACE;  }

#基础数类型
"void"   { return TYPE; }
"int"    { return TYPE; }
"long"   { return TYPE; }
"float"  { return TYPE; }
"double" { return TYPE; }
"bool"   { return TYPE; }
"string" { return TYPE; }

#标识符
[a-zA-Z_][a-zA-Z_0-9]*              { return idType(); }

#整数
"0"[xX][0-9a-fA-F]+{intsuffix}?     { return INT;    }
"0"[0-7]+{intsuffix}?               { return INT;    }
[0-9]+{intsuffix}?                  { return INT;    }

#浮点数
{fracconst}{exppart}?{floatsuffix}? { return FLOAT;  }
[0-9]+{exppart}{floatsuffix}?       { return FLOAT;  }

#字符和字符串
"'"{chartext}*"'"                   { return CHAR;   }
"\""{stringtext}*"\""               { return STRING; }

%%


