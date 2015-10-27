#pragma once
#include <iostream>

#include "Visitor.h"
#include "RuleClasses.h"

class CPrettyPrinterVisitor : public IVisitor {
	void Visit( CProgram* program )
	{
		if( program->GetMainClass() != nullptr ) {
			program->GetMainClass()->Accept( this );
		}
		if( program->GetClassDeclList() != nullptr ) {
			program->GetClassDeclList()->Accept( this );
		}
	}

	void Visit( CMainClass* program )
	{
		std::cout << "class ";
		std::cout << program->GetClassName();
		std::cout << " { \n";
		std::cout << "public static void main(String[] ";
		std::cout << program->GetMainArgsIdentifier();
		std::cout << ") {\n";
		if( program->GetStatement() != nullptr ) {
			program->GetStatement()->Accept( this );
		}
		std::cout << "\n}\n}\n";
	}

	void Visit( const CClassDeclList* program )
	{
		if( program->GetClassDecl() != nullptr ) {
			program->GetClassDecl()->Accept( this );
		}
		if( program->GetClassDeclList() != nullptr ) {
			program->GetClassDeclList()->Accept( this );
		}
	}
	void Visit( const CClassDecl* program )
	{
		std::cout << "class " << program->GetName() << " {\n";
		if( program->GetVarDeclList() != nullptr ) {
			program->GetVarDeclList()->Accept( this );
		}
		if( program->GetMethodDeclList() != nullptr ) {
			program->GetMethodDeclList()->Accept( this );
		}
		std::cout << "}";
	}

	void Visit( const CClassDeclDerived* program )
	{
		std::cout << "class " << program->GetName();
		std::cout << " extends " << program->GetBaseClassName() << " {\n";
		if( program->GetVarDeclList() != nullptr ) {
			program->GetVarDeclList()->Accept( this );
		}
		if( program->GetMethodDeclList() != nullptr ) {
			program->GetMethodDeclList()->Accept( this );
		}
		std::cout << "}";
	}
	void Visit( const CVarDecl* program )
	{
		std::cout << program->GetType() << " ";
		std::cout << program->GetName() << ";";
	}

	void Visit( const CVarDeclList* program )
	{
		program->GetVarDeclList()->Accept( this );
		program->GetVarDecl()->Accept( this );
	}

	void Visit( const CMethodDecl* program )
	{
		std::cout << "public ";
		program->GetType()->Accept( this );
	}

	void Visit( const CStandardType* program )
	{
		switch( program->GetType() ) {
			case CStandardType::StandardType::INT: std::cout << "int\n";
			case CStandardType::StandardType::INT_ARRAY: std::cout << "int[]\n";
			case CStandardType::StandardType::BOOL: std::cout << "bool\n";
		}
	}

	void Visit( const CUserType* program ) 
	{
		std::cout << program->GetTypeName() << std::endl;
	}

	void Visit( const CStatementListStatement* statement )
	{
		statement->GetStatementList()->Accept( this );
	}

	void Visit( const CArrayAssignStatement* statement ) 
	{
		std::cout << statement->GetArrayName() << "[";
		statement->GetElementNumber()->Accept( this );
		std::cout << "] = ";
		statement->GetRightPart()->Accept( this );
	}

	void Visit( const CAssignStatement* statement ) 
	{
		std::cout << statement->GetLeftPart() << " = ";
		statement->GetRightPart()->Accept( this );
	}
	
	void Visit( const CIfStatement* statement ) 
	{
		std::cout << "if( ";
		statement->GetCondition()->Accept( this );
		std::cout << " )" << std::endl;
		statement->GetIfTrueStatement()->Accept( this );
		std::cout << std::endl << "else" << std::endl;
		statement->GetIfFalseStatement()->Accept( this );
	}

	void Visit( const CWhileStatement* statement ) 
	{
		std::cout << "while( ";
		statement->GetCondition()->Accept( this );
		std::cout << " )" << std::endl;
		statement->GetBodyCycle()->Accept( this );
	}

	void Visit( const CPrintStatement* statement ) 
	{
		std::cout << "println( ";
		statement->GetExpression()->Accept( this );
		std::cout << " );";
	}

	void Visit( const CBinOpExpression* expr ) 
	{
		expr->GetLeftExp()->Accept( this );
		switch( expr->GetBinOp( ) ) {
			case CBinOpExpression::BinOp::AND: std::cout << "&&";
			case CBinOpExpression::BinOp::LESS: std::cout << "<";
			case CBinOpExpression::BinOp::PLUS: std::cout << "+";
			case CBinOpExpression::BinOp::MINUS: std::cout << "-";
			case CBinOpExpression::BinOp::TIMES: std::cout << "*";
			case CBinOpExpression::BinOp::DIVIDE: std::cout << "/";
		}
		expr->GetRightExp()->Accept( this );
	}

	void Visit( const CIndexExpression* expr ) 
	{
		expr->GetExp()->Accept( this );
		std::cout << "[";
		expr->GetIndexExp()->Accept( this );
		std::cout << "]";
	}

	void Visit( const CLenghtExpression* expr )
	{
		expr->GetExp()->Accept( this );
		std::cout << ".length";
	}

	void Visit( const CMethodExpression* expr ) 
	{
		expr->GetExp()->Accept( this );
		std::cout << "." << expr->GetIdentifier() << "(";
		expr->GetIndexExp()->Accept( this );
		std::cout << ")";
	}

	void Visit( const CIntLiteralExpression* expr ) 
	{
		std::cout << expr->GetValue();
	}

	void Visit( const CBoolLiteralExpression* expr ) 
	{
		std::cout << expr->GetValue();
	}

	void Visit( const CIdentifierExpression* expr ) 
	{
		std::cout << expr->GetIdentifier();
	}

	void Visit( const CThisExpression* expr ) 
	{
		std::cout << "this ";
	}

	void Visit( const CNewIntArrayExpression* expr )
	{
		std::cout << "new int [";
		expr->GetExp()->Accept( this );
		std::cout << "]";
	}

	void Visit( const CNewExpression* expr ) 
	{
		std::cout << "new " << expr->GetIdentifier() << "()";
	}

	void Visit( const CUnaryOpExpression* expr ) 
	{
		switch( expr->GetOperation() ) {
			case CUnaryOpExpression::UnaryOp::MINUS: std::cout << "-";
			case CUnaryOpExpression::UnaryOp::NOT: std::cout << "!";
		}
		expr->GetLeftExp()->Accept( this );
	}

	void Visit( const CBracesExpression* expr )
	{
		std::cout << "( ";
		expr->GetExp()->Accept( this );
		std::cout << " )";
	}

	void Visit( const CExpressionList* exprList ) 
	{
		exprList->GetExp()->Accept( this );
		exprList->GetExpList()->Accept( this );
	}

	void Visit( const CStatementList* stmtList ) 
	{
		stmtList->GetStatement()->Accept( this );
		stmtList->GetStatementList()->Accept( this );
	}
	
	void Visit( const CExpressionRest* expr ) 
	{
		expr->GetExp()->Accept( this );
	}

	void Visit( const CMethodDeclList* methodList ) 
	{
	
	}
};