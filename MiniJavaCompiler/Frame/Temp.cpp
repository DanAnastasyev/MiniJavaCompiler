#include "Temp.h"

namespace Temp {
	int CLabel::nextUniqueId = 0;

	CLabel::CLabel()
	{
		name = std::to_string( nextUniqueId++ );
	}

	CLabel::CLabel( std::string& _name ) :
		name( _name )
	{}

	int CTemp::nextUniqueId = 0;

	CTemp::CTemp()
	{
		name = std::to_string( nextUniqueId++ );
	}

	CTemp::CTemp( const CSymbol* symbol ) :
		name( symbol->GetString() )
	{}

}