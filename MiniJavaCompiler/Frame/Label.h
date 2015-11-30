#pragma once
#include <string>

#include "SymbolsTable/Symbol.h"

namespace Temp {
	class CLabel {
	public:
		CLabel();
		explicit CLabel( const CSymbol& label );

		const std::string& Name() const
		{
			return name;
		}

	private:
		static int nextUniqueId;
		std::string name;
	};
};