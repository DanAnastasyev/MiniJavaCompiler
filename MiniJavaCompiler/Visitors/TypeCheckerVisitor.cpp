#include "Visitors/TypeCheckerVisitor.h"
#include "SymbolsTable/SymbolsTable.h"
#include "RuleClasses/RuleClasses.h"

void CTypeCheckerVisitor::Visit( const CProgram* program )
{
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass )
{
}

void CTypeCheckerVisitor::Visit( const CClassDeclList* classDeclList )
{
}

void CTypeCheckerVisitor::Visit( const CClassDecl* classDecl )
{
}

void CTypeCheckerVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
}

void CTypeCheckerVisitor::Visit( const CVarDecl* varDecl )
{
}

void CTypeCheckerVisitor::Visit( const CVarDeclList* varDeclList )
{
}

void CTypeCheckerVisitor::Visit( const CMethodDecl* methodDecl )
{
}

void CTypeCheckerVisitor::Visit( const CStandardType* type )
{
}

void CTypeCheckerVisitor::Visit( const CUserType* type )
{
}

void CTypeCheckerVisitor::Visit( const CStatementListStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CArrayAssignStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CAssignStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CIfStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CWhileStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CBinOpExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CIndexExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CLenghtExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CIntLiteralExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CBoolLiteralExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CIdentifierExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CThisExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CNewIntArrayExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CNewExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CUnaryOpExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CBracesExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CExpressionList* exprList )
{
}

void CTypeCheckerVisitor::Visit( const CStatementList* stmtList )
{
}

void CTypeCheckerVisitor::Visit( const CExpressionRest* expr )
{
}

void CTypeCheckerVisitor::Visit( const CMethodDeclList* methodList )
{
}

void CTypeCheckerVisitor::Visit( const CFormalList* list )
{
}

void CTypeCheckerVisitor::Visit( const CFormalRestList* list )
{
}