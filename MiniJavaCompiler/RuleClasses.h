#pragma once
#include <string>
#include <memory>

#include "grammar.h"
#include "Position.h"
#include "Visitor.h"

class CMainClass;
class CClassDeclList;
class CClassDecl;

class CProgram : public IProgram {
public:
	CProgram( IMainClass* _mainClass, IClassDeclList* _classList, CPosition& position)
		: mainClass( _mainClass ),
		classList( _classList ), 
		position( position )
	{}

	IMainClass* GetMainClass()
	{
		return mainClass.get();
	}

	IClassDeclList* GetClassDeclList()
	{
		return classList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	std::shared_ptr<IMainClass> mainClass;
	std::shared_ptr<IClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(
		const std::string& _identifier,
		const std::string& _argv,
		IStatement* _statementList,
		const CPosition& _position)
		: identifier( _identifier )
		, argv( _argv )
		, statementList( _statementList )
		, position( _position )
	{}

	IStatement* GetStatementList()
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}
private:
	std::string identifier;
	std::string argv;
	std::shared_ptr<IStatement> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( IClassDecl* _declaration, IClassDeclList* _list, const CPosition& _positon ) :
		declaration( _declaration ),
		list( _list ),
		position( _positon )
	{}

	IClassDeclList* GetClassDeclList()
	{
		return list.get();
	}

	IClassDecl* GetClassDecl()
	{
		return declaration.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IClassDeclList> list;
	std::shared_ptr<IClassDecl> declaration;
	CPosition position;
};

class CClassDecl : public IClassDecl {
public:
	CClassDecl(
		IVarDeclList* _varList,
		IMethodDeclList* _methodList,
		const std::string& _className,
		const CPosition& _position ) :
		position( _position ),
		className( _className ),
		varList( _varList ),
		methodList( _methodList )
	{}

	std::string GetName()
	{
		return className;
	}

	IVarDeclList* GetVarDeclList()
	{
		return varList.get();
	}

	IMethodDeclList* GetMethodDeclList()
	{
		return methodList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	CPosition position;
	std::string className;
	std::shared_ptr<IVarDeclList> varList;
	std::shared_ptr<IMethodDeclList> methodList;
};

class CClassDeclDerived : IClassDecl {
public:
	CClassDeclDerived(
		IVarDeclList* _varList,
		IMethodDeclList* _methodList,
		const std::string& _className,
		const std::string& _baseClassName,
		const CPosition& _position ) :
		position( _position ),
		className( _className ),
		baseClassName( _baseClassName ),
		varList( _varList ),
		methodList( _methodList )
	{
	}

	std::string GetName() const
	{
		return className;
	}

	std::string GetBaseClassName() const
	{
		return baseClassName;
	}

	IVarDeclList* GetVarDeclList()
	{
		return varList.get();
	}

	IMethodDeclList* GetMethodDeclList()
	{
		return methodList.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	CPosition position;
	std::string className;
	std::string baseClassName;
	std::shared_ptr<IVarDeclList> varList;
	std::shared_ptr<IMethodDeclList> methodList;
};

class CStatementList : public IStatementList { // not implemented
public:
	CStatementList( const IStatement* _stmt, const IStatementList* _stmtList, const CPosition& pos ) :
		stmt( _stmt ),
		stmtList( _stmtList ),
		position( pos )
	{}

	const IStatement* GetStatement() const
	{
		return stmt.get();
	}

	const IStatementList* GetStatementList() const
	{
		return stmtList.get();
	}
private:
	const std::shared_ptr<IStatement> stmt;
	const std::shared_ptr<IStatementList> stmtList;
	CPosition position;
};

class CVarDecl : public IVarDecl {
public:
	CVarDecl( IType* _type, const std::string& _identifier, const CPosition& _position ) :
		type( _type ),
		identifier( _identifier ),
		position( _position )
	{}

	IType* GetType()
	{
		return type.get();
	}

	std::string GetName() const
	{
		return identifier;
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	CPosition position;
	std::shared_ptr<IType> type;
	std::string identifier;
};

class CMethodDecl : public IMethodDecl {
public:
	CMethodDecl(
		const IType* _type,
		const std::string& _methodName,
		const IFormalList* _formalList,
		const IVarDeclList* _varList,
		const CStatementList* _statementList,
		const IExp* _returnExpr,
		const CPosition& _position ) :
		type( _type ),
		methodName( _methodName ),
		formalList( _formalList ),
		varList( _varList ),
		statementList( _statementList ),
		returnExpr( _returnExpr ),
		position( _position )
	{}

