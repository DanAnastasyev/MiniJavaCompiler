#pragma once
#include <string>
#include <memory>

#include "grammar.h"
#include "Position.h"
#include "Visitor.h"
#include <vector>

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
	std::shared_ptr<IMainClass> mainClass;
	std::shared_ptr<IClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(
		const std::string& _identifier,
		const std::string& _arsId,
		IStatementList* _statementList,
		const CPosition& _position)
		: identifier( _identifier ), 
		argsIdentifier( _arsId ),
		statementList( _statementList ), 
		position( _position )
	{}

	const IStatementList* GetStatementList( ) const
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
	std::string identifier;
	std::string argsIdentifier;
	std::shared_ptr<IStatementList> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( IClassDecl* _declaration, IClassDeclList* _list, const CPosition& _positon ) :
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
	std::string className;
	std::string baseClassName;
	std::shared_ptr<IVarDeclList> varList;
	std::shared_ptr<IMethodDeclList> methodList;
};

class CStatementList : public IStatementList { // not implemented
public:
	CStatementList( IStatement* _stmt, IStatementList* _stmtList, const CPosition& pos ) :
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
	std::shared_ptr<IStatement> stmt;
	std::shared_ptr<IStatementList> stmtList;
	CPosition position;
};

class CVarDecl : public IVarDecl {
public:
	CVarDecl( IType* _type, const std::string& _identifier, const CPosition& _position ) :
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
	std::shared_ptr<IType> type;
	std::string identifier;
};

class CVarDeclList : public IClassDeclList {
public:
	CVarDeclList( IVarDeclList* _varDeclList, IVarDecl* _varDecl, const CPosition& pos ) :
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
	std::shared_ptr<IVarDeclList> varDeclList;
	std::shared_ptr<IVarDecl> varDecl;
	CPosition position;
};

class CMethodDecl : public IMethodDecl {
public:
	CMethodDecl(
		const std::string& _methodName,
		IFormalList* _formalList,
		IVarDeclList* _varList,
		CStatementList* _statementList,
		IExp* _returnExpr,
		const CPosition& _position ) :
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
	std::string methodName;
	std::shared_ptr<IFormalList> formalList;
	std::shared_ptr<IVarDeclList> varList;
	std::shared_ptr<IStatementList> statementList;
	std::shared_ptr<IExp> returnExpr;
	CPosition position;
};

class CMethodDeclList : public IMethodDeclList {
public:
	CMethodDeclList( IMethodDecl* _methodDecl, IMethodDeclList* _methodDeclList, const CPosition& pos ) :
		methodDeclList( _methodDeclList ),
		methodDecl( _methodDecl ),
		position( pos )
	{}

	const IVarDeclList* GetMethodDeclList( ) const
	{
		return methodDeclList.get();
	}

	const IVarDecl* GetMethodDecl( ) const
	{
		return methodDecl.get( );
	}
private:
	std::shared_ptr<IVarDeclList> methodDeclList;
	std::shared_ptr<IVarDecl> methodDecl;
	CPosition position;
};

class CFormalList : public IFormalList {
public:
	CFormalList( IType* _type, const std::string& _identifier, IFormalList* _formalList, const CPosition& pos ) :
		type( _type ),
		identifier( _identifier ),
		formalList( _formalList ),
		position( pos )
	{}

	const IFormalList* GetFormalRest() const
	{
		return formalList.get();
	}

	const IType* GetType() const
	{
		return type.get();
	}

	const std::string GetIdentifier() const
	{
		return identifier;
	}
private:
	std::shared_ptr<IType> type;
	std::string identifier;

	std::shared_ptr<IFormalList> formalList;

	CPosition position;
};

class CFormalRestList : public IFormalList {
public:
	CFormalRestList( IFormalList* _formalRest, IFormalList* _formalRestList, const CPosition& pos ) :
		formalRest( _formalRest ),
		formalRestList( _formalRestList ),
		position( pos )
	{}

	const IFormalList* GetFormalRest() const
	{
		return formalRest.get( );
	}

	const IFormalList* GetFormalRestList() const
	{
		return formalRestList.get();
	}
private:
	std::shared_ptr<IFormalList> formalRest;
	std::shared_ptr<IFormalList> formalRestList;
	CPosition position;
};

class CStatementListStatement : public IStatement {
public:
	CStatementListStatement( IStatementList* _statementList, const CPosition& _position ) :
		statementList( _statementList ),
		position( _position )
	{}

	const IStatementList* GetStatementList( ) const
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const override
	{
		visitor->Visit( this );
	}

private:
	std::shared_ptr<IStatementList> statementList;
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

	const IExp* GetExpression( ) const
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

	const IExp* GetRightPart( ) const
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

	const IExp* GetExp( ) const
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

	const IExp* GetExp( ) const
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
	std::shared_ptr<IExp> exp;
	std::shared_ptr<IExpList> expList;
	CPosition position;
};