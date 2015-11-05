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

	// пробег вверх по иерархии, чтобы найти циклическую зависимость при наследовании
	auto base = curClass->GetBaseClass();
	std::set<std::string> hierarchy;
	while( base != nullptr ) {
		if( hierarchy.find( base->GetName() ) != hierarchy.end() ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CClassDeclDerived. ") +
				base->GetName() +
				" - Cannot derive class from itself. " +
				"Line " + std::to_string( classDeclDerived->GetPosition( ).GetBeginPos( ).first ) +
				", column " + std::to_string( classDeclDerived->GetPosition( ).GetBeginPos( ).second ) + "." );
			break;
		}
		hierarchy.insert( base->GetName() );
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
	if( curClass == nullptr ) {
		errorStorage.PutError( "Variable declaration out of scope " + varDecl->GetPosition().ToString() );
		return;
	}
	varDecl->GetType()->Accept( this );

	if( !isPODType( lastTypeValue ) && symbolsTable->GetClass( lastTypeValue ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CVarDecl. " ) +
			lastTypeValue + " " + varDecl->GetName()->GetString() +
			" - incorrect variable declaration, class " + lastTypeValue + " not found. " +
			"Line " + std::to_string( varDecl->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( varDecl->GetPosition().GetBeginPos().second ) + "." );
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
	if( curClass == nullptr ) {
		errorStorage.PutError( "Method declaration out of scope " + methodDecl->GetPosition().ToString() );
		return;
	}
	methodDecl->GetType()->Accept( this );

	if( !isPODType( lastTypeValue ) && symbolsTable->GetClass( lastTypeValue ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CMethodDecl. " ) +
			lastTypeValue + " " + methodDecl->GetName()->GetString() +
			" - incorrect method declaration, class " + lastTypeValue + " not found. " +
			"Line " + std::to_string( methodDecl->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( methodDecl->GetPosition().GetBeginPos().second ) + "." );
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
		lastTypeValue = ".INT";
		break;
	case CStandardType::StandardType::BOOL:
		lastTypeValue = ".BOOL";
		break;
	case CStandardType::StandardType::INT_ARRAY:
		lastTypeValue = ".INT_ARRAY";
		break;
	default:
		lastTypeValue = nullptr;
		break;
	}
}

void CTypeCheckerVisitor::Visit( const CUserType* type )
{
	lastTypeValue = type->GetTypeName()->GetString();
}

void CTypeCheckerVisitor::Visit( const CStatementListStatement* statement )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "While statement out of scope " + statement->GetPosition().ToString() );
		return;
	}

	if( statement->GetStatementList() != nullptr ) {
		statement->GetStatementList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CArrayAssignStatement* statement )
{
	if( curClass == nullptr || curMethod == nullptr ) {
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
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Assign statement out of scope " + statement->GetPosition().ToString() );
		return;
	}

	SymbolsTable::CVarInfo* var = curMethod->GetVar( statement->GetLeftPart()->GetString() );
	var->GetType()->Accept( this );
	std::string leftType = lastTypeValue;
	statement->GetRightPart()->Accept( this );
	if( leftType != lastTypeValue ) {
		errorStorage.PutError( "Incompatible types " + statement->GetPosition().ToString() );
	}
}

void CTypeCheckerVisitor::Visit( const CIfStatement* statement )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "If statement out of scope " + statement->GetPosition().ToString() );
		return;
	}

	statement->GetCondition()->Accept( this );

	if( lastTypeValue != ".BOOL" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CIfStatement. " ) +
			"Expression of type " + lastTypeValue + " cannot be used as condition. " +
			"Line " + std::to_string( statement->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( statement->GetPosition().GetBeginPos().second ) + "." );
	}

	if( statement->GetIfTrueStatement() != nullptr ) {
		statement->GetIfTrueStatement()->Accept( this );
	}
	if( statement->GetIfFalseStatement() != nullptr ) {
		statement->GetIfFalseStatement()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CWhileStatement* statement )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "While statement out of scope " + statement->GetPosition( ).ToString( ) );
		return;
	}

	statement->GetCondition()->Accept( this );

	if( lastTypeValue != ".BOOL" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CWhileStatement. " ) +
			"Expression of type " + lastTypeValue + " cannot be used as condition. " +
			"Line " + std::to_string( statement->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( statement->GetPosition().GetBeginPos().second ) + "." );
	}

	if( statement->GetBodyCycle() != nullptr ) {
		statement->GetBodyCycle()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CPrintStatement* statement )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "While statement out of scope " + statement->GetPosition().ToString() );
		return;
	}

	statement->GetExpression()->Accept( this );

	if( lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CPrintStatement. " ) +
			"Expression of type " + lastTypeValue + " cannot be printed. Simple java can only print variables of type 'int'. " +
			"Line " + std::to_string( statement->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( statement->GetPosition().GetBeginPos().second ) + "." );
	}
}

