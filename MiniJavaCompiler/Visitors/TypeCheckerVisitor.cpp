#include <set>

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
	curClass = symbolsTable->GetClass( mainClass->GetClassName()->GetString() );

	if( mainClass->GetStatement() != nullptr ) {
		mainClass->GetStatement()->Accept( this );
	}

	curClass = nullptr;
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
	curClass = symbolsTable->GetClass( classDecl->GetName()->GetString() );
	
	if( classDecl->GetVarDeclList() != nullptr ) {
		classDecl->GetVarDeclList()->Accept( this );
	}
	if( classDecl->GetMethodDeclList() != nullptr ) {
		classDecl->GetMethodDeclList()->Accept( this );
	}

	curClass = nullptr;
}

void CTypeCheckerVisitor::Visit( const CClassDeclDerived* classDeclDerived )
{
	curClass = symbolsTable->GetClass( classDeclDerived->GetName()->GetString() );

	auto base = curClass->GetBaseClass();
	std::set<SymbolsTable::CClassInfo*> hierarchy;
	while( base != nullptr ) {
		if( hierarchy.find( base ) != hierarchy.end() ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CClassDeclDerived. ") +
				base->GetName() +
				" - Cannot derive class from itself. " +
				"Line " + std::to_string( classDeclDerived->GetPosition( ).GetBeginPos( ).first ) +
				", column " + std::to_string( classDeclDerived->GetPosition( ).GetBeginPos( ).second ) + "." );
			break;
		}
		hierarchy.insert( base );
	}

	if( classDeclDerived->GetVarDeclList() != nullptr ) {
		classDeclDerived->GetVarDeclList()->Accept( this );
	}
	if( classDeclDerived->GetMethodDeclList() != nullptr ) {
		classDeclDerived->GetMethodDeclList()->Accept( this );
	}

	curClass = nullptr;
}

void CTypeCheckerVisitor::Visit( const CVarDecl* varDecl )
{
	varDecl->GetType()->Accept( this );
	
	if( varDecl->GetType()->IsPODType() ) {
		return;
	}
	
	auto typeName = dynamic_cast<CUserType*>(lastTypeValue.get())->GetTypeName()->GetString();
	if (symbolsTable->GetClass( typeName ) == nullptr) {
		errorStorage.PutError( std::string( "[Type check] Node type - CVarDecl. " ) + 
			typeName + " " + varDecl->GetName()->GetString() + 
			" - incorrect variable declaration, class " + typeName + " not found. " +
			"Line " + std::to_string(varDecl->GetPosition().GetBeginPos().first) +
			", column " + std::to_string(varDecl->GetPosition().GetBeginPos().second) + ".");
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
	methodDecl->GetType()->Accept( this );

	auto typeName = dynamic_cast<CUserType*>(lastTypeValue.get( ))->GetTypeName( )->GetString( );
	if( symbolsTable->GetClass( typeName ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CMethodDecl. " ) +
			typeName + " " + methodDecl->GetName( )->GetString( ) +
			" - incorrect method declaration, class " + typeName + " not found. " +
			"Line " + std::to_string( methodDecl->GetPosition( ).GetBeginPos( ).first ) +
			", column " + std::to_string( methodDecl->GetPosition( ).GetBeginPos( ).second ) + "." );
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
	lastTypeValue = std::make_shared<CStandardType>(type);
}

void CTypeCheckerVisitor::Visit( const CUserType* type )
{
	lastTypeValue = std::make_shared<CUserType>( type );
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
	lastTypeValue = "int";
}

void CTypeCheckerVisitor::Visit( const CBoolLiteralExpression* expr )
{
}

void CTypeCheckerVisitor::Visit( const CIdentifierExpression* expr )
{
	if( curClass == nullptr ) {
		lastTypeValue = nullptr;
		return;
	}

	if( curMethod == nullptr ) { // если находимся внутри класса, но снаружи всех методов
		auto varInfo = curClass->GetVar( expr->GetIdentifier()->GetString() );
		if( varInfo == nullptr ) {
			lastTypeValue = nullptr;
			return;
		}
		lastTypeValue = std::make_shared<IType*>( varInfo->GetType() );
	}

	// сначала пытаемся найти локальную переменную или параметр
	auto varInfo = curMethod->GetVar( expr->GetIdentifier()->GetString() );
	if( varInfo == nullptr ) { // потом среди полей класса
		varInfo = curClass->GetVar( expr->GetIdentifier()->GetString() );
		if( varInfo == nullptr ) {
			lastTypeValue = nullptr;
			return;
		}
	}
	lastTypeValue = std::make_shared<IType*>( varInfo->GetType() );
}

void CTypeCheckerVisitor::Visit( const CThisExpression* expr )
{
	if( curClass == nullptr ) {
		lastTypeValue = nullptr;
		return;
	}
	lastTypeValue = symbolsTable->GetClass( curClass->GetName() );
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
	if( exprList->GetExp() != nullptr ) {
		exprList->GetExp()->Accept( this );
	}
	if( exprList->GetExpList() != nullptr ) {
		exprList->GetExpList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CStatementList* stmtList )
{
	if( stmtList->GetStatement() != nullptr ) {
		stmtList->GetStatement()->Accept( this );
	}
	if( stmtList->GetStatementList() != nullptr ) {
		stmtList->GetStatementList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CExpressionRest* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CMethodDeclList* methodList )
{
	if( methodList->GetMethodDecl() != nullptr ) {
		methodList->GetMethodDecl()->Accept( this );
	}
	if( methodList->GetMethodDeclList() != nullptr ) {
		methodList->GetMethodDeclList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CFormalList* list )
{
	if( list->GetType() != nullptr ) {
		list->GetType()->Accept( this );
	}
	if( list->GetFormalRest() != nullptr ) {
		list->GetFormalRest()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CFormalRestList* list )
{
	if( list->GetFormalRest() != nullptr ) {
		list->GetFormalRest()->Accept( this );
	}
	if( list->GetFormalRestList() != nullptr ) {
		list->GetFormalRestList()->Accept( this );
	}
}