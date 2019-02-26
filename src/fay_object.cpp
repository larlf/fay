#include <fay_object.h>
#include <mirror.h>

using namespace fay;
using namespace mirror;

std::string fay::BaseObject::toString()
{
	utils::StringBuilder sb;
	this->buildString(&sb);
	return sb.toString();
}

std::string fay::FilePart::print()
{
	std::string msg;
	std::string str;
	int strLine = 1;
	int strCol = -1;
	int strLineStart = -1;
	int state = 0;  //状态
	int nowLine = -1;  //当前显示的行

	//显示文件名
	{
		std::string str = "(" + std::to_string(this->line) + "," + std::to_string(this->col) + ") at " + this->file->filename;
		std::cout << termcolor::green  << str << termcolor::reset << std::endl;
		msg += str + "\n";
	}

	char c;
	for(int i = 0; i < this->file->text.size(); ++i)
	{
		c = this->file->text[i];

		//有一些字符不处理
		if(c == '\r')
			continue;

		//当前列
		strCol = i - strLineStart;

		//显示前后3行
		if(strLine >= line - 3 && strLine <= line + 3)
		{
			//处理行号
			if(nowLine != strLine)
			{
				nowLine = strLine;

				utils::StringBuilder sb;
				std::string temp = std::to_string(nowLine);
				sb.add(' ', 5 - temp.size());
				sb.add(temp);
				sb.add(" | ");
				temp = sb.toString();
				std::cout << termcolor::cyan << temp << termcolor::reset;
				msg += temp;
			}

			switch(state)
			{
				case 0:
					if(strLine == line && strCol == this->col)
					{
						state = 1;
						std::cout << termcolor::on_red;
					}
					break;
				case 1:
					if(this->count > 0)
					{
						if(strCol - this->col <= this->count)
							break;
					}
					else if(c != '\n' && c != ' ' && c != '\t' && c != ';' && c != ')')
						break;

					//如果需要结束，这里改变状态
					state = 2;
					std::cout << termcolor::reset;
					break;
			}

			std::cout << c;
			msg.append(&c, 0, 1);
		}

		//换行
		if(c == '\n')
		{
			strLine++;
			strLineStart = i;
		}
	}

	//防止状态没有转换过来
	if(state == 1)
		std::cout << termcolor::reset;
	std::cout << std::endl;

	return msg;
}
