#include "RuleClasses.h"

CMainClass::CMainClass( const std::string& _identifier, const std::string& _arsId, IStatement* _statement, const CPosition& _position ) :
	identifier( _identifier ),
	argsIdentifier( _arsId ),
	statement( _statement ),
	position( _position )
{
}

const IStatement* CMainClass::GetStatement() const
{
	return statement.get();
}

std::string CMainClass::GetClassName() const
{
	return identifier;
}

std::string CMainClass::GetMainArgsIdentifier() const
{
	return argsIdentifier;
}

void CMainClass::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}