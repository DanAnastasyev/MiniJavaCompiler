#include "RuleClasses.h"

CVarDecl::CVarDecl( IType* _type, const std::string& _identifier, const CPosition& _position ) :
CPositionStorage( _position ),
type( _type ),
identifier( CSymbol::GetSymbol(_identifier) )
{
}

const IType* CVarDecl::GetType() const
{
	return type.get();
}

const CSymbol* CVarDecl::GetName() const
{
	return identifier;
}

void CVarDecl::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CVarDeclList::CVarDeclList( IVarDeclList* _varDeclList, IVarDecl* _varDecl, const CPosition& pos ) :
CPositionStorage( pos ),
varDeclList( _varDeclList ),
varDecl( _varDecl )
{
}

const IVarDeclList* CVarDeclList::GetVarDeclList() const
{
	return varDeclList.get();
}

const IVarDecl* CVarDeclList::GetVarDecl() const
{
	return varDecl.get();
}

void CVarDeclList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}