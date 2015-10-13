#pragma once
#include "grammar.h"

class IVisitor {
public:
	void Visit( const IProgram* program );
	void Visit( const IMainClass* program );
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