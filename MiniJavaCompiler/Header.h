#pragma once
#include <string>
#include <memory>

#include "grammar.h"
#include "Position.h"
#include "Visitor.h"

class CMainClass;
class CClassDeclList;

class CProgram : public IProgram {
public:
	CProgram( CMainClass* mainClass, CClassDeclList* classList, CPosition& position)
		: mainClass( std::shared_ptr<CMainClass>( mainClass ) ),
		classList( std::shared_ptr<CClassDeclList>( classList ) ), 
		position( position )
	{}

	std::shared_ptr<CMainClass> GetMainClass()
	{
		return mainClass;
	}

	std::shared_ptr<CClassDeclList> GetClassDeclList()
	{
		return classList;
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
	CMainClass(const std::string& id, const std::string& argv, std::shared_ptr<CStatementList> statements, CPosition& position)
		: id( id )
		, argv( argv )
		, statementList( statementList )
		, position( position )
	{}

	void Accept( IVisitor* visitor ) const
	{
		visitor->Visit( this );
	}
private:
	std::string id;
	std::string argv;
	std::shared_ptr<CStatementList> statementList;
	CPosition position;
}; 

class CClassDeclList : public IClassDeclList {
public:
	CClassDeclList()
	{}
private:
};

class CStatementList : public IStatementList {
public:
	CStatementList()
	{}
private:
};