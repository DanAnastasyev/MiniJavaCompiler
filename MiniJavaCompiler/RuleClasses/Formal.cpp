#include "RuleClasses.h"

CFormalList::CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos ) :
CPositionStorage( pos ),
type( _type ),
identifier( CSymbol::GetSymbol(_identifier) ),
formalList( _formalList )
{
}

const IFormalList* CFormalList::GetFormalRest() const
{
	return formalList.get();
}

const IType* CFormalList::GetType() const
{
	return type.get();
}

const CSymbol* CFormalList::GetIdentifier() const
{
	return identifier;
}

void CFormalList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CFormalRestList::CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos ) :
CPositionStorage( pos ),
formalRest( _formalRest ),
formalRestList( _formalRestList )
{
}

const IFormalList* CFormalRestList::GetFormalRest() const
{
	return formalRest.get();
}

const IFormalList* CFormalRestList::GetFormalRestList() const
{
	return formalRestList.get();
}

void CFormalRestList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}