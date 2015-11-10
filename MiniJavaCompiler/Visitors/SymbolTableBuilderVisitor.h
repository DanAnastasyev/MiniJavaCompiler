#include "Visitor.h"
#include "SymbolsTable/SymbolsTable.h"
#include "RuleClasses/RuleClasses.h"

class CSymbolTableBuilderVisitor : public IVisitor {
public:
	CSymbolTableBuilderVisitor();

	SymbolsTable::CTable* GetSymbolsTable();

	void Visit( const CProgram* program ) override;
	void Visit( const CMainClass* program ) override;
	void Visit( const CClassDeclList* program ) override;
	void Visit( const CClassDecl* program ) override;
	void Visit( const CClassDeclDerived* program ) override;
	void Visit( const CVarDecl* program ) override;
	void Visit( const CVarDeclList* program ) override;
	void Visit( const CFormalList* list ) override;
	void Visit( const CFormalParam* param ) override;
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

	const CErrorStorage& GetErrorStorage() const;

private:
	SymbolsTable::CClassInfo* curClass = nullptr;
	SymbolsTable::CMethodInfo* curMethod = nullptr;
	std::shared_ptr<SymbolsTable::CTable> symbolsTable;
	IType* lastTypeValue = nullptr;

	bool isDebug = true;

	CErrorStorage errorStorage;
};