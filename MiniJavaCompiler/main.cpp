#include <iostream>
#include "PrettyPrinterVisitor.h"
extern int yyparse( std::shared_ptr<IProgram>& );

int main( int argc, char **argv )
{
	freopen( argv[1], "r", stdin );
	std::shared_ptr<IProgram> root;
	yyparse( root );
	std::shared_ptr<CPrettyPrinterVisitor> prettyPrinter( new CPrettyPrinterVisitor );
	root->Accept( prettyPrinter.get() );
	return 0;
}