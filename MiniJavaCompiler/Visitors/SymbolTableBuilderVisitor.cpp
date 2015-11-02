#include "SymbolTableBuilderVisitor.h"
#include <iostream>

void CSymbolTableBuilderVisitor::Visit( const CProgram* program )
{
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CMainClass* program )
{
	if( !symbolsTable->AddClass( program->GetClassName() ) ) {
		std::cout << "Class " + program->GetClassName() + " redefined";
	}
	curClass = symbolsTable->GetClass( program->GetClassName() );
	if( !curClass->AddMethod( "main", nullptr ) ) {
		std::cout << "Method main in class " + curClass->GetName() + " redefined";
	}
	curMethod = curClass->GetMethod( "main" );
	if( program->GetStatement() != nullptr ) {
		program->GetStatement()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclList* program )
{
	if( program->GetClassDecl() != nullptr ) {
		program->GetClassDecl()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CClassDecl* program )
{
	if( !symbolsTable->AddClass( program->GetName() ) ) {
		std::cout << "Class " + program->GetName() + " redefined";
	}
	curClass = symbolsTable->GetClass( program->GetName() );
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclDerived* program )
{
	if( !symbolsTable->AddClass( program->GetName() ) ) {
		std::cout << "Class " + program->GetName() + " redefined";
	}
	curClass = symbolsTable->GetClass( program->GetName() );
	SymbolsTable::CClassInfo* baseClass = symbolsTable->GetClass( program->GetBaseClassName() );
	for( auto method : baseClass->GetMethods() ) {
		curClass->AddMethod( method->GetName(), method->GetReturnType() );
	}

	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}
}

