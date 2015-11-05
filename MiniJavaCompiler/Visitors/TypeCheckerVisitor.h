#include <memory>

#include "Visitors/Visitor.h"

namespace SymbolsTable
{
	class CClassInfo;
	class CMethodInfo;
	class CTable;
}

class IType;
class CTypeCheckerVisitor : public IVisitor {
public:
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
	void Visit( const CFormalRestList* list ) override;

	const CErrorStorage& GetErrorStorage() const;

private:
	bool isPODType( const std::string& type ) const;

	SymbolsTable::CClassInfo* curClass = nullptr;
	SymbolsTable::CMethodInfo* curMethod = nullptr;
	SymbolsTable::CTable* symbolsTable = nullptr;
	std::string lastTypeValue;

	CErrorStorage errorStorage;
};