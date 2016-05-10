#include "Frame/PrologueEpilogueBuilder.h"

#include "Frame/Frame.h"

namespace PrologEpilogueBuilder {
	using namespace Assembler;

	std::vector<std::string> IntermidInstructionBuilder::AddPrologue( const Frame::CFrame& frame ) const
	{
		std::vector<std::string> prologIList;
		prologIList.push_back( "; prologue begin" );
		prologIList.push_back( frame.GetName()->GetString() + ":" );
		for( int i = 1; i < frame.Registers().size(); ++i ) {
			prologIList.push_back( "push " + frame.Registers()[i] );
		}
		
		int espShift = Frame::CFrame::WORD_SIZE * frame.GetLocalCount();
		prologIList.push_back( "sub esp, " + std::to_string( espShift ) );
		prologIList.push_back( "; prologue end" );
		return prologIList;
	}

	std::vector<std::string> IntermidInstructionBuilder::AddEpilogue( const Frame::CFrame& frame ) const
	{
		std::vector<std::string> epilogIList;
		epilogIList.push_back( "; epilogue begin" );
		int espShift = Frame::CFrame::WORD_SIZE * frame.GetLocalCount();

		epilogIList.push_back( "mov esp, ebp" );
		epilogIList.push_back( "add esp, " + std::to_string( frame.Registers().size() * Frame::CFrame::WORD_SIZE ) );
		for( int i = frame.Registers().size() - 1; i > 0; --i ) {
			epilogIList.push_back( "pop " + frame.Registers()[i] );
		}
		epilogIList.push_back( "ret" );
		epilogIList.push_back( "; epilogue end" );
		return epilogIList;
	}
}