	std::string GetName() const 
	{
		return methodName;
	}

	IFormalList* GetFormalList()
	{
		return formalList.get();
	}

	IVarDeclList* GetVarList()
	{
		return varList.get();
	}

	IStatementList* GetStatementList()
	{
		return statementList.get();
	}

	IExp* GetReturnExpression()
	{
		return returnExpr.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IType> type;
	const std::string methodName;
	const std::shared_ptr<IFormalList> formalList;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IStatementList> statementList;
	const std::shared_ptr<IExp> returnExpr;
	CPosition position;
};

class CStatementListStatement : public IStatement {
public:
	CStatementListStatement( CStatementList* _statementList, const CPosition& _position ) :
		statementList( _statementList ),
		position( _position )
	{}

	CStatementList* GetStatementList()
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::shared_ptr<CStatementList> statementList;
	CPosition position;
};

class CIfStatement : public IStatement {
public:
	CIfStatement(
		IExp* _condition,
		IStatement* _statementIfTrue,
		IStatement* _statementIfFalse,
		const CPosition _position ) :
		condition( _condition ),
		statementIfTrue( _statementIfTrue ),
		statementIfFalse( _statementIfFalse ),
		position( _position )
	{}

	IExp* GetCondition()
	{
		return condition.get();
	}

	IStatement* GetIfTrueStatement()
	{
		return statementIfTrue.get();
	}

	IStatement* GetIfFalseStatement()
	{
		return statementIfFalse.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::shared_ptr<IExp> condition;
	std::shared_ptr<IStatement> statementIfTrue;
	std::shared_ptr<IStatement> statementIfFalse;
	CPosition position;
};

class CWhileStatement : IStatement {
public:
	CWhileStatement( IExp* _condition, IStatement* _cycleBody, const CPosition& _position ) :
		condition( _condition ),
		cycleBody( _cycleBody ),
		position( _position )
	{}

	IExp* GetCondition()
	{
		return condition.get();
	}

	IStatement* GetBodyCycle()
	{
		return cycleBody.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::shared_ptr<IExp> condition;
	std::shared_ptr<IStatement> cycleBody;
	CPosition position;
};

class CPrintStatement : public IStatement {
public:
	CPrintStatement( IExp* _expression, const CPosition& _position ) :
		expression( _expression ),
		position( _position )
	{}

	IExp* GetExpression()
	{
		return expression.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::shared_ptr<IExp> expression;
	CPosition position;
};

class CAssignStatement : public IStatement {
public:
	CAssignStatement( const std::string& _left, IExp* _right, const CPosition& _position ) :
		left( _left ),
		right( _right ),
		position( _position )
	{}

	std::string GetLeftPart() const
	{
		return left;
	}

	IExp* GetRightPart()
	{
		return right.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::string left;
	std::shared_ptr<IExp> right;
	CPosition position;
};

class CArrayAssignStatement : public IStatement {
public:
	CArrayAssignStatement( const std::string& _arrayId, IExp* _elementNumber, IExp* _rightPart, const CPosition& _position) :
		arrayId( _arrayId ),
		elementNumber( _elementNumber ),
		rightPart( _rightPart ),
		position( _position )
	{}

	std::string GetArrayName() const
	{
		return arrayId;
	}

	IExp* GetElementNumber()
	{
		return elementNumber.get();
	}

	IExp* GetRightPart()
	{
		return rightPart.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		return visitor->Visit( this );
	}

private:
	std::string arrayId;
	std::shared_ptr<IExp> elementNumber;
	std::shared_ptr<IExp> rightPart;
	CPosition position;
};

class CStandardType : public IType {
public:
	enum StandardType { INT = 0, INT_ARRAY, BOOL };

	CStandardType( StandardType _type, const CPosition& _position ) :
		type( _type ),
		position( _position )
	{}

	StandardType GetType() const
	{
		return type;
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	StandardType type;
	CPosition position;
};

class CUserType : public IType {
public:
	CUserType( const std::string& _typeName, const CPosition& _position ) :
		typeName( _typeName ),
		position( _position )
	{}

	std::string GetTypeName() const
	{
		return typeName;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::string typeName;
	CPosition position;
};

class CBinOpExpression : public IExp {
public:
	enum BinOp { AND, LESS, PLUS, MINUS, TIMES, DIVIDE };

