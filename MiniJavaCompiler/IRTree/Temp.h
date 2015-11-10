#pragma once

#include <string>
#include "SymbolsTable\Symbol.h"

namespace Temp {
	class CLabel {
	public:
		CLabel();
		explicit CLabel(std::string& _name);
		const std::string& Name() const { return name; }
	private:
		static int nextUniqueId;
		std::string name;
	};

	class CTemp {
	public:
		CTemp();
		explicit CTemp(const CSymbol* symbol);
		const std::string& Name() const { return name; }

	private:
		static int nextUniqueId;
		std::string name;
	};

}

