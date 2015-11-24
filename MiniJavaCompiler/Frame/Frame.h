#pragma once

#include "SymbolsTable\Symbol.h"

namespace Frame {
	class IAccess {
	public:
		virtual ~IAccess() {}
	};

	class CFrame {
	public:
		CFrame( const CSymbol* _name, int _formalsCount );
		const IAccess* Formal( size_t index ) const;
	private:
		const CSymbol*  name;
		const int formalsCount;
		std::vector<IAccess> formals;
	};
}
