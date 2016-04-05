#pragma once
#include "Visitor.h"

#include "IRTree\Expressions.h"
#include "IRTree\Statements.h"
#include "Frame\Frame.h"

#include <stack>

class CIRBuilderVisitor : public IVisitor {
public:
	CIRBuilderVisitor( SymbolsTable::CTable* table );

	void Visit( const CProgram* program ) override;
	void Visit( const CMainClass* mainClass ) override;
	void Visit( const CClassDeclList* classDeclList ) override;
	void Visit( const CClassDecl* classDecl ) override;
	void Visit( const CClassDeclDerived* classDeclDerived ) override;
	void Visit( const CVarDecl* varDecl ) override;
	void Visit( const CVarDeclList* varDeclList ) override;
	void Visit( const CMethodDecl* methodDecl ) override;
	void Visit( const CStandardType* type ) override;
	void Visit( const CUserType* type ) override;
	void Visit( const CStatementListStatement* statement ) override;
	void Visit( const CArrayAssignStatement* statement ) override;
	void Visit( const CAssignStatement* statement ) override;
	void Visit( const CIfStatement* statement ) override;
	void Visit( const CWhileStatement* statement ) override;
	void Visit( const CPrintStatement* statement ) override;
	void Visit( const CBinOpExpression* expr ) override;
	void Visit( const CIndexExpression* expr ) override;
	void Visit( const CLenghtExpression* expr ) override;
	void Visit( const CMethodExpression* expr ) override;
	void Visit( const CIntLiteralExpression* expr ) override;
	void Visit( const CBoolLiteralExpression* expr ) override;
	void Visit( const CIdentifierExpression* expr ) override;
	void Visit( const CThisExpression* expr ) override;
	void Visit( const CNewIntArrayExpression* expr ) override;
	void Visit( const CNewExpression* expr ) override;
	void Visit( const CUnaryOpExpression* expr ) override;
	void Visit( const CBracesExpression* expr ) override;
	void Visit( const CExpressionList* exprList ) override;
	void Visit( const CStatementList* stmtList ) override;
	void Visit( const CExpressionRest* expr ) override;
	void Visit( const CMethodDeclList* methodList ) override;
	void Visit( const CFormalList* list ) override;
	void Visit( const CFormalParam* list ) override;

	std::vector<Frame::CFrame> GetFrames() const;
private:
	std::shared_ptr<const IRTree::CExprList> convertVectorToExprList( const std::vector<std::shared_ptr<const IRTree::IExpr>>& args ) const;

	std::stack<std::shared_ptr<const IRTree::IExpr>> parsedExpressions;
	std::stack<std::shared_ptr<const IRTree::IStm>> parsedStatements;

	std::vector<Frame::CFrame> frames; // Храним вектор фреймов

	SymbolsTable::CTable* symbolsTable;
	SymbolsTable::CClassInfo* curClass = nullptr;
	SymbolsTable::CMethodInfo* curMethod = nullptr;
};