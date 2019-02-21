#include <fay_lexer.h>
#include <fay_i18n.h>
#include <mirror_utils_string.h>
#include <mirror_utils_log.h>

using namespace mirror;
using namespace mirror::data;
using namespace fay;

PTR(Token) fay::Lexer::changeMode(PTR(Token) t)
{
	//如果进入到特别的内容中，处理下mode转换
	switch(t->type())
	{
		case TokenType::DoubleSlash:
			this->_mode = LexMode::Comment;
			return nullptr;
		case TokenType::Comment:
			this->_mode = LexMode::Program;
			return t;
	}

	return t;
}

fay::Lexer::Lexer()
{
	//关键字
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Class, "class"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Interface, "interface"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Var, "var"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::New, "new"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Fun, "fun"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::If, "if"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::ElseIf, "elseif"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Else, "else"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::For, "for"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Using, "using"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Package, "package"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::While, "while"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Do, "do"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Return, "return"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Label, "label"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Goto, "goto"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::As, "as"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Try, "try"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Catch, "catch"));
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::Finally, "finally"));

	//分隔符
	this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::Semicolon,  ';'));
	//this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::Assign, '='));
	this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::Colon, ':'));
	this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::Comma, ','));
	this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::LeftBrace, '{'));
	this->_rules.push_back((ITokenRule*)new SymbolTokenRule(LexMode::Program, TokenType::RightBrace, '}'));

	//基本数据类型
	this->_rules.push_back((ITokenRule*)new WordsTokenRule(LexMode::Program, TokenType::BasicType,
	{
		"void", "byte", "int", "float", "double", "string", "long"
	}));

	//布尔类型的数据
	this->_rules.push_back((ITokenRule*)new WordsTokenRule(LexMode::Program, TokenType::Bool,
	{
		"true", "false"
	}));

	//对方法和类的说明符
	this->_rules.push_back((ITokenRule*)new WordsTokenRule(LexMode::Program, TokenType::DescSymbol,
	{
		"public", "private", "static", "inline"
	}));

	//特别的方法名
	this->_rules.push_back((ITokenRule*)new WordsTokenRule(LexMode::Program, TokenType::SystemName,
	{
		FUN_CREATE, FUN_DESTORY, ".main"
	}));

	//数字
	this->_rules.push_back((ITokenRule*)new NumberTokenRule(LexMode::Program, TokenType::Number));

	//ID
	this->_rules.push_back((ITokenRule*)new IDTokenRule(LexMode::Program, TokenType::ID));

	//不同级别的操作符
	this->_rules.push_back((ITokenRule*)new WordsTokenRule(LexMode::Program, TokenType::OP,
	{
		"*=", "/=", "+=", "-=", "%=", "<<=", ">>=", "&=", "^=", "|=",
		"++", "--", ">>", "<<",
		"==", ">=", "<=", "!=",
		"&&", "||",
		"(", ")", "[", "]",
		"+", "-", "*", "/", "%",
		"~", "!", ">", "<",
		"&", "^", "|",
		"?", ":",
		"=",
	}));

	//字符和字符串
	this->_rules.push_back((ITokenRule*)new CharTokenRule(LexMode::Program, TokenType::Char));
	this->_rules.push_back((ITokenRule*)new StringTokenRule(LexMode::Program, TokenType::String));

	//注释
	this->_rules.push_back((ITokenRule*)new WordTokenRule(LexMode::Program, TokenType::DoubleSlash, "//"));
	this->_rules.push_back((ITokenRule*)new SingleCommentTokenRule(LexMode::Comment, TokenType::Comment));
}

fay::Lexer::~Lexer()
{
	//LOG_INFO("Destory Lexer");
	for each(auto it in this->_rules)
	{
		if(it != nullptr)
			delete it;
	}

	this->_rules.clear();
}

PTR(std::vector<PTR(Token)>) fay::Lexer::Execute(const std::string &text)
{
	//输入和输出
	ByteData chars((byte*)text.c_str(), text.size());
	PTR(std::vector<PTR(Token)>) r = MKPTR(std::vector<PTR(Token)>)();

	int pos = 0;  //当前处理的位置
	int line = 1;  //当前所在的行
	int lineEnd = 0;  //最后一行结束的位置
	while(pos < chars.size())
	{
		char c = chars[pos];

		//跳过空白字符
		if(c == ' ' || c == '\t' || c == '\r')
		{
			pos++;
			continue;
		}
		else if(c == '\n')
		{
			line++;
			lineEnd = pos;

			pos++;
			continue;
		}

		//计算所在列
		int col = pos - lineEnd;

		//生成当前位置的token列表
		std::vector<PTR(Token)> tokens;
		for each(auto it in this->_rules)
		{
			if(it->mode() == this->_mode)
			{
				Token* t = it->match(chars, pos, line, col);
				if(t != nullptr)
					tokens.push_back(PTR(Token)(t));
			}
		}

		//如果找到了，就向下进行，不然就报错
		if(tokens.size() > 0)
		{
			PTR(Token) t = tokens[0];

			//有多个结果的时候，取一个最长的
			//长度相同取优先级最高的
			if(tokens.size() > 1)
			{
				for each(auto it in tokens)
				{
					if(it->size() > t->size())
						t = it;
				}
			}

			//对这部分内容进行其它分析
			for(auto i = 0; i < t->size(); ++i)
			{
				//检查换行符，对行数进行计数
				if(chars[pos + i] == '\n')
				{
					line++;
					lineEnd = pos + i;
				}
			}

			//修改位置指针
			pos += t->size();

			//添加取返回结果
			t = changeMode(t);
			if(t != nullptr) r->push_back(t);

		}
		else
		{
			//生成错误信息
			utils::StringBuilder sb;
			sb.add(I18N::Get("err.lex"))->endl();

			//取当前行的内容
			int strPos = lineEnd + 1;
			while(strPos < chars.size() && chars[strPos] != '\n' && chars[strPos] != '\r')
			{
				if(chars[strPos] == '\t')
					sb.add(' ');
				else
					sb.add((char)chars[strPos]);

				strPos++;
			}

			//显示位置
			sb.endl();
			for(auto i = 0; i < col - 1; ++i)
				sb.add(' ');
			sb.add('^');

			//抛出异常
			throw LexerException(sb.toString(), line, col);
		}
	}

	return r;
}
