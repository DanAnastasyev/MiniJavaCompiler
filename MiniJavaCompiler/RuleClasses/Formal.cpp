#include "RuleClasses.h"

CFormalList::CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos ) :
type( _type ),
identifier( _identifier ),
formalList( _formalList ),
position( pos )
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

std::string CFormalList::GetIdentifier() const
{
	return identifier;
}

CFormalRestList::CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos ) :
formalRest( _formalRest ),
formalRestList( _formalRestList ),
position( pos )
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