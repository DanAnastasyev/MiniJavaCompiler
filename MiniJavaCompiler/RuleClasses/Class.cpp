#include "RuleClasses.h"

CClassDeclList::CClassDeclList( IClassDecl* _declaration, IClassDeclList* _list, const CPosition& _position ) :
	CPositionStorage( _position ),
	list( _list ),
	declaration( _declaration )
{
}

const IClassDeclList* CClassDeclList::GetClassDeclList() const
{
	return list.get();
}

const IClassDecl* CClassDeclList::GetClassDecl() const
{
	return declaration.get();
}

void CClassDeclList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CClassDecl::CClassDecl( IVarDeclList* _varList, IMethodDeclList* _methodList, const std::string& _className, const CPosition& _position ) :
CPositionStorage( _position ),
className( CSymbol::GetSymbol( _className ) ),
varList( _varList ),
methodList( _methodList )
{
}

const CSymbol* CClassDecl::GetName() const
{
	return className;
}

const IVarDeclList* CClassDecl::GetVarDeclList() const
{
	return varList.get();
}

const IMethodDeclList* CClassDecl::GetMethodDeclList() const
{
	return methodList.get();
}

void CClassDecl::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CClassDeclDerived::CClassDeclDerived( IVarDeclList* _varList, IMethodDeclList* _methodList, const std::string& _className, const std::string& _baseClassName, const CPosition& _position ) :
CPositionStorage( _position ),
className( CSymbol::GetSymbol( _className ) ),
baseClassName( CSymbol::GetSymbol(_baseClassName) ),
varList( _varList ),
methodList( _methodList )
{
}

const CSymbol* CClassDeclDerived::GetName() const
{
	return className;
}

const CSymbol* CClassDeclDerived::GetBaseClassName() const
{
	return baseClassName;
}

const IVarDeclList* CClassDeclDerived::GetVarDeclList() const
{
	return varList.get();
}

const IMethodDeclList* CClassDeclDerived::GetMethodDeclList() const
{
	return methodList.get();
}

void CClassDeclDerived::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}