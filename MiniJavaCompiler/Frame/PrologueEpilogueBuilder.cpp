#include "Frame/PrologueEpilogueBuilder.h"

#include "Frame/Frame.h"

namespace PrologEpilogBuilder {
	using namespace Assembler;

	std::vector<std::string> IntermidInstructionBuilder::AddProlog( const Frame::CFrame* frame ) const
	{
		std::vector<std::string> prologIList;
		prologIList.push_back( "; prolog begin" );
		prologIList.push_back( frame->GetName()->GetString() + ":" );
		prologIList.push_back( "PUSH EBP" );
		prologIList.push_back( "MOV EBP, ESP" );
		
		int espShift = frame->WORD_SIZE * frame->GetLocalCount();
		prologIList.push_back( "SUB esp, " + std::to_string( espShift ) );
		for( const auto& regName : frame->Registers() ) {
			prologIList.push_back( "PUSH " + regName );
		}
		prologIList.push_back( "; prolog end" );
		return prologIList;
	}

	std::vector<std::string> IntermidInstructionBuilder::AddEpilog( const Frame::CFrame* frame ) const
	{
		std::vector<std::string> epilogIList;
		std::vector<const std::string> regs;
		for( auto it = frame->Registers().rbegin(); it != frame->Registers().rend(); ++it ) {
			regs.push_back( *it );
		}
		epilogIList.push_back( "; epilog begin" );
		int espShift = frame->WORD_SIZE * frame->GetLocalCount();
		for( const auto& item : regs ) {
			epilogIList.push_back( "pop " + item );
		}

		epilogIList.push_back( "ADD ESP, " + std::to_string( espShift ) );
		epilogIList.push_back( "POP EBP" );
		epilogIList.push_back( "RET" );
		epilogIList.push_back( "; epilog end" );
		return epilogIList;
	}
}