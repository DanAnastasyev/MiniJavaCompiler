#include "Visitors/IRBuilderVisitor.h"
#include "RuleClasses/RuleClasses.h"

void IRBuilderVisitor::Visit( const CProgram* program )
{
	program->GetMainClass()->Accept( this );
	program->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMainClass* mainClass )
{
	mainClass->GetStatement()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDeclList* classDeclList )
{
	classDeclList->GetClassDecl()->Accept( this );
	classDeclList->GetClassDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDecl* classDecl )
{
	classDecl->GetVarDeclList()->Accept( this );
	classDecl->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	classDeclDerived->GetVarDeclList()->Accept( this );
	classDeclDerived->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CVarDecl* varDecl )
{
	varDecl->GetType()->Accept( this );
}

void IRBuilderVisitor::Visit( const CVarDeclList* varDeclList )
{
	varDeclList->GetVarDeclList()->Accept( this );
	varDeclList->GetVarDecl()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMethodDecl* methodDecl )
{
	methodDecl->GetType()->Accept( this );
	methodDecl->GetFormalList()->Accept( this );
	methodDecl->GetVarList()->Accept( this );
	methodDecl->GetStatementList()->Accept( this );
	methodDecl->GetReturnExpression()->Accept( this );
}

void IRBuilderVisitor::Visit( const CStandardType* type )
{
}

void IRBuilderVisitor::Visit( const CUserType* type )
{
}

void IRBuilderVisitor::Visit( const CStatementListStatement* statement )
{
	statement->GetStatementList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CArrayAssignStatement* statement )
{

}

void IRBuilderVisitor::Visit( const CAssignStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CIfStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CWhileStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CPrintStatement* statement )
{
}

void IRBuilderVisitor::Visit( const CBinOpExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CIndexExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CLenghtExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CMethodExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CIntLiteralExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CBoolLiteralExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CIdentifierExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CThisExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CNewIntArrayExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CNewExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CUnaryOpExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CBracesExpression* expr )
{
}

void IRBuilderVisitor::Visit( const CExpressionList* exprList )
{
	exprList->GetExp()->Accept( this );
	exprList->GetExpList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CStatementList* stmtList )
{
	stmtList->GetStatement()->Accept( this );
	stmtList->GetStatementList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CExpressionRest* expr )
{
	expr->GetExp()->Accept( this );
}

void IRBuilderVisitor::Visit( const CMethodDeclList* methodList )
{
	methodList->GetMethodDecl()->Accept( this );
	methodList->GetMethodDeclList()->Accept( this );
}

void IRBuilderVisitor::Visit( const CFormalList* list )
{
}

void IRBuilderVisitor::Visit( const CFormalParam* list )
{
	list->GetType()->Accept( this );
}