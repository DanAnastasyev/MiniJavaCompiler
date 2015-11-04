#include "Visitors/TypeCheckerVisitor.h"
#include "SymbolsTable/SymbolsTable.h"
#include "RuleClasses/RuleClasses.h"

void CTypeCheckerVisitor::Visit( const CProgram* program )
{
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass )
{
	if( mainClass->GetStatement() != nullptr ) {
		mainClass->GetStatement()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CClassDeclList* classDeclList )
{
	if( classDeclList->GetClassDecl() != nullptr ) {
		classDeclList->GetClassDecl()->Accept( this );
	}
	if( classDeclList->GetClassDeclList() != nullptr ) {
		classDeclList->GetClassDeclList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CClassDecl* classDecl )
{
	if( classDecl->GetVarDeclList() != nullptr ) {
		classDecl->GetVarDeclList()->Accept( this );
	}
	if( classDecl->GetMethodDeclList() != nullptr ) {
		classDecl->GetMethodDeclList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	if( classDeclDerived->GetVarDeclList() != nullptr ) {
		classDeclDerived->GetVarDeclList()->Accept( this );
	}
	if( classDeclDerived->GetMethodDeclList() != nullptr ) {
		classDeclDerived->GetMethodDeclList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CVarDecl* varDecl )
{
	if( varDecl->GetType() != nullptr ) {
		varDecl->GetType()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CVarDeclList* varDeclList )
{
	if( varDeclList->GetVarDeclList() != nullptr ) {
		varDeclList->GetVarDeclList()->Accept( this );
	}
	if( varDeclList->GetVarDecl() != nullptr ) {
		varDeclList->GetVarDecl()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CMethodDecl* methodDecl )
{
	if( methodDecl->GetType() != nullptr ) {
		methodDecl->GetType()->Accept( this );
	}
	if( methodDecl->GetFormalList() != nullptr ) {
		methodDecl->GetFormalList()->Accept( this );
	}
	if( methodDecl->GetVarList() != nullptr ) {
		methodDecl->GetVarList()->Accept( this );
	}
	if( methodDecl->GetStatementList() != nullptr ) {
		methodDecl->GetStatementList()->Accept( this );
	}
	if( methodDecl->GetReturnExpression() != nullptr ) {
		methodDecl->GetReturnExpression()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CStandardType* type )
{
	switch( type->GetType() ) {
	case CStandardType::StandardType::INT:
		lastTypeValue = "INT";
		break;
	case CStandardType::StandardType::BOOL:
		lastTypeValue = "BOOL";
		break;
	case CStandardType::StandardType::INT_ARRAY:
		lastTypeValue = "INT_ARRAY";
		break;
	default:
		break;
	}
}

void CTypeCheckerVisitor::Visit( const CUserType* type )
{
	lastTypeValue = type->GetTypeName()->GetString();
}

void CTypeCheckerVisitor::Visit( const CStatementListStatement* statement )
{

}

void CTypeCheckerVisitor::Visit( const CArrayAssignStatement* statement )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Assign statement out of scope " + statement->GetPosition().ToString() );
		return;
	}
	if( curMethod == nullptr ) {
		errorStorage.PutError( "Assign statement out of scope " + statement->GetPosition().ToString() );
		return;
	}
	SymbolsTable::CVarInfo* var = curMethod->GetVar( statement->GetArrayName()->GetString() );
	var->GetType()->Accept( this );
	std::string leftType = lastTypeValue;
	statement->GetRightPart()->Accept( this );
	if( leftType != lastTypeValue ) {
		errorStorage.PutError( "Incompatible types " + statement->GetPosition().ToString() );
	}
}

void CTypeCheckerVisitor::Visit( const CAssignStatement* statement )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Assign statement out of scope " + statement->GetPosition().ToString() );
		return;
	}
	if( curMethod == nullptr ) {
		errorStorage.PutError( "Assign statement out of scope " + statement->GetPosition().ToString() );
		return;
	}
	SymbolsTable::CVarInfo* var = curMethod->GetVar( statement->GetLeftPart( )->GetString( ) );
	var->GetType()->Accept( this );
	std::string leftType = lastTypeValue;
	statement->GetRightPart()->Accept( this );
	if( leftType != lastTypeValue ) {
		errorStorage.PutError( "Incompatible types " + statement->GetPosition().ToString() );
	}
}

void CTypeCheckerVisitor::Visit( const CIfStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CWhileStatement* statement )
{
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement )
{
	if( statement->GetExpression() != nullptr ) {
		statement->GetExpression()->Accept( this );
	}
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
	if( exprList->GetExp( ) != nullptr ) {
		exprList->GetExp( )->Accept( this );
	}
	if( exprList->GetExpList( ) != nullptr ) {
		exprList->GetExpList( )->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CStatementList* stmtList )
{
	if( stmtList->GetStatement( ) != nullptr ) {
		stmtList->GetStatement( )->Accept( this );
	}
	if( stmtList->GetStatementList( ) != nullptr ) {
		stmtList->GetStatementList( )->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CExpressionRest* expr )
{
	if( expr->GetExp( ) != nullptr ) {
		expr->GetExp( )->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CMethodDeclList* methodList )
{
	if( methodList->GetMethodDecl( ) != nullptr ) {
		methodList->GetMethodDecl( )->Accept( this );
	}
	if( methodList->GetMethodDeclList( ) != nullptr ) {
		methodList->GetMethodDeclList( )->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CFormalList* list )
{
	if( list->GetType( ) != nullptr ) {
		list->GetType( )->Accept( this );
	}
	if( list->GetFormalRest( ) != nullptr ) {
		list->GetFormalRest( )->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CFormalRestList* list )
{
	if( list->GetFormalRest( ) != nullptr ) {
		list->GetFormalRest( )->Accept( this );
	}
	if( list->GetFormalRestList( ) != nullptr ) {
		list->GetFormalRestList( )->Accept( this );
	}
}