#pragma once
#include <mirror_utils_string.h>
#include <mirror_sys_const.h>
#include <fay_const.h>

//这个文件主要用于定义一些不同对象之前公用或限定规则的类

namespace fay
{
	//所有对象的基类
	class FayObject
	{
	public:
		FayObject() {}
		virtual ~FayObject() {}

		//全名
		virtual const std::string &fullname() { return mirror::utils::StringUtils::Blank;  }
		//生成字符串的方法
		virtual void toString(mirror::utils::StringBuilder* sb) {}
	};

	//指令
	class FayInst : public FayObject
	{
	public:
		virtual void toString(mirror::utils::StringBuilder* sb) { sb->endl(); }
	};

	//值
	class FayValue
	{
	private:
		int64_t _value;
	};

	//有索引的Mpa结构
	template<class T>
	class IndexMap
	{
	private:
		T _null;
		std::vector<T> _list;
		MAP<std::string, pos_t> _map;

	public:
		pos_t size() { return this->_list.size(); }
		const std::vector<T> &list() { return this->_list; }

		pos_t add(const std::string &key, T obj)
		{
			//如果没有此对象，添加进来
			auto r = this->_map.find(key);
			if (r == this->_map.end())
			{
				pos_t index = this->_list.size();
				this->_list.push_back(obj);
				this->_map[key] = index;
				return index;
			}
			else
				return r->second;
		}

		pos_t findIndex(const std::string &key)
		{
			auto r = this->_map.find(key);
			if (r != this->_map.end())
				return r->second;

			return -1;
		}

		T find(pos_t index)
		{
			if (index >= 0 && index < this->_list.size())
				return this->_list[index];

			return nullptr;
		}

		T find(const std::string &key)
		{
			pos_t index = findIndex(key);
			if (index >= 0)
				return this->_list[index];

			return nullptr;
		}
	};
}