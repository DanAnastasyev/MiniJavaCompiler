#pragma once
#include <string>
#include <memory>
#include <vector>

#include "grammar.h"
#include "Position.h"
#include "Visitors/Visitor.h"
#include "SymbolsTable/Symbol.h"

class CProgram : public IProgram {
public:
	CProgram( IMainClass* _mainClass, IClassDeclList* _classList, CPosition& position );

	const IMainClass* GetMainClass() const;

	const IClassDeclList* GetClassDeclList() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IMainClass> mainClass;
	const std::shared_ptr<IClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass( const std::string& _identifier, const std::string& _arsId, IStatement* _statement, const CPosition& _position );

	const IStatement* GetStatement() const;

	const CSymbol* GetClassName() const;

	const CSymbol* GetMainArgsIdentifier() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const CSymbol* identifier;
	const CSymbol* argsIdentifier;
	const std::shared_ptr<IStatement> statement;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( IClassDecl* _declaration, IClassDeclList* _list, const CPosition& _positon );

	const IClassDeclList* GetClassDeclList() const;

	const IClassDecl* GetClassDecl() const;

	void Accept( IVisitor* visitor ) const override;
private:
	const std::shared_ptr<IClassDeclList> list;
	const std::shared_ptr<IClassDecl> declaration;
	CPosition position;
};

class CClassDecl : public IClassDecl {
public:
	CClassDecl( IVarDeclList* _varList, IMethodDeclList* _methodList, const std::string& _className, const CPosition& _position );

	const CSymbol* GetName() const;

	const IVarDeclList* GetVarDeclList() const;

	const IMethodDeclList* GetMethodDeclList() const;

	void Accept( IVisitor* visitor ) const override;

private:
	CPosition position;
	const CSymbol* className;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CClassDeclDerived : public IClassDecl {
public:
	CClassDeclDerived(
		IVarDeclList* _varList,
		IMethodDeclList* _methodList,
		const std::string& _className,
		const std::string& _baseClassName,
		const CPosition& _position );

	const CSymbol* GetName() const;

	const CSymbol* GetBaseClassName() const;

	const IVarDeclList* GetVarDeclList() const;

	const IMethodDeclList* GetMethodDeclList() const;

	void Accept( IVisitor* visitor ) const override;

private:
	CPosition position;
	const CSymbol* className;
	const CSymbol* baseClassName;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CStatementList : public IStatementList { // not implemented
public:
	CStatementList( IStatement* _stmt, IStatementList* _stmtList, const CPosition& pos );

	const IStatement* GetStatement() const;

	const IStatementList* GetStatementList() const;

	void Accept( IVisitor* visitor ) const override;
private:
	const std::shared_ptr<IStatement> stmt;
	const std::shared_ptr<IStatementList> stmtList;
	CPosition position;
};

class CVarDecl : public IVarDecl {
public:
	CVarDecl( IType* _type, const std::string& _identifier, const CPosition& _position );

	const IType* GetType() const;

	const CSymbol* GetName() const;

	void Accept( IVisitor* visitor ) const override;

private:
	CPosition position;
	const std::shared_ptr<IType> type;
	const CSymbol* identifier;
};

class CVarDeclList : public IVarDeclList {
public:
	CVarDeclList( IVarDeclList* _varDeclList, IVarDecl* _varDecl, const CPosition& pos );

	const IVarDeclList* GetVarDeclList() const;

	const IVarDecl* GetVarDecl() const;

	void Accept( IVisitor* visitor ) const override;
private:
	const std::shared_ptr<IVarDeclList> varDeclList;
	const std::shared_ptr<IVarDecl> varDecl;
	CPosition position;
};

class CMethodDecl : public IMethodDecl {
public:
	CMethodDecl(
		IType* _type,
		const std::string& _methodName,
		IFormalList* _formalList,
		IVarDeclList* _varList,
		IStatementList* _statementList,
		IExp* _returnExpr,
		const CPosition& _position );

	const IType* GetType() const;

	const CSymbol* GetName() const;

	const IFormalList* GetFormalList() const;

	const IVarDeclList* GetVarList() const;

	const IStatementList* GetStatementList() const;

	const IExp* GetReturnExpression() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IType> type;
	const CSymbol* methodName;
	const std::shared_ptr<IFormalList> formalList;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IStatementList> statementList;
	const std::shared_ptr<IExp> returnExpr;
	CPosition position;
};

class CMethodDeclList : public IMethodDeclList {
public:
	CMethodDeclList( IMethodDecl* _methodDecl, IMethodDeclList* _methodDeclList, const CPosition& pos );

	const IMethodDeclList* GetMethodDeclList() const;

	const IMethodDecl* GetMethodDecl() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IMethodDeclList> methodDeclList;
	const std::shared_ptr<IMethodDecl> methodDecl;
	CPosition position;
};

class CFormalList : public IFormalList {
public:
	CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos );

	const IFormalList* GetFormalRest() const;

	const IType* GetType() const;

	const CSymbol* GetIdentifier() const;


	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IType> type;
	const CSymbol* identifier;

	std::shared_ptr<IFormalList> formalList;

	CPosition position;
};

class CFormalRestList : public IFormalList {
public:
	CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos );

