#pragma once

#include <SymbolsTable/Symbol.h>

namespace Frame
{
	class IAccess {
	public:
		virtual ~IAccess()
		{
		}
	};
	
	class CFrame {
	public:
		CFrame( const CSymbol* name, int formalsCount );
		int FormalsCount() const
		{
			return formals.size();
		}
		const IAccess* Formal( size_t index ) const;

	private:
		std::vector<IAccess> formals;

	};
}