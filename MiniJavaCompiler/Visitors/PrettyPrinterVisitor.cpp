#include <iostream>

#include "PrettyPrinterVisitor.h"

void CPrettyPrinterVisitor::Visit( const CProgram* program )
{
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CMainClass* program )
{
	std::cout << "class ";
	std::cout << program->GetClassName()->GetString();
	std::cout << " { \n";
	std::cout << "public static void main(String[] ";
	std::cout << program->GetMainArgsIdentifier()->GetString();
	std::cout << ") {\n";
	if( program->GetStatement() != nullptr ) {
		program->GetStatement()->Accept( this );
	}
	std::cout << "\n}\n}\n";
}

void CPrettyPrinterVisitor::Visit( const CClassDeclList* program )
{
	if( program->GetClassDecl() != nullptr ) {
		program->GetClassDecl()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CClassDecl* program )
{
	std::cout << "class " << program->GetName()->GetString() << " {\n";
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}
	std::cout << "}\n";
}

void CPrettyPrinterVisitor::Visit( const CClassDeclDerived* program )
{
	std::cout << "class " << program->GetName()->GetString();
	std::cout << " extends " << program->GetBaseClassName()->GetString() << " {\n";
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}
	std::cout << "}\n";
}

void CPrettyPrinterVisitor::Visit( const CVarDecl* program )
{
	program->GetType()->Accept( this );
	std::cout << program->GetName()->GetString() << ";\n";
}

void CPrettyPrinterVisitor::Visit( const CVarDeclList* program )
{
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetVarDecl() != nullptr ) {
		program->GetVarDecl()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CMethodDecl* program )
{
	std::cout << "public ";
	if( program->GetType() != nullptr ) {
		program->GetType()->Accept( this );
	}
	std::cout << program->GetName()->GetString() << "( ";
	if( program->GetFormalList() != nullptr ) {
		program->GetFormalList()->Accept( this );
	}
	std::cout << " )\n{\n";
	if( program->GetVarList() != nullptr ) {
		program->GetVarList()->Accept( this );
	}
	if( program->GetStatementList() != nullptr ) {
		program->GetStatementList()->Accept( this );
	}
	std::cout << "return ";
	if( program->GetReturnExpression() != nullptr ) {
		program->GetReturnExpression()->Accept( this );
	}
	std::cout << ";\n}\n";
}

void CPrettyPrinterVisitor::Visit( const CStandardType* program )
{
	switch( program->GetType() ) {
	case CStandardType::StandardType::INT:
		std::cout << "int ";
		break;
	case CStandardType::StandardType::INT_ARRAY:
		std::cout << "int[] ";
		break;
	case CStandardType::StandardType::BOOL:
		std::cout << "bool ";
		break;
	}
}

void CPrettyPrinterVisitor::Visit( const CUserType* program )
{
	std::cout << program->GetTypeName()->GetString() << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CStatementListStatement* statement )
{
	std::cout << "{" << std::endl;
	if( statement->GetStatementList() != nullptr ) {
		statement->GetStatementList()->Accept( this );
	}
	std::cout << std::endl << "}" << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CArrayAssignStatement* statement )
{
	std::cout << statement->GetArrayName()->GetString() << "[";
	if( statement->GetElementNumber() != nullptr ) {
		statement->GetElementNumber()->Accept( this );
	}
	std::cout << "] = ";
	if( statement->GetRightPart() != nullptr ) {
		statement->GetRightPart()->Accept( this );
	}
	std::cout << ";" << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CFormalParam* param )
{
	if( param->GetType( ) != nullptr ) {
		param->GetType( )->Accept( this );
	}
	std::cout << " " + param->GetIdentifier()->GetString( ) << " ";
}

void CPrettyPrinterVisitor::Visit( const CFormalList* list )
{
	auto params = list->GetParamList();
	for( int i = params.size() - 1; i > 0; --i ) {
		params[i].get()->Accept( this );
		std::cout << ", ";
	}
	params[0].get()->Accept( this );
}

