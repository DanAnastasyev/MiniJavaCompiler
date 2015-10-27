#include <iostream>
#include "PrettyPrinterVisitor.h"
extern int yyparse( std::shared_ptr<IProgram>& );

int main( int argc, char **argv )
{
	freopen( argv[1], "r", stdin );
	std::shared_ptr<IProgram> root;
	yyparse( root );
	root->Accept( new CPrettyPrinterVisitor );
	return 0;
}