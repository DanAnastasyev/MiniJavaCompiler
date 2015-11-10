#include <iostream>
#include "SymbolTableBuilderVisitor.h"

CSymbolTableBuilderVisitor::CSymbolTableBuilderVisitor()
{
	symbolsTable = std::make_shared<SymbolsTable::CTable>();
}

SymbolsTable::CTable* CSymbolTableBuilderVisitor::GetSymbolsTable()
{
	return symbolsTable.get();
}

void CSymbolTableBuilderVisitor::Visit( const CProgram* program )
{
	std::cout << std::endl << "TableBuilder:" << std::endl;
	if( program->GetMainClass() != nullptr ) {
		program->GetMainClass()->Accept( this );
	}
	if( program->GetClassDeclList() != nullptr ) {
		program->GetClassDeclList()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CMainClass* program )
{
	if( !symbolsTable->AddClass( program->GetClassName()->GetString() ) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CMainClass. " ) +
			program->GetClassName()->GetString() + " redefined. " +
			"Line " + std::to_string( program->GetPosition( ).GetBeginPos( ).first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	}
	curClass = symbolsTable->GetClass( program->GetClassName()->GetString() );
	
	if( !curClass->AddMethod( "main", "" ) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CMainClass. " ) +
			"Method main in class " + curClass->GetName() + " redefined. "
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else if( isDebug ) {
		std::cout << program->GetClassName()->GetString() << "::main(String[])" << std::endl;
	}
	
	curMethod = curClass->GetMethod( "main" );
	if( program->GetStatement() != nullptr ) {
		program->GetStatement()->Accept( this );
	}

	curMethod = nullptr;
	curClass = nullptr;
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
	if( !symbolsTable->AddClass( program->GetName()->GetString()) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CClassDecl. " ) +
			program->GetName()->GetString() + " redefined." +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else if( isDebug ) {
		std::cout << program->GetName()->GetString() << std::endl;
	}

	curClass = symbolsTable->GetClass( program->GetName()->GetString() );

	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}

	curClass = nullptr;
}

void CSymbolTableBuilderVisitor::Visit( const CClassDeclDerived* program )
{
	if( !symbolsTable->AddClass( program->GetName()->GetString()) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CClassDeclDerived. " ) +
			"Class " + program->GetName()->GetString() + " redefined" + ". " +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else if( isDebug ) {
		std::cout << program->GetName()->GetString() << " extends " << program->GetBaseClassName()->GetString() << std::endl;
	}

	curClass = symbolsTable->GetClass( program->GetName()->GetString() );

	// Переносим все методы и переменные из базового класса в наследника
	curClass->SetBaseClass( symbolsTable->GetClass( program->GetBaseClassName()->GetString() ) );

	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetMethodDeclList() != nullptr ) {
		program->GetMethodDeclList()->Accept( this );
	}

	curClass = nullptr;
}

void CSymbolTableBuilderVisitor::Visit( const CVarDecl* program )
{
	program->GetType()->Accept( this );
	std::string type = lastTypeValue;
	std::string id = program->GetName()->GetString();

	if( curClass == nullptr ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CVarDecl. " ) +
			"Var " + id + " is defined out of scope" + ". " +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else if( curMethod == nullptr ) {
		if( !curClass->AddVar( id, type ) ) {
			errorStorage.PutError( std::string( "[Table builder] Node type - CVarDecl. " ) +
				"Var " + id + " is already defined in " + curClass->GetName() + ". " +
				"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
				", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
		} else if( isDebug ) {
			std::cout << curClass->GetName() << "::" << id << std::endl;
		}
	} else if( !curMethod->AddLocalVar( id, type ) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CVarDecl. " ) +
			"Var " + id + " is already defined in " + curClass->GetName() + "::" + curMethod->GetName() + ". " +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else if( isDebug ) {
		std::cout << curClass->GetName() << "::" << curMethod->GetName() << "::" << id << std::endl;
	}
}

void CSymbolTableBuilderVisitor::Visit( const CVarDeclList* program )
{
	if( program->GetVarDeclList() != nullptr ) {
		program->GetVarDeclList()->Accept( this );
	}
	if( program->GetVarDecl() != nullptr ) {
		program->GetVarDecl()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CFormalParam* list )
{
	list->GetType()->Accept( this );
	std::string type = lastTypeValue;
	std::string id = list->GetIdentifier()->GetString();

	if( curMethod == nullptr ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CFormalParam. " ) +
			"Var " + id + " is defined out of scope" + ". " +
			"Line " + std::to_string( list->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( list->GetPosition().GetBeginPos().second ) + "." );
	} else if( !curMethod->AddParamVar( id, type ) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CFormalParam. " ) +
			"Var " + id + " is already defined in " + curMethod->GetName() + ". " +
			"Line " + std::to_string( list->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( list->GetPosition().GetBeginPos().second ) + "." );
	} else if( isDebug ) {
		std::cout << curMethod->GetName() << " param: " << id << std::endl;
	}
}

void CSymbolTableBuilderVisitor::Visit( const CFormalList* list )
{
	auto params = list->GetParamList();
	for( int i = params.size() - 1; i >= 0; --i ) {
		params[i].get()->Accept( this );
	}
}

void CSymbolTableBuilderVisitor::Visit( const CMethodDecl* program )
{
	program->GetType()->Accept( this );
	std::string returnType = lastTypeValue;

	if( curClass == nullptr ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CMethodDecl. " ) +
			"Method " + program->GetName()->GetString() + " is defined out of scope" + ". " +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
		return;
	}
	if( curClass->GetBaseClass() != nullptr ) {
		for( auto method : curClass->GetBaseClass()->GetMethods() ) {
			if( method->GetName() == program->GetName()->GetString() ) {
				errorStorage.PutError( std::string( "[Table builder] Node type - CMethodDecl. " ) +
					"Method " + program->GetName()->GetString() + " already defined in base class " + program->GetName()->GetString() + ". " +
					"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
					", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
				return;
			}
		}
	}
	if( !curClass->AddMethod( program->GetName()->GetString(), returnType ) ) {
		errorStorage.PutError( std::string( "[Table builder] Node type - CMethodDecl. " ) +
			"Method " + program->GetName( )->GetString( ) + " is already defined in class " + curClass->GetName() + ". " +
			"Line " + std::to_string( program->GetPosition().GetBeginPos().first ) +
			", column " + std::to_string( program->GetPosition().GetBeginPos().second ) + "." );
	} else {
		if( isDebug ) {
			std::cout << curClass->GetName() << "::" << program->GetName()->GetString() << std::endl;
		}

		curMethod = curClass->GetMethod( program->GetName()->GetString() );
		if( program->GetFormalList() != nullptr ) {
			program->GetFormalList()->Accept( this );
		}
		if( program->GetVarList() != nullptr ) {
			program->GetVarList()->Accept( this );
		}
		curMethod = nullptr;
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
	switch( program->GetType() ) {
	case CStandardType::StandardType::INT:
		lastTypeValue = ".INT";
		break;
	case CStandardType::StandardType::BOOL:
		lastTypeValue = ".BOOL";
		break;
	case CStandardType::StandardType::INT_ARRAY:
		lastTypeValue = ".INT_ARRAY";
	default:
		break;
	}
}

void CSymbolTableBuilderVisitor::Visit( const CUserType* program )
{
	lastTypeValue = program->GetTypeName()->GetString();
}

void CSymbolTableBuilderVisitor::Visit( const CIndexExpression* expr )
{
	lastTypeValue = ".INT";
}

const CErrorStorage& CSymbolTableBuilderVisitor::GetErrorStorage() const
{
	return errorStorage;
}
