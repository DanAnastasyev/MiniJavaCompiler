#include <iostream>
#include <conio.h>
#include <fstream>

#include "Visitors/SymbolTableBuilderVisitor.h"
#include "Visitors/PrettyPrinterVisitor.h"
#include "Visitors/TypeCheckerVisitor.h"
#include "Visitors/IRBuilderVisitor.h"
#include "graphviz/IRTreeToDigraphConverter.h"
#include "Canon/Canon.h"
#include "Canon/TraceSchedule.h"
#include "Assembler/BaseInstruction.h"
#include "RegisterDistribution/RegisterDistribution.h"


extern int yyparse( std::shared_ptr<IProgram>& );

void autoOpen( const std::string& prefix, const std::string& format = "png" )
{
	std::cout << prefix << std::endl;
	system( std::string( "..\\externals\\dot\\dot.exe -T" + format + " " + prefix + ".dot -o " + prefix + "." + format ).c_str( ) );
	if( getch( ) == 0x20 ) {
		system( ( prefix + "." + format ).c_str( ) );
	}
}

void flushLinearized(
	IRTree::CStmListPtr linearizedFrameStmList,
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& canonFilename )
{
	auto tail = linearizedFrameStmList;
	for( auto stm = tail->GetHead( );
		tail != nullptr;
		tail = tail->GetTail( ),
		stm = ( tail != nullptr ) ? tail->GetHead( ) : nullptr ) {
		stm->Accept( irTreeToDigraphConverter.get( ) );
	}
	irTreeToDigraphConverter->Flush( canonFilename + ".dot" );
}

void flushBlocks(
	std::shared_ptr<CBasicBlocks> blocksList,
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& canonFilename )
{
	auto tail = blocksList->GetBlocks( );
	for( auto stmList = tail->GetHead( );
		tail != nullptr;
		tail = tail->GetTail( ),
		stmList = ( tail != nullptr ) ? tail->GetHead( ) : nullptr ) {
		for( auto stm = stmList->GetHead( );
			stmList != nullptr;
			stmList = stmList->GetTail( ),
			stm = ( stmList != nullptr ) ? stmList->GetHead( ) : nullptr ) {
			stm->Accept( irTreeToDigraphConverter.get( ) );
		}
	}
	irTreeToDigraphConverter->Flush( canonFilename + ".dot" );
}

void flushTrace(
	IRTree::CStmListPtr trace,
	std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter,
	const std::string& traceFilename )
{
	auto tail = trace;
	for( auto stm = tail->GetHead( );
		tail != nullptr;
		tail = tail->GetTail( ),
		stm = ( tail != nullptr ) ? tail->GetHead( ) : nullptr ) {
		stm->Accept( irTreeToDigraphConverter.get( ) );
	}
	irTreeToDigraphConverter->Flush( traceFilename + ".dot" );
}

void flushAssemblerCode( const std::list<const Assembler::CBaseInstruction*>& instructionList, const std::string& fileName )
{
	std::ofstream file( fileName );
	for( auto instr : instructionList ) {
		file << instr->GetAssemblerInstruction();
	}
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

	std::vector<std::string> registers;
	for( int i = 0; i < 6; ++i ) {
		registers.push_back( "r" + std::to_string( i ) );
	}

	for( auto& frame : irBuilder->GetFrames() ) {
		std::string format = "pdf";
		std::string blocksFilename = "output\\Blocks_" + frame.GetName()->GetString();
		std::string irTreeFilename = "output\\IRTree_" + frame.GetName()->GetString();
		std::string canonFilename = "output\\Canon_" + frame.GetName()->GetString();
		std::string traceFilename = "output\\Trace_" + frame.GetName()->GetString();
		std::string assemblerFilename = "output\\Assembler_" + frame.GetName()->GetString();

		// Печатаем деревья для отдельной функции
		std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter( new IRTree::CIRTreeToDigraphConverter() );

		frame.GetRootStm()->Accept( irTreeToDigraphConverter.get() );
		irTreeToDigraphConverter->Flush( irTreeFilename + ".dot" );

		auto linearizedFrameStmList = CCanon::Linearize( frame.GetRootStm() );
		flushLinearized( linearizedFrameStmList, irTreeToDigraphConverter, canonFilename );

		auto blocks = std::make_shared<CBasicBlocks>( linearizedFrameStmList );
		flushBlocks( blocks, irTreeToDigraphConverter, blocksFilename );

		CTraceSchedule schedule( blocks );
		flushTrace( schedule.GetStms(), irTreeToDigraphConverter, traceFilename );

		std::list<const Assembler::CBaseInstruction*> asmList = frame.GenerateCode( schedule.GetStms() );
		flushAssemblerCode( asmList, assemblerFilename );
		Assembler::CInterferenceGraph graph( asmList, registers );

		for( auto& cmd : graph.GetCode() ) {
			std::cout << cmd->Format( graph.GetColors( ) );
		}

		//autoOpen( irTreeFilename );
		//autoOpen( canonFilename );
		//autoOpen( blocksFilename );
		//autoOpen( traceFilename );
	}

	return 0;
}