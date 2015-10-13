#pragma once
#include <string>
#include <memory>

#include "grammar.h"
#include "Position.h"

class CProgram : public IProgram {
public:
	CProgram(std::shared_ptr<IMainClass> mainClass, std::shared_ptr<IClassDeclList> classList, CPosition& position)
		: mainClass( mainClass )
		, classList( classList )
		, position( position )
	{}
private:
	std::shared_ptr<IMainClass> mainClass;
	std::shared_ptr<IClassDeclList> classList;
	CPosition position;
};

class CMainClass : public IMainClass {
public:
	CMainClass(const std::string& id, const std::string& argv, std::shared_ptr<IStatementList> statements, CPosition& position)
		: id( id )
		, argv( argv )
		, statementList( statementList )
		, position( position )
	{}
private:
	std::string id;
	std::string argv;
	std::shared_ptr<IStatementList> statementList;
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