void CTypeCheckerVisitor::Visit( const CBinOpExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}

	expr->GetLeftExp()->Accept( this );
	auto left = lastTypeValue;
	expr->GetRightExp()->Accept( this );
	auto right = lastTypeValue;

	if( (expr->GetBinOp() == CBinOpExpression::MINUS || expr->GetBinOp() == CBinOpExpression::PLUS ||
		expr->GetBinOp() == CBinOpExpression::TIMES || expr->GetBinOp() == CBinOpExpression::DIVIDE) &&
		(left != ".INT" || right != ".INT") ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CBinOpExpression. " ) +
			"Expression of type " + lastTypeValue + " cannot be used in arithmetic operations, use 'int'. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}
	if( expr->GetBinOp() == CBinOpExpression::AND && (left != ".BOOL" || right != ".BOOL") ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CBinOpExpression. " ) +
			"Expression of type " + lastTypeValue + " cannot be used in logic operations, use 'bool'. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValue = ".INT";
}

void CTypeCheckerVisitor::Visit( const CIndexExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}

	expr->GetExp()->Accept( this );
	if( lastTypeValue != ".INT_ARRAY" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CIndexExpression. " ) +
			"Cannot get element of not variable which is not array. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}
	expr->GetIndexExp()->Accept( this );
	if( lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CIndexExpression. " ) +
			"Cannot get element of array, when index is not an integer. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValue = ".INT";
}

void CTypeCheckerVisitor::Visit( const CLenghtExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition( ).ToString( ) );
		lastTypeValue = nullptr;
		return;
	}

	expr->GetExp()->Accept( this );

	if( lastTypeValue != ".INT_ARRAY" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CLengthExpression. " ) +
			"Cannot get length of not array expression. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValue = ".INT";
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expr )
{
	not implemented;
}

void CTypeCheckerVisitor::Visit( const CIntLiteralExpression* expr )
{
	lastTypeValue = ".INT";
}

void CTypeCheckerVisitor::Visit( const CBoolLiteralExpression* expr )
{
	lastTypeValue = ".BOOL";
}

void CTypeCheckerVisitor::Visit( const CIdentifierExpression* expr )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}

	if( curMethod == nullptr ) { // если находимся внутри класса, но снаружи всех методов
		auto varInfo = curClass->GetVar( expr->GetIdentifier()->GetString() );
		if( varInfo == nullptr ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CIdentifierExpression. " ) +
				"Undeclared identifier " + expr->GetIdentifier()->GetString() + ". " +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
			lastTypeValue = nullptr;
			return;
		}
		varInfo->GetType()->Accept( this );
		return;
	}

	// сначала пытаемся найти локальную переменную или параметр
	auto varInfo = curMethod->GetVar( expr->GetIdentifier()->GetString() );
	if( varInfo == nullptr ) { // потом среди полей класса
		varInfo = curClass->GetVar( expr->GetIdentifier()->GetString() );
		if( varInfo == nullptr ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CIdentifierExpression. " ) +
				"Undeclared identifier " + expr->GetIdentifier()->GetString() + ". " +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
			lastTypeValue = nullptr;
			return;
		}
	}
	varInfo->GetType()->Accept( this );
}

void CTypeCheckerVisitor::Visit( const CThisExpression* expr )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}
	lastTypeValue = curClass->GetName();
}

void CTypeCheckerVisitor::Visit( const CNewIntArrayExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}

	expr->GetExp()->Accept( this );
	if( lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CNewIntArrayExpression. " ) +
			"Cannot create array of non integer length. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValue = ".INT_ARRAY";
}

void CTypeCheckerVisitor::Visit( const CNewExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValue = nullptr;
		return;
	}

	if( symbolsTable->GetClass( expr->GetIdentifier()->GetString() ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CNewExpression. " ) +
			"Undefined class name - " + expr->GetIdentifier()->GetString() + ". " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValue = expr->GetIdentifier()->GetString();
}

void CTypeCheckerVisitor::Visit( const CUnaryOpExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition( ).ToString( ) );
		return;
	}
	
	expr->GetLeftExp( )->Accept( this );
	if( expr->GetOperation( ) == CUnaryOpExpression::MINUS && lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CUnaryOpExpression. " ) +
			"Expression of type " + lastTypeValue + " cannot be used in arithmetic operations, use 'int'. " +
			"Line " + std::to_string( expr->GetPosition( ).GetBeginPos( ).first ) +
			", column " + std::to_string( expr->GetPosition( ).GetBeginPos( ).second ) + "." );
	}

	lastTypeValue = ".INT";
}

void CTypeCheckerVisitor::Visit( const CBracesExpression* expr )
{
	not implemented
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
	list->GetType()->Accept( this );

	if( !isPODType( lastTypeValue ) && symbolsTable->GetClass( lastTypeValue ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CFormalList. " ) +
			lastTypeValue + " " + list->GetIdentifier()->GetString() +
			" - wrong argument type in method declaration, class " + lastTypeValue + " not found. " +
			"Line " + std::to_string( list->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( list->GetPosition().GetBeginPos().second ) + "." );
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

const CErrorStorage& CTypeCheckerVisitor::GetErrorStorage() const
{
	return errorStorage;
}

bool CTypeCheckerVisitor::isPODType( const std::string& type ) const
{
	return  type == ".INT" || type == ".BOOL" || type == ".INT_ARRAY";
}