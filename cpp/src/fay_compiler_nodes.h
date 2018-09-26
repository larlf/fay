#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "fay_lang_utils.h"
#include "fay_compiler_ast.h"

using namespace fay::lang;

namespace fay
{
	namespace compiler
	{
		class INode
		{
		public:
			INode() {}
			virtual ~INode() {}
			virtual void dump(TextBuilder *tb) = 0;
		};

		class VarStmtNode : public INode
		{
		private:
			std::string id;
			ValueType type;

		public:
			VarStmtNode(ASTNode *ast);

			virtual void dump(TextBuilder *tb) override;

		};
	}
}