void CPrettyPrinterVisitor::Visit( const CMethodDeclList* methodList )
{
	if( methodList->GetMethodDecl() != nullptr ) {
		methodList->GetMethodDecl()->Accept( this );
	}
	if( methodList->GetMethodDeclList() != nullptr ) {
		methodList->GetMethodDeclList()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CExpressionRest* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CAssignStatement* statement )
{
	std::cout << statement->GetLeftPart()->GetString() << " = ";
	if( statement->GetRightPart() != nullptr ) {
		statement->GetRightPart()->Accept( this );
	}
	std::cout << ";" << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CIfStatement* statement )
{
	std::cout << "if( ";
	if( statement->GetCondition() != nullptr ) {
		statement->GetCondition()->Accept( this );
	}
	std::cout << " )" << std::endl;
	if( statement->GetIfTrueStatement() != nullptr ) {
		statement->GetIfTrueStatement()->Accept( this );
	}
	if( statement->GetIfFalseStatement() != nullptr ) {
		std::cout << std::endl << "else" << std::endl;
		statement->GetIfFalseStatement()->Accept( this );
	}
	std::cout << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CWhileStatement* statement )
{
	std::cout << "while( ";
	if( statement->GetCondition() != nullptr ) {
		statement->GetCondition()->Accept( this );
	}
	std::cout << " )" << std::endl;
	if( statement->GetBodyCycle() != nullptr ) {
		statement->GetBodyCycle()->Accept( this );
	}
	std::cout << std::endl;
}

void CPrettyPrinterVisitor::Visit( const CPrintStatement* statement )
{
	std::cout << "System.out.println( ";
	if( statement->GetExpression() != nullptr ) {
		statement->GetExpression()->Accept( this );
	}
	std::cout << " );\n";
}

void CPrettyPrinterVisitor::Visit( const CBinOpExpression* expr )
{
	if( expr->GetLeftExp() != nullptr ) {
		expr->GetLeftExp()->Accept( this );
	}
	switch( expr->GetBinOp() ) {
	case CBinOpExpression::BinOp::AND:
		std::cout << " && ";
		break;
	case CBinOpExpression::BinOp::LESS:
		std::cout << " < ";
		break;
	case CBinOpExpression::BinOp::PLUS:
		std::cout << " + ";
		break;
	case CBinOpExpression::BinOp::MINUS:
		std::cout << " - ";
		break;
	case CBinOpExpression::BinOp::TIMES:
		std::cout << " * ";
		break;
	case CBinOpExpression::BinOp::DIVIDE:
		std::cout << " / ";
		break;
	}
	if( expr->GetRightExp() != nullptr ) {
		expr->GetRightExp()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CIndexExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
	if( expr->GetIndexExp() != nullptr ) {
		std::cout << "[";
		expr->GetIndexExp()->Accept( this );
		std::cout << "]";
	}
}

void CPrettyPrinterVisitor::Visit( const CLenghtExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
		std::cout << ".length";
	}
}

void CPrettyPrinterVisitor::Visit( const CMethodExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		expr->GetExp()->Accept( this );
	}
	std::cout << "." << expr->GetIdentifier()->GetString() << "(";
	if( expr->GetExpList() != nullptr ) {
		expr->GetExpList()->Accept( this );
	}
	std::cout << ")";
}

void CPrettyPrinterVisitor::Visit( const CIntLiteralExpression* expr )
{
	std::cout << expr->GetValue()->GetString();
}

void CPrettyPrinterVisitor::Visit( const CBoolLiteralExpression* expr )
{
	std::cout << expr->GetValue()->GetString();
}

void CPrettyPrinterVisitor::Visit( const CIdentifierExpression* expr )
{
	std::cout << expr->GetIdentifier()->GetString();
}

void CPrettyPrinterVisitor::Visit( const CThisExpression* expr )
{
	std::cout << "this ";
}

void CPrettyPrinterVisitor::Visit( const CNewIntArrayExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		std::cout << "new int [";
		expr->GetExp()->Accept( this );
		std::cout << "]";
	}
}

void CPrettyPrinterVisitor::Visit( const CNewExpression* expr )
{
	std::cout << "new " << expr->GetIdentifier()->GetString() << "()";
}

void CPrettyPrinterVisitor::Visit( const CUnaryOpExpression* expr )
{
	switch( expr->GetOperation() ) {
	case CUnaryOpExpression::UnaryOp::MINUS:
		std::cout << "-";
		break;
	case CUnaryOpExpression::UnaryOp::NOT:
		std::cout << "!";
		break;
	}
	if( expr->GetLeftExp() != nullptr ) {
		expr->GetLeftExp()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CBracesExpression* expr )
{
	if( expr->GetExp() != nullptr ) {
		std::cout << "( ";
		expr->GetExp()->Accept( this );
		std::cout << " )";
	}
}

void CPrettyPrinterVisitor::Visit( const CExpressionList* exprList )
{
	if( exprList->GetExp() != nullptr ) {
		exprList->GetExp()->Accept( this );
	}
	if( exprList->GetExpList() != nullptr ) {
		exprList->GetExpList()->Accept( this );
	}
}

void CPrettyPrinterVisitor::Visit( const CStatementList* stmtList )
{
	if( stmtList->GetStatement() != nullptr ) {
		stmtList->GetStatement()->Accept( this );
	}
	if( stmtList->GetStatementList() != nullptr ) {
		stmtList->GetStatementList()->Accept( this );
	}
}
