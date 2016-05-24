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
#include "Frame/PrologueEpilogueBuilder.h"
#include "FlowGraph.h"
#include "InterferenceGraph.h"


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

	std::vector<std::string> registers = { "eax", "ebx", "ecx", "edx", "edi", "esi" };

	for( auto& frame : irBuilder->GetFrames() ) {
		std::string format = "pdf";
		std::string blocksFilename = "output\\Blocks_" + frame.GetName()->GetString();
		std::string irTreeFilename = "output\\IRTree_" + frame.GetName()->GetString();
		std::string canonFilename = "output\\Canon_" + frame.GetName()->GetString();
		std::string traceFilename = "output\\Trace_" + frame.GetName()->GetString();
		std::string assemblerFilename = "output\\Assembler_" + frame.GetName()->GetString() + ".txt";

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

//		autoOpen( irTreeFilename );
//		autoOpen( canonFilename );
//		autoOpen( blocksFilename );
//		autoOpen( traceFilename );

		Assembler::CBaseInstructionList* asmList = frame.GenerateCode( schedule.GetStms( ) );
		AssemFlowGraph afg( asmList );

		std::map<const Temp::CTemp*, bool> onStack;

		CInterferenceGraph* inteferenceGraph = new CInterferenceGraph( afg.GetNodes( ), &afg, &frame, onStack );
		//inteferenceGraph->WriteGraph( "interference" + std::to_string( 0 ) + ".txt", false, 0 );
		int colorNum = 6;
		int step = 0;

		inteferenceGraph->SetColors( colorNum );
		//inteferenceGraph->WriteGraph( "interferenceColored" + std::to_string( 0 ) + "step" + std::to_string( step ) + ".txt", true, colorNum );

		while( !inteferenceGraph->IsColored( colorNum ) ) {
			step++;
			asmList = inteferenceGraph->UpdateInstructionList( asmList, colorNum, &frame, afg );
			onStack = inteferenceGraph->GetOnStack( );
			afg = AssemFlowGraph( asmList );
			inteferenceGraph = new CInterferenceGraph( afg.GetNodes( ), &afg, &frame, onStack );
			inteferenceGraph->SetColors( colorNum );
			//inteferenceGraph->WriteGraph( "interferenceColored" + std::to_string( 0 ) + "step" + std::to_string( step ) + ".txt", true, colorNum );
		}

		Assembler::CBaseInstructionList* instListhead = asmList;
		Assembler::CBaseInstructionList* prev = asmList;

		// убрали MOVE a <- a
		for( Assembler::CBaseInstructionList* curr = instListhead; curr != 0; curr = curr->tail ) {
			const Assembler::CMove* instr = dynamic_cast<const Assembler::CMove*>( curr->head );
			if( ( instr != 0 && inteferenceGraph->GetColorMap()[instr->DefinedVars()->Head().get()] == 
				inteferenceGraph->GetColorMap( )[instr->UsedVars( )->Head().get()] ) ) 
			{
				prev->tail = curr->tail;
			} else {
				prev = curr;
			}
		}

		asmList = instListhead;
		std::ofstream finalAsmCodeStream( "output\\Final_" + frame.GetName( )->GetString( ) + ".asm" );
		while( asmList != NULL ) {
			finalAsmCodeStream << asmList->head->Format( inteferenceGraph->GetColorStringsMap( ) );
			asmList = asmList->tail;
		}
		finalAsmCodeStream << std::endl;


		//flushAssemblerCode( asmList, assemblerFilename );
		//Assembler::CInterferenceGraph graph( asmList, registers );
		//PrologEpilogueBuilder::IntermidInstructionBuilder builder;
		//auto prologue = builder.AddPrologue( frame );
		//auto epilogue = builder.AddEpilogue( frame );

		//std::ofstream finalAsmCodeStream("output\\Final_" + frame.GetName()->GetString() + ".asm");
		//for( auto& cmd : prologue ) {
		//	finalAsmCodeStream << cmd << std::endl;
		//}
		//for( auto& cmd : graph.GetCode() ) {
		//	finalAsmCodeStream << cmd->Format( graph.GetColors() );
		//}
		//for( auto& cmd : epilogue ) {
		//	finalAsmCodeStream << cmd << std::endl;
		//}
	}

	return 0;
}