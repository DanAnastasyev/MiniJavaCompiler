#pragma once
#include "Frame/Frame.h"

namespace PrologEpilogueBuilder {
	class IntermidInstructionBuilder {
	public:
		std::vector<std::string> AddPrologue( const Frame::CFrame& frame ) const;
		std::vector<std::string> AddEpilogue( const Frame::CFrame& frame ) const;
	};

}