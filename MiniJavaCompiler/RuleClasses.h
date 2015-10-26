#pragma once
#include <string>
#include <memory>
#include <vector>

#include "grammar.h"
#include "Position.h"
#include "Visitor.h"

class CMainClass;
class CClassDeclList;
class CClassDecl;

class CProgram : public IProgram {
public:
	CProgram( const IMainClass* _mainClass, const IClassDeclList* _classList, CPosition& position) :
		mainClass( _mainClass ),
		classList( _classList ), 
		position( position )
	{}

	const IMainClass* GetMainClass() const
	{
		return mainClass.get();
	}

	const IClassDeclList* GetClassDeclList() const
	{
		return classList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IMainClass> mainClass;
	const std::shared_ptr<IClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(
		const std::string& _identifier,
		const std::string& _arsId,
		const IStatement* _statementList,
		const CPosition& _position) :
		identifier( _identifier ), 
		argsIdentifier( _arsId ),
		statementList( _statementList ), 
		position( _position )
	{}

	const IStatement* GetStatementList( ) const
	{
		return statementList.get();
	}

	std::string GetClassName() const
	{
		return identifier;
	}

	std::string GetMainArgsIdentifier() const
	{
		return argsIdentifier;
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	const std::string identifier;
	const std::string argsIdentifier;
	const std::shared_ptr<IStatement> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( const IClassDecl* _declaration, const IClassDeclList* _list, const CPosition& _positon ) :
		declaration( _declaration ),
		list( _list ),
		position( _positon )
	{}

	const IClassDeclList* GetClassDeclList( ) const
	{
		return list.get();
	}

	const IClassDecl* GetClassDecl() const
	{
		return declaration.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IClassDeclList> list;
	const std::shared_ptr<IClassDecl> declaration;
	CPosition position;
};

class CClassDecl : public IClassDecl {
public:
	CClassDecl(
		const IVarDeclList* _varList,
		const IMethodDeclList* _methodList,
		const std::string& _className,
		const CPosition& _position ) :
		position( _position ),
		className( _className ),
		varList( _varList ),
		methodList( _methodList )
	{}

	std::string GetName() const
	{
		return className;
	}

	const IVarDeclList* GetVarDeclList() const 
	{
		return varList.get();
	}

	const IMethodDeclList* GetMethodDeclList() const
	{
		return methodList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}

private:
	CPosition position;
	const std::string className;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CClassDeclDerived : IClassDecl {
public:
	CClassDeclDerived(
		const IVarDeclList* _varList,
		const IMethodDeclList* _methodList,
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

	const IVarDeclList* GetVarDeclList( ) const
	{
		return varList.get();
	}

	const IMethodDeclList* GetMethodDeclList( ) const
	{
		return methodList.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	CPosition position;
	const std::string className;
	const std::string baseClassName;
	const std::shared_ptr<IVarDeclList> varList;
	const std::shared_ptr<IMethodDeclList> methodList;
};

class CStatementList : public IStatementList { // not implemented
public:
	CStatementList( const IStatement* _stmt, const IStatementList* _stmtList, const CPosition& pos ) :
		stmt( _stmt ),
		stmtList( _stmtList ),
		position( pos )
	{}

	const IStatement* GetStatement( ) const
	{
		return stmt.get();
	}

	const IStatementList* GetStatementList( ) const
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
	CVarDecl( const IType* _type, const std::string& _identifier, const CPosition& _position ) :
		type( _type ),
		identifier( _identifier ),
		position( _position )
	{}

	const IType* GetType( ) const
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
	const std::shared_ptr<IType> type;
	const std::string identifier;
};

class CVarDeclList : public IVarDeclList {
public:
	CVarDeclList( const IVarDeclList* _varDeclList, const IVarDecl* _varDecl, const CPosition& pos ) :
		varDeclList(_varDeclList),
		varDecl( _varDecl ),
		position( pos )
	{}

	const IVarDeclList* GetVarDeclList() const
	{
		return varDeclList.get();
	}

	const IVarDecl* GetVarDecl() const
	{
		return varDecl.get();
	}
private:
	const std::shared_ptr<IVarDeclList> varDeclList;
	const std::shared_ptr<IVarDecl> varDecl;
	CPosition position;
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

	const IFormalList* GetFormalList( ) const
	{
		return formalList.get();
	}

	const IVarDeclList* GetVarList( ) const
	{
		return varList.get();
	}

	const IStatementList* GetStatementList( ) const
	{
		return statementList.get();
	}

	const IExp* GetReturnExpression( ) const
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

class CMethodDeclList : public IMethodDeclList {
public:
	CMethodDeclList( const IMethodDecl* _methodDecl, const IMethodDeclList* _methodDeclList, const CPosition& pos ) :
		methodDeclList( _methodDeclList ),
		methodDecl( _methodDecl ),
		position( pos )
	{}

	const IVarDeclList* GetMethodDeclList() const
	{
		return methodDeclList.get();
	}

	const IVarDecl* GetMethodDecl() const
	{
		return methodDecl.get( );
	}

	void Accept( IVisitor* visitor ) const override
	{
//		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IVarDeclList> methodDeclList;
	const std::shared_ptr<IVarDecl> methodDecl;
	CPosition position;
};

class CStatementListStatement : public IStatement {
public:
	CStatementListStatement( const IStatementList* _statementList, const CPosition& _position ) :
		statementList( _statementList ),
		position( _position )
	{}

	const IStatementList* GetStatementList() const
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IStatementList> statementList;
	CPosition position;
};

class CIfStatement : public IStatement {
public:
	CIfStatement(
		const IExp* _condition,
		const IStatement* _statementIfTrue,
		const IStatement* _statementIfFalse,
		const CPosition _position ) :
		condition( _condition ),
		statementIfTrue( _statementIfTrue ),
		statementIfFalse( _statementIfFalse ),
		position( _position )
	{}

	const IExp* GetCondition( ) const
	{
		return condition.get();
	}

	const IStatement* GetIfTrueStatement( ) const
	{
		return statementIfTrue.get();
	}

	const IStatement* GetIfFalseStatement( ) const
	{
		return statementIfFalse.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> statementIfTrue;
	const std::shared_ptr<IStatement> statementIfFalse;
	CPosition position;
};

class CWhileStatement : IStatement {
public:
	CWhileStatement( const IExp* _condition, const IStatement* _cycleBody, const CPosition& _position ) :
		condition( _condition ),
		cycleBody( _cycleBody ),
		position( _position )
	{}

	const IExp* GetCondition( ) const
	{
		return condition.get();
	}

	const IStatement* GetBodyCycle( ) const
	{
		return cycleBody.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IExp> condition;
	const std::shared_ptr<IStatement> cycleBody;
	CPosition position;
};

class CPrintStatement : public IStatement {
public:
	CPrintStatement( const IExp* _expression, const CPosition& _position ) :
		expression( _expression ),
		position( _position )
	{}

	const IExp* GetExpression( ) const
	{
		return expression.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::shared_ptr<IExp> expression;
	CPosition position;
};

class CAssignStatement : public IStatement {
public:
	CAssignStatement( const std::string& _left, const IExp* _right, const CPosition& _position ) :
		left( _left ),
		right( _right ),
		position( _position )
	{}

	std::string GetLeftPart() const
	{
		return left;
	}

	const IExp* GetRightPart( ) const
	{
		return right.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	const std::string left;
	const std::shared_ptr<IExp> right;
	CPosition position;
};

class CArrayAssignStatement : public IStatement {
public:
	CArrayAssignStatement( const std::string& _arrayId, const IExp* _elementNumber, const IExp* _rightPart, const CPosition& _position) :
		arrayId( _arrayId ),
		elementNumber( _elementNumber ),
		rightPart( _rightPart ),
		position( _position )
	{}

	std::string GetArrayName() const
	{
		return arrayId;
	}

	const IExp* GetElementNumber( ) const
	{
		return elementNumber.get();
	}

	const IExp* GetRightPart( ) const
	{
		return rightPart.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		return visitor->Visit( this );
	}

private:
	const std::string arrayId;
	const std::shared_ptr<IExp> elementNumber;
	const std::shared_ptr<IExp> rightPart;
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
	const std::string typeName;
	CPosition position;
};

class CBinOpExpression : public IExp {
public:
	enum BinOp { AND, LESS, PLUS, MINUS, TIMES, DIVIDE };

	CBinOpExpression( const IExp* _leftExp, BinOp _binOp, const IExp* _rightExp, const CPosition& pos ) :
		leftExp( _leftExp ),
		rightExp( _rightExp ),
		position( pos ),
		binOp( _binOp )
	{}

	const IExp* GetLeftExp( ) const
	{
		return leftExp.get();
	}
	
	const IExp* GetRightExp( ) const
	{
		return rightExp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> leftExp;
	const std::shared_ptr<IExp> rightExp;
	CPosition position;
	BinOp binOp;
};

class CIndexExpression : public IExp {
public:
	CIndexExpression( const IExp* _exp, const IExp* _indexExp, const CPosition& pos ) :
		exp( _exp ),
		indexExp( _indexExp ),
		position( pos )
	{}

	const IExp* GetExp( ) const
	{
		return exp.get();
	}

	const IExp* GetIndexExp( ) const
	{
		return indexExp.get( );
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExp> indexExp;
	CPosition position;
};

class CLenghtExpression : public IExp {
public:
	CLenghtExpression( const IExp* _exp, const CPosition& pos ) :
		exp( _exp ),
		position( pos )
	{}

	const IExp* GetExp( ) const
	{
		return exp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
};

class CMethodExpression : public IExp {
public:
	CMethodExpression( const IExp* _exp, const std::string& _identifier, const IExpList* _expList, const CPosition& pos ) :
		exp( _exp ),
		identifier( _identifier ),
		expList( _expList ),
		position( pos )
	{}

	const IExp* GetExp( ) const
	{
		return exp.get();
	}

	const IExpList* GetIndexExp( ) const
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
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
	const std::string identifier;
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
	const std::string identifier;
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
	CNewIntArrayExpression( const IExp* _exp, const CPosition& pos ) :
		exp( _exp ), 
		position( pos )
	{}

	const IExp* GetExp( ) const
	{
		return exp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> exp;
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
	const std::string identifier;
	CPosition position;
};

class CUnaryOpExpression : public IExp {
public:
	enum UnaryOp { MINUS, NOT };

	CUnaryOpExpression( UnaryOp _op, const IExp* _exp, const CPosition& pos ) :
		exp( _exp ),
		position( pos ),
		op( _op )
	{}

	const IExp* GetLeftExp( ) const
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
	const std::shared_ptr<IExp> exp;
	CPosition position;
	UnaryOp op;
};

class CBracesExpression : public IExp {
	CBracesExpression( const IExp* _exp, const CPosition& pos ) :
	exp( _exp ),
		position( pos )
	{}

	const IExp* GetExp() const
	{
		return exp.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> exp;
	CPosition position;
};

class CExpressionList : public IExpList {
public:
	CExpressionList( const IExp* _exp, const IExpList* _expList, const CPosition& pos ) :
		exp( _exp ),
		expList( _expList ),
		position( pos )
	{}
	
	const IExp* GetExp( ) const
	{
		return exp.get();
	}

	const IExpList* GetExpList( ) const
	{
		return expList.get();
	}

	void Accept( IVisitor*  visitor ) const override
	{
		visitor->Visit( this );
	}
private:
	const std::shared_ptr<IExp> exp;
	const std::shared_ptr<IExpList> expList;
	CPosition position;
};	