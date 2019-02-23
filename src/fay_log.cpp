#include "fay_log.h"

using namespace mirror::utils;

std::recursive_mutex fay::LogBus::LogLock;

std::string fay::LogBus::_LogFile;

void fay::LogBus::SetLogFile(const std::string &filename)
{
	LogBus::_LogFile = filename;
	if(filename.size() > 0)
		FileUtils::WriteTextFile(filename, "");
}

void fay::LogBus::Log(LogType type, const std::string &data)
{
	std::string msg = data;

	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);

	switch(type)
	{
		case LogType::Info:
#if WIN32
			std::cout << termcolor::yellow << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::yellow << msg << termcolor::reset << std::endl;
#endif
			break;
		case LogType::Warn:
			msg.insert(0, "Warn : ");
#if WIN32
			std::cout << termcolor::magenta << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::magenta << msg << termcolor::reset << std::endl;
#endif
			break;
		case LogType::Error:
			msg.insert(0, "Error : ");
#if WIN32
			std::cout << termcolor::red << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::red << msg << termcolor::reset << std::endl;
#endif
			break;
	}

	if(_LogFile.size() > 0)
		FileUtils::AppendTextFile(_LogFile, msg);
}

void fay::LogBus::PrintSource(const std::string &filename, const std::string &text, int line, int col, int count)
{
	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);

	std::string msg;
	std::string str;
	int strLine = 1;
	int strCol = -1;
	int strLineStart = -1;
	int state = 0;  //״̬
	int nowLine = -1;  //��ǰ��ʾ����

	//��ʾ�ļ���
	{
		std::cout << termcolor::green << "[" << filename << "]" << termcolor::reset << std::endl;
		msg += "[" + filename + "]\n";
	}

	char c;
	for(int i = 0; i < text.size(); ++i)
	{
		c = text[i];

		//��һЩ�ַ�������
		if(c == '\r')
			continue;

		//��ǰ��
		strCol = i - strLineStart;

		//��ʾǰ��3��
		if(strLine >= line - 3 && strLine <= line + 3)
		{
			//�����к�
			if(nowLine != strLine)
			{
				nowLine = strLine;

				StringBuilder sb;
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
					if(strLine == line && strCol == col)
					{
						state = 1;
						std::cout << termcolor::on_red;
					}
					break;
				case 1:
					if(count > 0)
					{
						if(strCol - col <= count)
							break;
					}
					else if(c != '\n' && c != ' ' && c != '\t' && c != ';' && c != ')')
						break;

					//�����Ҫ����������ı�״̬
					state = 2;
					std::cout << termcolor::reset;
					break;
			}

			std::cout << c;
			msg.append(&c, 0, 1);
		}

		//����
		if(c == '\n')
		{
			strLine++;
			strLineStart = i;
		}
	}

	//��ֹ״̬û��ת������
	if(state == 1)
		std::cout << termcolor::reset;
	std::cout << std::endl;

	//�����Log�ļ�
	if(_LogFile.size() > 0)
		FileUtils::AppendTextFile(_LogFile, msg);
}
