#include <iostream>

#include "Visitor.h"

class CPrettyPrinterVisitor : public IVisitor {
	void Visit( const IProgram* program )
	{
		std::cout << "Program" << std::endl;
	}

	void Visit( const IMainClass* program )
	{
		std::cout << "Main Class" << std::endl;
	} 
};