#pragma once

#include "Header.h"

class CProgram;
class CMainClass;

class IVisitor {
public:
	void Visit( const CProgram* program );
	void Visit( const CMainClass* program );
	//void Visit( const IClassDeclList* program );
	//void Visit( const IClassDecl* program );
	//void Visit( const IVarDeclList* program );
	//void Visit( const IVarDecl* program );
	//void Visit( const IMethodDeclList* program );
	//void Visit( const IMethodDecl* program );
	//void Visit( const IFormalList* program );
	//void Visit( const IType* program );
	//void Visit( const IStatementList* program );
	//void Visit( const IStatement* program );
	//void Visit( const IExpList* program );
	//void Visit( const IExp* program );
};