#include <iostream>
#include <Windows.h>

#include "Visitors/SymbolTableBuilderVisitor.h"
#include "Visitors/PrettyPrinterVisitor.h"
#include "Visitors/TypeCheckerVisitor.h"
#include "Visitors/IRBuilderVisitor.h"
#include "graphviz/IRTreeToDigraphConverter.h"
#include "Canon/Canon.h"

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
	if( !typeChecker->GetErrorStorage().GetAllErrors().empty() ) {
		for( auto ex : typeChecker->GetErrorStorage().GetAllErrors() ) {
			std::cout << ex << std::endl;
		}
		return 0;
	}

	std::shared_ptr<CIRBuilderVisitor> irBuilder( new CIRBuilderVisitor( symbolTableBuilder->GetSymbolsTable() ) );
	root->Accept( irBuilder.get() );

	for (const auto& frame : irBuilder->GetFrames()) {
		std::string name = frame.GetName()->GetString();
		std::wstring outputFilename =
			std::wstring( L"output/IRTree_" ) +
			std::wstring( name.begin(), name.end() ) +
			std::wstring( L".dot" );

		// �������� ������� ��� ��������� �������
		std::shared_ptr<IRTree::CIRTreeToDigraphConverter> irTreeToDigraphConverter(
			new IRTree::CIRTreeToDigraphConverter(outputFilename));
		
		//frame.GetRootStm()->Accept(irTreeToDigraphConverter.get());
		//irTreeToDigraphConverter->Flush();

		auto linearizedFrameStmList = CCanon::Linearize( frame.GetRootStm() );
		for( auto linearizedFrameStm = linearizedFrameStmList->GetHead(); 
			linearizedFrameStmList != nullptr; 
			linearizedFrameStmList = linearizedFrameStmList->GetTail(), 
			linearizedFrameStm = ( linearizedFrameStmList != nullptr ) ? linearizedFrameStmList->GetHead() : nullptr ) 
		{
			linearizedFrameStm->Accept( irTreeToDigraphConverter.get() );
		}
		irTreeToDigraphConverter->Flush();

		std::wstring transformedName =
			std::wstring( L"output/IRTRee_" ) +
			std::wstring( name.begin(), name.end() ) +
			std::wstring( L".png" );
		ShellExecute( nullptr, L"cmd.exe", (std::wstring( L"dot -Tpng " ) + outputFilename + L" " + transformedName).c_str(),
			nullptr, L".", SW_SHOW );
		ShellExecute( nullptr, L"open", transformedName.c_str(), nullptr, L".", SW_SHOW );
	}
	
	return 0;
}