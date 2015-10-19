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

	void Accept( IVisitor* visitor ) const
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<CMainClass> mainClass;
	std::shared_ptr<CClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(const std::string& _identifier, const std::string& _argv, CStatementList* _statementList, CPosition& _position)
		: identifier( _identifier )
		, argv( _argv )
		, statementList( _statementList )
		, position( _position )
	{}

	CStatementList* GetStatementList()
	{
		return statementList.get();
	}

	void Accept( IVisitor* visitor ) const
	{
		visitor->Visit( this );
	}
private:
	std::string identifier;
	std::string argv;
	std::shared_ptr<CStatementList> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList( CClassDecl* _declaration, CClassDeclList* _list, CPosition& _positon ) :
		declaration( _declaration ),
		list( _list ),
		position( _positon )
	{}

	CClassDeclList* GetClassDeclList()
	{
		return list.get();
	}

	CClassDecl* GetClassDecl()
	{
		return declaration.get();
	}

	void Accept( IVisitor* visitor ) const
	{
		visitor->Visit( this );
	}
private:
	std::shared_ptr<CClassDeclList> list;
	std::shared_ptr<CClassDecl> declaration;
	CPosition position;
};

class CClassDecl : public IClassDecl {

};

class CStatementList : public IStatementList {
public:
	CStatementList()
	{}
private:
};