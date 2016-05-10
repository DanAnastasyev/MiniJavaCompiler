#pragma once
#include "Frame/Frame.h"

namespace PrologEpilogBuilder {
	class IntermidInstructionBuilder {
	public:
		std::vector<std::string> AddProlog( const Frame::CFrame* frame ) const;
		std::vector<std::string> AddEpilog( const Frame::CFrame* frame ) const;
	};

}