#include "Frame/Temp.h"

namespace Temp {
	int CLabel::nextUniqueId = 0;

	CLabel::CLabel()
	{
		name = CSymbol::GetSymbol( std::to_string( nextUniqueId++ ) );
	}

	CLabel::CLabel( const CSymbol* label ) :
		name( label )
	{}

	const CSymbol* CLabel::GetName() const
	{
		return name;
	}

	std::shared_ptr<const CLabel> CLabelList::Head() const
	{
		return head;
	}

	std::shared_ptr<const CLabelList> CLabelList::Tail() const
	{
		return tail;
	}

int CTemp::nextUniqueId = 0;

	CTemp::CTemp()
	{
		name = CSymbol::GetSymbol( std::to_string( nextUniqueId++ ) );
	}

	CTemp::CTemp( const CSymbol* symbol ) :
		name( symbol )
	{}

	const CSymbol* CTemp::GetName() const
	{
		return name;
	}

}