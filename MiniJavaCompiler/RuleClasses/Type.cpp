#include "RuleClasses.h"

CStandardType::CStandardType( StandardType _type, const CPosition& _position ) :
	type( _type ),
	position( _position )
{
}

CStandardType::CStandardType( const CStandardType* other ) :
	type( other->GetType() ),
	position( other->GetPosition() )
{
}

CStandardType::StandardType CStandardType::GetType() const
{
	return type;
}

void CStandardType::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CPosition CStandardType::GetPosition() const
{
	return position;
}

CUserType::CUserType( const std::string& _typeName, const CPosition& _position ) :
	typeName( _typeName ),
	position( _position )
{
}

CUserType::CUserType( const CUserType* other ) :
	typeName( other->GetTypeName() ),
	position( other->GetPosition() )
{
}

std::string CUserType::GetTypeName() const
{
	return typeName;
}

void CUserType::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CPosition CUserType::GetPosition( ) const
{
	return position;
}