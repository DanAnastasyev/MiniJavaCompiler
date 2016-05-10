#include "Frame/PrologueEpilogueBuilder.h"

#include "Frame/Frame.h"

namespace PrologEpilogueBuilder {
	using namespace Assembler;

	std::vector<std::string> IntermidInstructionBuilder::AddPrologue( const Frame::CFrame& frame ) const
	{
		std::vector<std::string> prologIList;
		prologIList.push_back( "; prologue begin" );
		prologIList.push_back( frame.GetName()->GetString() + ":" );
		prologIList.push_back( "push ebp" );
		prologIList.push_back( "mov ebp, esp" );
		
		int espShift = Frame::CFrame::WORD_SIZE * frame.GetLocalCount();
		prologIList.push_back( "sub esp, " + std::to_string( espShift ) );
		for( const auto& regName : frame.Registers() ) {
			prologIList.push_back( "push " + regName );
		}
		prologIList.push_back( "; prologue end" );
		return prologIList;
	}

	std::vector<std::string> IntermidInstructionBuilder::AddEpilogue( const Frame::CFrame& frame ) const
	{
		std::vector<std::string> epilogIList;
		std::vector<const std::string> regs;
		for( auto it = frame.Registers().rbegin(); it != frame.Registers().rend(); ++it ) {
			regs.push_back( *it );
		}
		epilogIList.push_back( "; epilogue begin" );
		int espShift = Frame::CFrame::WORD_SIZE * frame.GetLocalCount();
		for( const auto& item : regs ) {
			epilogIList.push_back( "pop " + item );
		}

		epilogIList.push_back( "add esp, " + std::to_string( espShift ) );
		epilogIList.push_back( "pop ebp" );
		epilogIList.push_back( "ret" );
		epilogIList.push_back( "; epilogue end" );
		return epilogIList;
	}
}