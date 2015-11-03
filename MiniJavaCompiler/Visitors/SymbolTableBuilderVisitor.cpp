#include "SymbolTableBuilderVisitor.h"
#include <iostream>

void CSymbolTableBuilderVisitor::Visit( const CProgram* program )
{
	symbolsTable = new SymbolsTable::CTable();

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
		std::cout << "Class " + program->GetClassName() + " redefined" << std::endl;
	}
	curClass = symbolsTable->GetClass( program->GetClassName() );
	if( !curClass->AddMethod( "main", nullptr ) ) {
		std::cout << "Method main in class " + curClass->GetName() + " redefined" << std::endl;
	} else if( isDebug ) {
		std::cout << program->GetClassName() << " main method was added" << std::endl;
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
		std::cout << "Class " + program->GetName() + " redefined" << std::endl;
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
		std::cout << "Class " + program->GetName() + " redefined" << std::endl;
	}
	curClass = symbolsTable->GetClass( program->GetName() );
	// Переносим все методы и переменные из базового класса в наследника
	SymbolsTable::CClassInfo* baseClass = symbolsTable->GetClass( program->GetBaseClassName() );
	for( auto method : baseClass->GetMethods() ) {
		method->GetReturnType()->GetType()->Accept( this );
		curClass->AddMethod( method->GetName(), lastTypeValue.get() );
	}
	for( auto var : baseClass->GerVars() ) {
		var->GetType()->Accept( this );
		curClass->AddVar( var->GetName(), lastTypeValue.get() );
	}

	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CVarDecl* program )
{
	program->GetType()->Accept( this );
	IType* type = lastTypeValue.get();
	std::string id = program->GetName();

	if( curClass == nullptr ) {
		std::cout << "Var " + id + " is defined out of scope" << std::endl;
	} else if( curMethod == nullptr ) {
		if( !curClass->AddVar( id, type ) ) {
			std::cout << "Var " + id + " is already defined in " + curClass->GetName() << std::endl;
		}
	} else if( !curMethod->AddLocalVar( id, type ) ) {
		std::cout << "Var " + id + " is already defined in " + curMethod->GetName() << std::endl;
	}
}

void CSymbolTableBuilderVisitor::Visit( const CVarDeclList* program )
{
	if( program->GetVarDecl() != nullptr ) {
		program->GetVarDecl()->Accept( this );
	}
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CFormalList* list )
{
	list->GetType()->Accept( this );
	IType* type = lastTypeValue.get();
	std::string id = list->GetIdentifier();

	if( curMethod == nullptr ) {
		std::cout << "Var " + id + " is defined out of scope" << std::endl;
	} else if( !curMethod->AddLocalVar( id, type ) ) {
		std::cout << "Var " + id + " is already defined in " + curMethod->GetName() << std::endl;
	}

	if( list->GetFormalRest() != nullptr ) {
		list->GetFormalRest()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CFormalRestList* list )
{
	if( list->GetFormalRest() != nullptr ) {
		list->GetFormalRest()->Accept( this );
	}
	if( list->GetFormalRestList() != nullptr ) {
		list->GetFormalRestList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDecl* program )
{
	program->GetType()->Accept( this );
	IType* returnType = lastTypeValue.get();

	if( curClass == nullptr ) {
		std::cout << "Method " << program->GetName() + " is defined out of scope" << std::endl;
	} else if( !curClass->AddMethod( program->GetName(), returnType ) ) {
		std::cout << "Method " << program->GetName() + " is already defined in class " << curClass->GetName() << std::endl;
	} else {
		curMethod = curClass->GetMethod( program->GetName() );
		if( program->GetFormalList() != nullptr ) {
			program->GetFormalList()->Accept( this );
		}
		if( program->GetVarList() != nullptr ) {
			program->GetVarList()->Accept( this );
		}
	}
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDeclList* methodList )
{
	if( methodList->GetMethodDecl() != nullptr ) {
		methodList->GetMethodDecl()->Accept( this );
	}
	if( methodList->GetMethodDeclList() != nullptr ) {
		methodList->GetMethodDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CStandardType* program )
{
	lastTypeValue = std::make_shared<CStandardType>( program );
}

void CSymbolTableBuilderVisitor::Visit( const CUserType* program )
{
	lastTypeValue = std::make_shared<CUserType>( program );
}

