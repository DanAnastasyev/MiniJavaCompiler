#include <iostream>
#include "Visitors/SymbolTableBuilderVisitor.h"
#include "Visitors/PrettyPrinterVisitor.h"
#include "Visitors/TypeCheckerVisitor.h"
extern int yyparse( std::shared_ptr<IProgram>& );

int main( int argc, char **argv )
{
	freopen( argv[1], "r", stdin );
	std::shared_ptr<IProgram> root;
	yyparse( root );
	std::shared_ptr<CPrettyPrinterVisitor> prettyPrinter( new CPrettyPrinterVisitor );
	root->Accept( prettyPrinter.get() );

	std::shared_ptr<CSymbolTableBuilderVisitor> symbolTableBuilder( new CSymbolTableBuilderVisitor );
	root->Accept( symbolTableBuilder.get() );

	if( !symbolTableBuilder->GetErrorStorage().GetAllErrors().empty() ) {
		for( auto ex : symbolTableBuilder->GetErrorStorage().GetAllErrors() ) {
			std::cout << ex << std::endl;
		}
		return 0;
	} 
	std::shared_ptr<CTypeCheckerVisitor> typeChecker( new CTypeCheckerVisitor( symbolTableBuilder->GetSymbolsTable() ) );
	root->Accept( typeChecker.get() );
	if( !symbolTableBuilder->GetErrorStorage().GetAllErrors().empty() ) {
		for( auto ex : symbolTableBuilder->GetErrorStorage().GetAllErrors() ) {
			std::cout << ex << std::endl;
		}
		return 0;
	}
	return 0;
}