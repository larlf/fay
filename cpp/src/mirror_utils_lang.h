#pragma once

namespace mirror
{
	namespace utils
	{
		class LangUtils
		{
		public:
			/**
			* 取得数组的长度
			*/
			template<class T>
			inline static size_t SizeOfArray(T &array)
			{
				return sizeof(array) / sizeof(array[0]);
			}
		};
	}
}

