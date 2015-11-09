#include "RuleClasses.h"

//CFormalList::CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos ) :
//CPositionStorage( pos ),
//type( _type ),
//identifier( CSymbol::GetSymbol(_identifier) ),
//formalList( _formalList )
//{
//}
//
//const IFormalList* CFormalList::GetFormalRest() const
//{
//	return formalList.get();
//}
//
//const IType* CFormalList::GetType() const
//{
//	return type.get();
//}
//
//const CSymbol* CFormalList::GetIdentifier() const
//{
//	return identifier;
//}

CFormalList::CFormalList( IFormalList* _formalList, const CPosition& pos ) :
CPositionStorage( pos )
{
	if( _formalList != nullptr ) {
		formalList = dynamic_cast<CFormalList*>(_formalList)->GetParamList();
	}
}

void CFormalList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

std::vector<std::shared_ptr<IFormalList>> CFormalList::GetParamList() const 
{
	return formalList;
}

void CFormalList::PushParam( IFormalList* param )
{
	formalList.push_back( std::shared_ptr<IFormalList>( param ) );
}

CFormalParam::CFormalParam( IType* _type, const std::string& _identifier, const CPosition& pos ) :
CPositionStorage(pos),
type(_type),
identifier( CSymbol::GetSymbol( _identifier ) )
{
}

void CFormalParam::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

const IType* CFormalParam::GetType() const 
{
	return type.get();
}
const CSymbol* CFormalParam::GetIdentifier() const
{
	return identifier;
}

//CFormalRestList::CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos ) :
//CPositionStorage( pos ),
//formalRest( _formalRest ),
//formalRestList( _formalRestList )
//{
//}
//
//const IFormalList* CFormalRestList::GetFormalRest() const
//{
//	return formalRest.get();
//}
//
//const IFormalList* CFormalRestList::GetFormalRestList() const
//{
//	return formalRestList.get();
//}
//
//void CFormalRestList::Accept( IVisitor* visitor ) const
//{
//	visitor->Visit( this );
//}