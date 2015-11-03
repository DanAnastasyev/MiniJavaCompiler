#include "Visitor.h"
#include "SymbolsTable/SymbolsTable.h"
#include "RuleClasses/RuleClasses.h"

class CSymbolTableBuilderVisitor : public IVisitor {
public:
	void Visit( const CProgram* program ) override;
	void Visit( const CMainClass* program ) override;
	void Visit( const CClassDeclList* program ) override;
	void Visit( const CClassDecl* program ) override;
	void Visit( const CClassDeclDerived* program ) override;
	void Visit( const CVarDecl* program ) override;
	void Visit( const CVarDeclList* program ) override;
	void Visit( const CFormalList* list ) override;
	void Visit( const CFormalRestList* list ) override;
	void Visit( const CMethodDecl* program ) override;
	void Visit( const CMethodDeclList* methodList ) override;
	void Visit( const CStandardType* program ) override;
	void Visit( const CUserType* program ) override;
	void Visit( const CStatementListStatement* statement ) override {}
	void Visit( const CArrayAssignStatement* statement ) override {}
	void Visit( const CAssignStatement* statement ) override {}
	void Visit( const CIfStatement* statement ) override {}
	void Visit( const CWhileStatement* statement ) override {}
	void Visit( const CPrintStatement* statement ) override {}
	void Visit( const CBinOpExpression* expr ) override {}
	void Visit( const CIndexExpression* expr ) override {}
	void Visit( const CLenghtExpression* expr ) override {}
	void Visit( const CMethodExpression* expr ) override {}
	void Visit( const CIntLiteralExpression* expr ) override {}
	void Visit( const CBoolLiteralExpression* expr ) override {}
	void Visit( const CIdentifierExpression* expr ) override {}
	void Visit( const CThisExpression* expr ) override {}
	void Visit( const CNewIntArrayExpression* expr ) override {}
	void Visit( const CNewExpression* expr ) override {}
	void Visit( const CUnaryOpExpression* expr ) override {}
	void Visit( const CBracesExpression* expr ) override {}
	void Visit( const CExpressionList* exprList ) override {}
	void Visit( const CStatementList* stmtList ) override {}
	void Visit( const CExpressionRest* expr ) override {}

private:
	SymbolsTable::CClassInfo* curClass;
	SymbolsTable::CMethodInfo* curMethod;
	SymbolsTable::CTable* symbolsTable;
	std::shared_ptr<IType> lastTypeValue;

	bool isDebug = true;
};