#include "RuleClasses.h"

CMethodDecl::CMethodDecl( IType* _type, const std::string& _methodName, IFormalList* _formalList, IVarDeclList* _varList, IStatementList* _statementList, IExp* _returnExpr, const CPosition& _position ) :
CPositionStorage( _position ),
type( _type ),
methodName( CSymbol::GetSymbol(_methodName) ),
formalList( _formalList ),
varList( _varList ),
statementList( _statementList ),
returnExpr( _returnExpr )
{
}

const IType* CMethodDecl::GetType() const
{
	return type.get();
}

const CSymbol* CMethodDecl::GetName() const
{
	return methodName;
}

const IFormalList* CMethodDecl::GetFormalList() const
{
	return formalList.get();
}

const IVarDeclList* CMethodDecl::GetVarList() const
{
	return varList.get();
}

const IStatementList* CMethodDecl::GetStatementList() const
{
	return statementList.get();
}

const IExp* CMethodDecl::GetReturnExpression() const
{
	return returnExpr.get();
}

void CMethodDecl::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}

CMethodDeclList::CMethodDeclList( IMethodDecl* _methodDecl, IMethodDeclList* _methodDeclList, const CPosition& pos ) :
CPositionStorage( pos ),
methodDeclList( _methodDeclList ),
methodDecl( _methodDecl )
{
}

const IMethodDeclList* CMethodDeclList::GetMethodDeclList() const
{
	return methodDeclList.get();
}

const IMethodDecl* CMethodDeclList::GetMethodDecl() const
{
	return methodDecl.get();
}

void CMethodDeclList::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}
