#include "RuleClasses.h"

CMainClass::CMainClass( const std::string& _identifier, const std::string& _arsId, IStatement* _statement, const CPosition& _position ) :
	identifier( CSymbol::GetSymbol(_identifier) ),
	argsIdentifier(CSymbol::GetSymbol(_arsId) ),
	statement( _statement ),
	position( _position )
{
}

const IStatement* CMainClass::GetStatement() const
{
	return statement.get();
}

const CSymbol* CMainClass::GetClassName() const
{
	return identifier;
}

const CSymbol* CMainClass::GetMainArgsIdentifier() const
{
	return argsIdentifier;
}

void CMainClass::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}