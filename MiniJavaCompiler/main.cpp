#include <iostream>
#include <conio.h>

#include "Visitors/SymbolTableBuilderVisitor.h"
#include "Visitors/PrettyPrinterVisitor.h"
#include "Visitors/TypeCheckerVisitor.h"
#include "Visitors/IRBuilderVisitor.h"
#include "graphviz/IRTreeToDigraphConverter.h"
#include "Canon/Canon.h"
#include "Canon/TraceSchedule.h"

extern int yyparse( std::shared_ptr<IProgram>& );

void autoOpen( const std::string& prefix, const std::string& format = "png" )
{
	std::cout << prefix << std::endl;
	system( std::string( "..\\externals\\dot\\dot.exe -T" + format + " " + prefix + ".dot -o " + prefix + "." + format ).c_str() );
	if( getch() == 0x20 ) {
		system( (prefix + "." + format).c_str() );
	}
}

void flushLinearized(
	IRTree::CStmListPtr linearizedFrameStmList, 
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& canonFilename )
{
	auto tail = linearizedFrameStmList;
	for( auto stm = tail->GetHead();
		tail != nullptr;
		tail = tail->GetTail(),
		stm = (tail != nullptr) ? tail->GetHead() : nullptr ) 
	{
		stm->Accept( irTreeToDigraphConverter.get() );
	}
	irTreeToDigraphConverter->Flush( canonFilename + ".dot" );
}

void flushBlocks(
	std::shared_ptr<CBasicBlocks> blocksList,
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& canonFilename )
{
	auto tail = blocksList->GetBlocks();
	for( auto stmList = tail->GetHead();
		tail != nullptr;
		tail = tail->GetTail( ),
		stmList = ( tail != nullptr ) ? tail->GetHead( ) : nullptr )
	{
		for( auto stm = stmList->GetHead( );
			stmList != nullptr;
			stmList = stmList->GetTail( ),
			stm = ( stmList != nullptr ) ? stmList->GetHead( ) : nullptr ) 
		{
			stm->Accept( irTreeToDigraphConverter.get( ) );
		}
	}
	irTreeToDigraphConverter->Flush( canonFilename + ".dot" );
}

void flushTraced(
	std::shared_ptr<CBasicBlocks> blocks,
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& traceFilename )
{
	auto tail = blocks->GetBlocks();
	for( auto stmList = tail->GetHead();
		tail != nullptr;
		tail = tail->GetTail(),
		stmList = (tail != nullptr) ? tail->GetHead() : nullptr ) {
		for( auto stm = stmList->GetHead();
			stmList != nullptr;
			stmList = stmList->GetTail(),
			stm = (stmList != nullptr) ? stmList->GetHead() : nullptr ) {
			stm->Accept( irTreeToDigraphConverter.get() );
		}
	}
	irTreeToDigraphConverter->Flush( traceFilename + ".dot" );
}

int main( int argc, char **argv )
{
	if( argc <= 1 ) {
		return 1;
	}
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
	if( !typeChecker->GetErrorStorage().GetAllErrors().empty() ) {
		for( auto ex : typeChecker->GetErrorStorage().GetAllErrors() ) {
			std::cout << ex << std::endl;
		}
		return 0;
	}

	std::shared_ptr<CIRBuilderVisitor> irBuilder( new CIRBuilderVisitor( symbolTableBuilder->GetSymbolsTable() ) );
	root->Accept( irBuilder.get() );

	for( const auto& frame : irBuilder->GetFrames() ) {
		std::string format = "pdf";
		std::string tmpFilename = "output\\TMP_" + frame.GetName()->GetString();
		std::string irTreeFilename = "output\\IRTree_" + frame.GetName()->GetString();
		std::string canonFilename = "output\\Canon_" + frame.GetName()->GetString();
		std::string traceFilename = "output\\Trace_" + frame.GetName()->GetString();

		// Печатаем деревья для отдельной функции
		std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter( new IRTree::CIRTreeToDigraphConverter() );

		frame.GetRootStm()->Accept( irTreeToDigraphConverter.get() );
		irTreeToDigraphConverter->Flush( irTreeFilename + ".dot" );

		auto linearizedFrameStmList = CCanon::Linearize( frame.GetRootStm() );
		flushLinearized( linearizedFrameStmList, irTreeToDigraphConverter, canonFilename );

		auto blocks = std::make_shared<CBasicBlocks>( linearizedFrameStmList );
		flushBlocks( blocks, irTreeToDigraphConverter, tmpFilename );
		autoOpen( tmpFilename );

		//CTraceSchedule schedule( blocks );
		//flushTraced( blocks, irTreeToDigraphConverter, traceFilename );

		//autoOpen( irTreeFilename );
		//autoOpen( canonFilename );
		//autoOpen( traceFilename );
	}

	return 0;
}