#include "Temp.h"

namespace Temp {
	int CLabel::nextUniqueId = 0;

	CLabel::CLabel()
	{
		name = CSymbol::GetSymbol( std::to_string( nextUniqueId++ ) );
	}

	CLabel::CLabel( const CSymbol* label ) :
		name( label )
	{}

	int CTemp::nextUniqueId = 0;

	CTemp::CTemp()
	{
		name = CSymbol::GetSymbol( std::to_string( nextUniqueId++ ) );
	}

	CTemp::CTemp( const CSymbol* symbol ) :
		name( symbol )
	{}

}