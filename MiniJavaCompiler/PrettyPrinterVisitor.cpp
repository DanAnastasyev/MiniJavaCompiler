#include <iostream>

#include "Visitor.h"
#include "Header.h"

class CPrettyPrinterVisitor : public IVisitor {
	void Visit( CProgram* program )
	{
		program->GetMainClass()->Accept( this );
		//program->GetClassDeclList()->Accept( this );
	}

	void Visit( CMainClass* program )
	{
		
	} 
};