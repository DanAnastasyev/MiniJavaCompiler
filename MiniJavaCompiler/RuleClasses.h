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
	CProgram( CMainClass* _mainClass, CClassDeclList* _classList, CPosition& position)
		: mainClass( _mainClass ),
		classList( _classList ), 
		position( position )
	{}

	CMainClass* GetMainClass()
	{
		return mainClass.get();
	}

	CClassDeclList* GetClassDeclList()
	{
		return classList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}
private:
	std::shared_ptr<CMainClass> mainClass;
	std::shared_ptr<CClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(
		const std::string& _identifier,
		const std::string& _argv,
		IStatementList* _statementList,
		const CPosition& _position)
		: identifier( _identifier )
		, argv( _argv )
		, statementList( _statementList )
		, position( _position )
	{}

	IStatementList* GetStatementList()
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const override {
		visitor->Visit( this );
	}
private:
	std::string identifier;
	std::string argv;
	std::shared_ptr<IStatementList> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( CClassDecl* _declaration, CClassDeclList* _list, const CPosition& _positon ) :
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

	std::string GetName()
	{
		return className;
	}

	std::string GetBaseClassName()
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
	CStatementList()
	{}
private:
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

	std::string GetName()
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

	std::string GetName()
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
	std::string methodName;
	std::shared_ptr<IFormalList> formalList;
	std::shared_ptr<IVarDeclList> varList;
	std::shared_ptr<IStatementList> statementList;
	std::shared_ptr<IExp> returnExpr;
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
	enum StandardType { INT = 0, INT_ARRAY, BOOl };

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