#include "RuleClasses.h"

CStandardType::CStandardType( StandardType _type, const CPosition& _position ) :
type( _type ),
position( _position )
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

CUserType::CUserType( const std::string& _typeName, const CPosition& _position ) :
typeName( _typeName ),
position( _position )
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