	CBinOpExpression( IExp* _leftExp, BinOp _binOp, IExp* _rightExp, const CPosition& pos ) :
		leftExp( _leftExp ),
		rightExp( _rightExp ),
		position( pos ),
		binOp( _binOp )
	{}

	IExp* GetLeftExp() 
	{
		return leftExp.get();
	}
	
	IExp* GetRightExp()
	{
		return rightExp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> leftExp;
	std::shared_ptr<IExp> rightExp;
	CPosition position;
	BinOp binOp;
};

class CIndexExpression : public IExp {
public:
	CIndexExpression( IExp* _exp, IExp* _indexExp, const CPosition& pos ) :
		exp( _exp ),
		indexExp( _indexExp ),
		position( pos )
	{}

	IExp* GetExp()
	{
		return exp.get();
	}

	IExp* GetIndexExp()
	{
		return indexExp.get( );
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	std::shared_ptr<IExp> indexExp;
	CPosition position;
};

class CLenghtExpression : public IExp {
public:
	CLenghtExpression( IExp* _exp, const CPosition& pos ) :
		exp( _exp ),
		position( pos )
	{}

	IExp* GetExp()
	{
		return exp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	CPosition position;
};

class CMethodExpression : public IExp {
public:
	CMethodExpression( IExp* _exp, const std::string& _identifier, IExpList* _expList, const CPosition& pos ) :
		exp( _exp ),
		identifier( _identifier ),
		expList( _expList ),
		position( pos )
	{}

	IExp* GetExp()
	{
		return exp.get();
	}

	IExpList* GetIndexExp()
	{
		return expList.get( );
	}

	std::string GetIdentifier() const
	{
		return identifier;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	std::shared_ptr<IExpList> expList;
	std::string identifier;
	CPosition position;
};

class CIntLiteralExpression : public IExp {
public:
	CIntLiteralExpression( int _val, const CPosition& pos ) :
		val( _val ), 
		position( pos )
	{}

	int GetValue() const
	{
		return val;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	int val;
	CPosition position;
};

class CBoolLiteralExpression : public IExp {
public:
	CBoolLiteralExpression( bool _val, const CPosition& pos ) :
		val( _val ), 
		position( pos )
	{}

	bool GetValue() const
	{
		return val;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	bool val;
	CPosition position;
};

class CIdentifierExpression : public IExp {
public:
	CIdentifierExpression( const std::string& id, const CPosition& pos ) :
		identifier( id ), 
		position( pos )
	{}

	std::string GetIdentifier() const
	{
		return identifier;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::string identifier;
	CPosition position;
};

class CThisExpression : public IExp {
public:
	CThisExpression( const CPosition& pos ) :
		position( pos )
	{}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	CPosition position;
};

class CNewIntArrayExpression : public IExp {
public:
	CNewIntArrayExpression( IExp* _exp, const CPosition& pos ) :
		exp( _exp ), 
		position( pos )
	{}

	IExp* GetExp() const
	{
		return exp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	CPosition position;
};

class CNewExpression : public IExp {
public:
	CNewExpression( const std::string& id, const CPosition& pos ) :
		identifier( id ), 
		position( pos )
	{}

	std::string GetIdentifier() const
	{
		return identifier;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::string identifier;
	CPosition position;
};

class CUnaryOpExpression : public IExp {
public:
	enum UnaryOp { MINUS, NOT };

	CUnaryOpExpression( UnaryOp _op, IExp* _exp, const CPosition& pos ) :
		exp( _exp ),
		position( pos ),
		op( _op )
	{}

	IExp* GetLeftExp()
	{
		return exp.get();
	}

	UnaryOp GetOperation() const
	{
		return op;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	CPosition position;
	UnaryOp op;
};

class CBracesExpression : public IExp {
	CBracesExpression( IExp* _exp, const CPosition& pos ) :
	exp( _exp ),
		position( pos )
	{}

	std::shared_ptr<IExp> GetExp() const
	{
		return exp;
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	CPosition position;
};

class CExpressionList : public IExpList {
public:
	CExpressionList( IExp* _exp, IExpList* _expList, const CPosition& pos ) :
		exp( _exp ),
		expList( _expList ),
		position( pos )
	{}
	
	IExp* GetExp()
	{
		return exp.get();
	}

	IExpList* GetExpList()
	{
		return expList.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<IExp> exp;
	std::shared_ptr<IExpList> expList;
	CPosition position;
};	