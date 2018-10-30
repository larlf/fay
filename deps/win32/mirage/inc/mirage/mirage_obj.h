#pragma once
#include <string>

using namespace std;

namespace mirage
{
	namespace obj
	{
		class IObject
		{
		public:
			virtual string toString();
		};
	}
}