	const IFormalList* GetFormalRest() const;

	const IFormalList* GetFormalRestList() const;


	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IFormalList> formalRest;
	std::shared_ptr<IFormalList> formalRestList;
	CPosition position;
};

class CStatementListStatement : public IStatement {
public:
	CStatementListStatement( IStatementList* _statementList, const CPosition& _position );

	const IStatementList* GetStatementList() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IStatementList> statementList;
	CPosition position;
};

class CIfStatement : public IStatement {
public:
	CIfStatement( IExp* _condition, IStatement* _statementIfTrue, IStatement* _statementIfFalse, const CPosition _position );

	const IExp* GetCondition() const;

	const IStatement* GetIfTrueStatement() const;

	const IStatement* GetIfFalseStatement() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> statementIfTrue;
	const std::shared_ptr<IStatement> statementIfFalse;
	CPosition position;
};

class CWhileStatement : public IStatement {
public:
	CWhileStatement( IExp* _condition, IStatement* _cycleBody, const CPosition& _position );

	const IExp* GetCondition() const;

	const IStatement* GetBodyCycle() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> cycleBody;
	CPosition position;
};

class CPrintStatement : public IStatement {
public:
	CPrintStatement( IExp* _expression, const CPosition& _position );

	const IExp* GetExpression() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const std::shared_ptr<IExp> expression;
	CPosition position;
};

class CAssignStatement : public IStatement {
public:
	CAssignStatement( const std::string& _left, IExp* _right, const CPosition& _position );

	const CSymbol* GetLeftPart() const;

	const IExp* GetRightPart() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const CSymbol* left;
	const std::shared_ptr<IExp> right;
	CPosition position;
};

class CArrayAssignStatement : public IStatement {
public:
	CArrayAssignStatement( const std::string& _arrayId, IExp* _elementNumber, IExp* _rightPart, const CPosition& _position );

	const CSymbol* GetArrayName() const;

	const IExp* GetElementNumber() const;

	const IExp* GetRightPart() const;

	void Accept( IVisitor* visitor ) const override;

private:
	const CSymbol* arrayId;
	const std::shared_ptr<IExp> elementNumber;
	const std::shared_ptr<IExp> rightPart;
	CPosition position;
};

class CStandardType : public IType {
public:
	enum StandardType { INT = 0, INT_ARRAY, BOOL };

	CStandardType( StandardType _type, const CPosition& _position );

	CStandardType( const CStandardType* other );

	StandardType GetType() const;

	void Accept( IVisitor* visitor ) const override;

	CPosition GetPosition() const;
private:
	StandardType type;
	CPosition position;
};

