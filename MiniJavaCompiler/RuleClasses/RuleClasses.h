#pragma once
#include <string>
#include <memory>
#include <vector>

#include "grammar.h"
#include "Position.h"
#include "Visitors/Visitor.h"
#include "SymbolsTable/Symbol.h"
#include "PositionStorage.h"

class CProgram : public IProgram, public CPositionStorage {
public:
	CProgram( IMainClass* _mainClass, IClassDeclList* _classList, CPosition& position );

	void Accept( IVisitor* visitor ) const override;

	const IMainClass* GetMainClass() const;
	const IClassDeclList* GetClassDeclList() const;

private:
	const std::shared_ptr<IMainClass> mainClass;
	const std::shared_ptr<IClassDeclList> classList;
};

class CMainClass : public IMainClass, public CPositionStorage {
public:
	CMainClass( const std::string& _identifier, const std::string& _arsId, IStatement* _statement, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IStatement* GetStatement() const;
	const CSymbol* GetClassName() const;
	const CSymbol* GetMainArgsIdentifier() const;

private:
	const CSymbol* identifier;
	const CSymbol* argsIdentifier;
	const std::shared_ptr<IStatement> statement;
}; 

class CClassDeclList : public IClassDeclList, public CPositionStorage {
public:
	CClassDeclList( IClassDecl* _declaration, IClassDeclList* _list, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;
	
	const IClassDeclList* GetClassDeclList() const;
	const IClassDecl* GetClassDecl() const;

private:
	const std::shared_ptr<IClassDeclList> list;
	const std::shared_ptr<IClassDecl> declaration;
};

class CClassDecl : public IClassDecl, public CPositionStorage {
public:
	CClassDecl( IVarDeclList* _varList, IMethodDeclList* _methodList, const std::string& _className, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const CSymbol* GetName() const;
	const IVarDeclList* GetVarDeclList() const;
	const IMethodDeclList* GetMethodDeclList( ) const;

private:
	const CSymbol* className;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CClassDeclDerived : public IClassDecl, public CPositionStorage {
public:
	CClassDeclDerived(
		IVarDeclList* _varList,
		IMethodDeclList* _methodList,
		const std::string& _className,
		const std::string& _baseClassName,
		const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const CSymbol* GetName() const;
	const CSymbol* GetBaseClassName() const;
	const IVarDeclList* GetVarDeclList() const;
	const IMethodDeclList* GetMethodDeclList() const;

private:
	const CSymbol* className;
	const CSymbol* baseClassName;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CStatementList : public IStatementList, public CPositionStorage { // not implemented
public:
	CStatementList( IStatement* _stmt, IStatementList* _stmtList, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IStatement* GetStatement() const;
	const IStatementList* GetStatementList() const;

private:
	const std::shared_ptr<IStatement> stmt;
	const std::shared_ptr<IStatementList> stmtList;
};

class CVarDecl : public IVarDecl, public CPositionStorage {
public:
	CVarDecl( IType* _type, const std::string& _identifier, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IType* GetType() const;
	const CSymbol* GetName() const;

private:
	const std::shared_ptr<IType> type;
	const CSymbol* identifier;
};

class CVarDeclList : public IVarDeclList, public CPositionStorage {
public:
	CVarDeclList( IVarDeclList* _varDeclList, IVarDecl* _varDecl, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IVarDeclList* GetVarDeclList() const;
	const IVarDecl* GetVarDecl() const;

private:
	const std::shared_ptr<IVarDeclList> varDeclList;
	const std::shared_ptr<IVarDecl> varDecl;
};

class CMethodDecl : public IMethodDecl, public CPositionStorage {
public:
	CMethodDecl(
		IType* _type,
		const std::string& _methodName,
		IFormalList* _formalList,
		IVarDeclList* _varList,
		IStatementList* _statementList,
		IExp* _returnExpr,
		const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IType* GetType( ) const;
	const CSymbol* GetName() const;
	const IFormalList* GetFormalList() const;
	const IVarDeclList* GetVarList() const;
	const IStatementList* GetStatementList() const;
	const IExp* GetReturnExpression() const;

private:
	const std::shared_ptr<IType> type;
	const CSymbol* methodName;
	const std::shared_ptr<IFormalList> formalList;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IStatementList> statementList;
	const std::shared_ptr<IExp> returnExpr;
};

class CMethodDeclList : public IMethodDeclList, public CPositionStorage {
public:
	CMethodDeclList( IMethodDecl* _methodDecl, IMethodDeclList* _methodDeclList, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IMethodDeclList* GetMethodDeclList() const;
	const IMethodDecl* GetMethodDecl() const;

private:
	const std::shared_ptr<IMethodDeclList> methodDeclList;
	const std::shared_ptr<IMethodDecl> methodDecl;
};

class CFormalList : public IFormalList, public CPositionStorage
{
public:
	CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IFormalList* GetFormalRest() const;
	const IType* GetType() const;
	const CSymbol* GetIdentifier() const;

private:
	std::shared_ptr<IType> type;
	const CSymbol* identifier;
	std::shared_ptr<IFormalList> formalList;
};

class CFormalRestList : public IFormalList, public CPositionStorage {
public:
	CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IFormalList* GetFormalRest() const;
	const IFormalList* GetFormalRestList() const;

private:
	std::shared_ptr<IFormalList> formalRest;
	std::shared_ptr<IFormalList> formalRestList;
};

class CStatementListStatement : public IStatement, public CPositionStorage {
public:
	CStatementListStatement( IStatementList* _statementList, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IStatementList* GetStatementList() const;

private:
	const std::shared_ptr<IStatementList> statementList;
};

class CIfStatement : public IStatement, public CPositionStorage {
public:
	CIfStatement( IExp* _condition, IStatement* _statementIfTrue, IStatement* _statementIfFalse, const CPosition _position );

	void Accept( IVisitor* visitor ) const override;

	const IExp* GetCondition() const;
	const IStatement* GetIfTrueStatement() const;
	const IStatement* GetIfFalseStatement() const;

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> statementIfTrue;
	const std::shared_ptr<IStatement> statementIfFalse;
};

class CWhileStatement : public IStatement, public CPositionStorage {
public:
	CWhileStatement( IExp* _condition, IStatement* _cycleBody, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IExp* GetCondition() const;
	const IStatement* GetBodyCycle() const;

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> cycleBody;
};

class CPrintStatement : public IStatement, public CPositionStorage {
public:
	CPrintStatement( IExp* _expression, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const IExp* GetExpression() const;

private:
	const std::shared_ptr<IExp> expression;
};

class CAssignStatement : public IStatement, public CPositionStorage {
public:
	CAssignStatement( const std::string& _left, IExp* _right, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const CSymbol* GetLeftPart() const;
	const IExp* GetRightPart() const;

private:
	const CSymbol* left;
	const std::shared_ptr<IExp> right;
};

class CArrayAssignStatement : public IStatement, public CPositionStorage {
public:
	CArrayAssignStatement( const std::string& _arrayId, IExp* _elementNumber, IExp* _rightPart, const CPosition& _position );

	void Accept( IVisitor* visitor ) const override;

	const CSymbol* GetArrayName() const;
	const IExp* GetElementNumber() const;
	const IExp* GetRightPart() const;

private:
	const CSymbol* arrayId;
	const std::shared_ptr<IExp> elementNumber;
	const std::shared_ptr<IExp> rightPart;
};

class CStandardType : public IType, public CPositionStorage {
public:
	enum StandardType { INT = 0, INT_ARRAY, BOOL };

	CStandardType( StandardType _type, const CPosition& _position );
	CStandardType( const CStandardType* other );

	void Accept( IVisitor* visitor ) const override;

	StandardType GetType() const;
	bool IsPODType() const override;

private:
	StandardType type;
};

class CUserType : public IType, public CPositionStorage {
public:
	CUserType( const std::string& _typeName, const CPosition& _position );
	CUserType( const CUserType* other );

	void Accept( IVisitor*  visitor ) const override;

	bool IsPODType() const override;
	const CSymbol* GetTypeName() const;

private:
	const CSymbol* typeName;
};

class CBinOpExpression : public IExp, public CPositionStorage {
public:
	enum BinOp { AND, LESS, PLUS, MINUS, TIMES, DIVIDE };

	CBinOpExpression( IExp* _leftExp, BinOp _binOp, IExp* _rightExp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	BinOp GetBinOp() const;
	const IExp* GetLeftExp() const;	
	const IExp* GetRightExp() const;

private:
	const std::shared_ptr<IExp> leftExp;
	const std::shared_ptr<IExp> rightExp;
	BinOp binOp;
};

class CIndexExpression : public IExp, public CPositionStorage {
public:
	CIndexExpression( IExp* _exp, IExp* _indexExp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetExp() const;
	const IExp* GetIndexExp() const;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExp> indexExp;
};

class CLenghtExpression : public IExp, public CPositionStorage {
public:
	CLenghtExpression( IExp* _exp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetExp() const;

private:
	const std::shared_ptr<IExp> exp;
};

class CMethodExpression : public IExp, public CPositionStorage {
public:
	CMethodExpression( IExp* _exp, const std::string& _identifier, IExpList* _expList, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetExp() const;
	const IExpList* GetExpList() const;
	const CSymbol* GetIdentifier() const;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
	const CSymbol* identifier;
};

class CIntLiteralExpression : public IExp, public CPositionStorage {
public:
	CIntLiteralExpression( const std::string& _val, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const CSymbol* GetValue() const;

private:
	const CSymbol* val;
};

class CBoolLiteralExpression : public IExp, public CPositionStorage {
public:
	CBoolLiteralExpression( const std::string& _val, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const CSymbol* GetValue() const;

private:
	const CSymbol* val;
};

class CIdentifierExpression : public IExp, public CPositionStorage {
public:
	CIdentifierExpression( const std::string& id, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const CSymbol* GetIdentifier() const;

private:
	const CSymbol* identifier;
};

class CThisExpression : public IExp, public CPositionStorage {
public:
	CThisExpression( const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;
};

class CNewIntArrayExpression : public IExp, public CPositionStorage {
public:
	CNewIntArrayExpression( IExp* _exp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetExp() const;

private:
	const std::shared_ptr<IExp> exp;
};

class CNewExpression : public IExp, public CPositionStorage {
public:
	CNewExpression( const std::string& id, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const CSymbol* GetIdentifier() const;

private:
	const CSymbol* identifier;
};

class CUnaryOpExpression : public IExp, public CPositionStorage {
public:
	enum UnaryOp { MINUS, NOT };

	CUnaryOpExpression( UnaryOp _op, IExp* _exp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetLeftExp() const;
	UnaryOp GetOperation() const;

private:
	const std::shared_ptr<IExp> exp;
	UnaryOp op;
};

class CBracesExpression : public IExp, public CPositionStorage {
public:
	CBracesExpression( IExp* _exp, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

	const IExp* GetExp() const;

private:
	const std::shared_ptr<IExp> exp;
};

class CExpressionList : public IExpList, public CPositionStorage {
public:
	CExpressionList( IExp* _exp, IExpList* _expList, const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;
	
	const IExp* GetExp() const;
	const IExpList* GetExpList() const;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
};

class CExpressionRest : public IExp, public CPositionStorage {
public:
	CExpressionRest( IExp* _exp, const CPosition& pos );

	void Accept( IVisitor* visitor ) const override;

	const IExp* GetExp() const;

private:
	std::shared_ptr<IExp> exp;
};