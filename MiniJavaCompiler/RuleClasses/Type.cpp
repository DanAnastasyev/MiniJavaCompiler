#include "RuleClasses.h"

CStandardType::CStandardType( StandardType _type, const CPosition& _position ) :
CPositionStorage( _position ),
type( _type )
{
}

CStandardType::CStandardType( const CStandardType* other ) :
CPositionStorage( other->GetPosition() ),
type( other->GetType() )
{
}

bool CStandardType::IsPODType() const
{
	return true;
}

CStandardType::StandardType CStandardType::GetType() const
{
	return type;
}

void CStandardType::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CUserType::CUserType( const std::string& _typeName, const CPosition& _position ) :
CPositionStorage(_position ),
typeName( CSymbol::GetSymbol(_typeName) )
{
}

CUserType::CUserType( const CUserType* other ) :
CPositionStorage( other->GetPosition() ),
typeName( other->GetTypeName( ) )
{
}

const CSymbol* CUserType::GetTypeName() const
{
	return typeName;
}

bool CUserType::IsPODType() const
{
	return false;
}

void CUserType::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}