class CUserType : public IType {
public:
	CUserType( const std::string& _typeName, const CPosition& _position );
	CUserType( const CUserType* other );

	const CSymbol* GetTypeName() const;

	void Accept( IVisitor*  visitor ) const override;

	CPosition GetPosition( ) const;
private:
	const CSymbol* typeName;
	CPosition position;
};

class CBinOpExpression : public IExp {
public:
	enum BinOp { AND, LESS, PLUS, MINUS, TIMES, DIVIDE };

	CBinOpExpression( IExp* _leftExp, BinOp _binOp, IExp* _rightExp, const CPosition& pos );

	BinOp GetBinOp() const;

	const IExp* GetLeftExp() const;
	
	const IExp* GetRightExp() const;

	void Accept( IVisitor*  visitor ) const override;
private:
	const std::shared_ptr<IExp> leftExp;
	const std::shared_ptr<IExp> rightExp;
	CPosition position;
	BinOp binOp;
};

class CIndexExpression : public IExp {
public:
	CIndexExpression( IExp* _exp, IExp* _indexExp, const CPosition& pos );

	const IExp* GetExp() const;

	const IExp* GetIndexExp() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExp> indexExp;
	CPosition position;
};

class CLenghtExpression : public IExp {
public:
	CLenghtExpression( IExp* _exp, const CPosition& pos );

	const IExp* GetExp() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
};

class CMethodExpression : public IExp {
public:
	CMethodExpression( IExp* _exp, const std::string& _identifier, IExpList* _expList, const CPosition& pos );

	const IExp* GetExp() const;

	const IExpList* GetIndexExp() const;

	const CSymbol* GetIdentifier() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
	const CSymbol* identifier;
	CPosition position;
};

class CIntLiteralExpression : public IExp {
public:
	CIntLiteralExpression( const std::string& _val, const CPosition& pos );

	const CSymbol* GetValue( ) const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const CSymbol* val;
	CPosition position;
};

class CBoolLiteralExpression : public IExp {
public:
	CBoolLiteralExpression( const std::string& _val, const CPosition& pos );

	const CSymbol* GetValue( ) const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const CSymbol* val;
	CPosition position;
};

class CIdentifierExpression : public IExp {
public:
	CIdentifierExpression( const std::string& id, const CPosition& pos );

	const CSymbol* GetIdentifier() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const CSymbol* identifier;
	CPosition position;
};

class CThisExpression : public IExp {
public:
	CThisExpression( const CPosition& pos );

	void Accept( IVisitor*  visitor ) const override;

private:
	CPosition position;
};

class CNewIntArrayExpression : public IExp {
public:
	CNewIntArrayExpression( IExp* _exp, const CPosition& pos );

	const IExp* GetExp() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
};

class CNewExpression : public IExp {
public:
	CNewExpression( const std::string& id, const CPosition& pos );

	const CSymbol* GetIdentifier() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const CSymbol* identifier;
	CPosition position;
};

class CUnaryOpExpression : public IExp {
public:
	enum UnaryOp { MINUS, NOT };

	CUnaryOpExpression( UnaryOp _op, IExp* _exp, const CPosition& pos );

	const IExp* GetLeftExp() const;

	UnaryOp GetOperation() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
	UnaryOp op;
};

class CBracesExpression : public IExp {
public:
	CBracesExpression( IExp* _exp, const CPosition& pos );

	const IExp* GetExp() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
};

class CExpressionList : public IExpList {
public:
	CExpressionList( IExp* _exp, IExpList* _expList, const CPosition& pos );
	
	const IExp* GetExp() const;

	const IExpList* GetExpList() const;

	void Accept( IVisitor*  visitor ) const override;

private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
	CPosition position;
};

class CExpressionRest : public IExp {
public:
	CExpressionRest( IExp* _exp, const CPosition& pos );

	const IExp* GetExp() const;

	void Accept( IVisitor* visitor ) const override;

private:
	std::shared_ptr<IExp> exp;
	CPosition position;
};