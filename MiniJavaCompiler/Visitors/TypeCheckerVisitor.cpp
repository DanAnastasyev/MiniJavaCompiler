#include <set>

#include "Visitors/TypeCheckerVisitor.h"
#include "SymbolsTable/SymbolsTable.h"
#include "RuleClasses/RuleClasses.h"

CTypeCheckerVisitor::CTypeCheckerVisitor( SymbolsTable::CTable* table )
{
	symbolsTable = table;
}

void CTypeCheckerVisitor::Visit( const CProgram* program )
{
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CTypeCheckerVisitor::Visit( const CMainClass* mainClass )
{
	curClass = symbolsTable->GetClass( mainClass->GetClassName()->GetString() );
	curMethod = curClass->GetMethod( "main" );
	
	if( mainClass->GetStatement() != nullptr ) {
		mainClass->GetStatement()->Accept( this );
	}

	curMethod = nullptr;
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

	std::string lastType = lastTypeValueStack.back();
	lastTypeValueStack.pop_back();
	if( !isPODType( lastType ) && symbolsTable->GetClass( lastType ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CVarDecl. " ) +
			lastType + " " + varDecl->GetName( )->GetString( ) +
			" - incorrect variable declaration, class " + lastType + " not found. " +
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
	curMethod = curClass->GetMethod( methodDecl->GetName()->GetString() );
	methodDecl->GetType()->Accept( this );

	std::string lastTypeValue = lastTypeValueStack.back( );
	lastTypeValueStack.pop_back();
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
		lastTypeValueStack.push_back( ".INT" );
		break;
	case CStandardType::StandardType::BOOL:
		lastTypeValueStack.push_back( ".BOOL" );
		break;
	case CStandardType::StandardType::INT_ARRAY:
		lastTypeValueStack.push_back( ".INT_ARRAY" );
		break;
	default:
		lastTypeValueStack.push_back( ".INT" );
		break;
	}
}

void CTypeCheckerVisitor::Visit( const CUserType* type )
{
	lastTypeValueStack.push_back( type->GetTypeName()->GetString() );
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
	std::string leftType = lastTypeValueStack.back();
	lastTypeValueStack.pop_back();
	statement->GetRightPart()->Accept( this );
	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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
	std::string leftType = lastTypeValueStack.back();
	lastTypeValueStack.pop_back();
	statement->GetRightPart()->Accept( this );
	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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
		lastTypeValueStack.push_back( ".BOOL" );
		return;
	}

	expr->GetLeftExp()->Accept( this );
	auto left = lastTypeValueStack.back();
	lastTypeValueStack.pop_back();
	expr->GetRightExp()->Accept( this );
	auto right = lastTypeValueStack.back();
	lastTypeValueStack.pop_back();

	if( expr->GetBinOp() == CBinOpExpression::MINUS || expr->GetBinOp() == CBinOpExpression::PLUS ||
		expr->GetBinOp() == CBinOpExpression::TIMES || expr->GetBinOp() == CBinOpExpression::DIVIDE ) {
		if( left != ".INT" || right != ".INT" ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CBinOpExpression. " ) +
				"Expression of type " + (left != ".INT" ? left : right) + " cannot be used in arithmetic operations, use 'int'. " +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
		}
		lastTypeValueStack.push_back( ".INT" );
	}
	if( expr->GetBinOp() == CBinOpExpression::AND ) {
		if( left != ".BOOL" || right != ".BOOL" ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CBinOpExpression. " ) +
				"Expression of type " + (left != ".BOOL" ? left : right) + " cannot be used in logic operations, use 'bool'. " +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
		}
		lastTypeValueStack.push_back( ".BOOL" );
	}
}

void CTypeCheckerVisitor::Visit( const CIndexExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValueStack.push_back( ".INT" );
		return;
	}

	expr->GetExp()->Accept( this );
	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	if( lastTypeValue != ".INT_ARRAY" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CIndexExpression. " ) +
			"Cannot get element of not variable which is not array. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	expr->GetIndexExp()->Accept( this );
	lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
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
		lastTypeValueStack.push_back( ".INT" );
		return;
	}

	expr->GetExp()->Accept( this );

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	if( lastTypeValue != ".INT_ARRAY" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CLengthExpression. " ) +
			"Cannot get length of not array expression. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValueStack.push_back( ".INT" );
}

void CTypeCheckerVisitor::Visit( const CMethodExpression* expr )
{
	expr->GetExp()->Accept( this );
	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	SymbolsTable::CClassInfo* usedClass = symbolsTable->GetClass( lastTypeValue );
	if( usedClass == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CMethodExpression. " ) +
			"Cannot find such class " + lastTypeValue +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	SymbolsTable::CMethodInfo* usedMethod = usedClass->GetMethod( expr->GetIdentifier()->GetString() );
	int typeValuePointer = lastTypeValueStack.size();
	expr->GetExpList()->Accept( this );
	auto params = usedMethod->GetParams();
	if( lastTypeValueStack.size() - typeValuePointer != params.size() ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CMethodExpression. " ) +
			"Invalid number of arguments " + usedMethod->GetName() +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}
	for( int i = typeValuePointer; i < lastTypeValueStack.size(); ++i ) {
		if( params[i - typeValuePointer]->GetName() != lastTypeValueStack[i] ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CMethodExpression. " ) +
				"Wrong function argument type " + params[i - typeValuePointer]->GetName() +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
		}
	}

	for( int i = 0, size = lastTypeValueStack.size() - typeValuePointer; i < size; ++i ) {
		lastTypeValueStack.pop_back();
	}
	lastTypeValueStack.push_back( usedMethod->GetReturnType()->GetName() );
}

void CTypeCheckerVisitor::Visit( const CIntLiteralExpression* expr )
{
	lastTypeValueStack.push_back( ".INT" );
}

void CTypeCheckerVisitor::Visit( const CBoolLiteralExpression* expr )
{
	lastTypeValueStack.push_back( ".BOOL" );
}

void CTypeCheckerVisitor::Visit( const CIdentifierExpression* expr )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValueStack.push_back( ".INT" );
		return;
	}

	if( curMethod == nullptr ) { // если находимся внутри класса, но снаружи всех методов
		auto varInfo = curClass->GetVar( expr->GetIdentifier()->GetString() );
		if( varInfo == nullptr ) {
			errorStorage.PutError( std::string( "[Type check] Node type - CIdentifierExpression. " ) +
				"Undeclared identifier " + expr->GetIdentifier()->GetString() + ". " +
				"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
			lastTypeValueStack.push_back( ".INT" );
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
			lastTypeValueStack.push_back( ".INT" );
			return;
		}
	}
	varInfo->GetType()->Accept( this );
}

void CTypeCheckerVisitor::Visit( const CThisExpression* expr )
{
	if( curClass == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValueStack.push_back( curClass->GetName( ) );
		return;
	}
	lastTypeValueStack.push_back( curClass->GetName() );
}

void CTypeCheckerVisitor::Visit( const CNewIntArrayExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		lastTypeValueStack.push_back( ".INT_ARRAY" );
		return;
	}

	expr->GetExp()->Accept( this );

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	if( lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CNewIntArrayExpression. " ) +
			"Cannot create array of non integer length. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValueStack.push_back( ".INT_ARRAY" );
}

void CTypeCheckerVisitor::Visit( const CNewExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition().ToString() );
		if( expr->GetIdentifier() != nullptr ) {
			lastTypeValueStack.push_back( expr->GetIdentifier()->GetString() );
		} else {
			lastTypeValueStack.push_back( ".INT" );
		}
		return;
	}

	if( symbolsTable->GetClass( expr->GetIdentifier()->GetString() ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CNewExpression. " ) +
			"Undefined class name - " + expr->GetIdentifier()->GetString() + ". " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValueStack.push_back( expr->GetIdentifier()->GetString() );
}

void CTypeCheckerVisitor::Visit( const CUnaryOpExpression* expr )
{
	if( curClass == nullptr || curMethod == nullptr ) {
		errorStorage.PutError( "Expression out of scope " + expr->GetPosition( ).ToString( ) );
		lastTypeValueStack.push_back( ".INT" );
		return;
	}
	
	expr->GetLeftExp()->Accept( this );

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	if( expr->GetOperation() == CUnaryOpExpression::MINUS && lastTypeValue != ".INT" ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CUnaryOpExpression. " ) +
			"Expression of type " + lastTypeValue + " cannot be used in arithmetic operations, use 'int'. " +
			"Line " + std::to_string( expr->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( expr->GetPosition().GetBeginPos().second ) + "." );
	}

	lastTypeValueStack.push_back( ".INT" );
}

void CTypeCheckerVisitor::Visit( const CBracesExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
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

void CTypeCheckerVisitor::Visit( const CFormalParam* param )
{
	if( param->GetType() != nullptr ) {
		param->GetType()->Accept( this );
	}

	std::string lastTypeValue = lastTypeValueStack.back();
	lastTypeValueStack.back();
	if( !isPODType( lastTypeValue ) && symbolsTable->GetClass( lastTypeValue ) == nullptr ) {
		errorStorage.PutError( std::string( "[Type check] Node type - CFormalList. " ) +
			lastTypeValue + " " + param->GetIdentifier()->GetString() +
			" - wrong argument type in method declaration, class " + lastTypeValue + " not found. " +
			"Line " + std::to_string( param->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( param->GetPosition().GetBeginPos().second ) + "." );
	}
}

void CTypeCheckerVisitor::Visit( const CFormalList* list )
{
	auto params = list->GetParamList();
	for( int i = params.size() - 1; i >= 0; --i ) {
		params[i].get()->Accept( this );
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