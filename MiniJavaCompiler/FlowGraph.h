#pragma once

#include "Graph.h"
#include <map>
#include <set>
#include "Frame/Temp.h"

namespace Assembler {
	class CBaseInstruction;
	class CBaseInstructionList;
}

class IFlowGraph : public CGraph {
public:
	virtual ~IFlowGraph() {}

	virtual const std::set<const Temp::CTemp*> GetDefSet( const CNode* node ) = 0;
	virtual const std::set<const Temp::CTemp*> GetUseSet( const CNode* node ) = 0;
	virtual bool IsMove( const CNode* node ) = 0;
};

class AssemFlowGraph: public IFlowGraph {
public:
	AssemFlowGraph( Assembler::CBaseInstructionList* instructions );

	virtual const std::set<const Temp::CTemp*> GetDefSet( const CNode* node );
	virtual const std::set<const Temp::CTemp*> GetUseSet( const CNode* node );
	virtual bool IsMove( const CNode* node );

	const Assembler::CBaseInstruction* GetInstruction( const CNode* node );
	std::set<const Temp::CTemp*> GetLiveIn( const CNode* node );
	std::set<const Temp::CTemp*> GetLiveOut( const CNode* node );
private:
	std::map<const Temp::CLabel*, const Assembler::CBaseInstruction*> labelToInstructionTable;
	std::map<const CNode*, const Assembler::CBaseInstruction*> nodeToInstructionTable;
	std::map<const Assembler::CBaseInstruction*, CNode*> instructionToNodeTable;

	std::map<const CNode*, std::set<const Temp::CTemp*> > liveIn;
	std::map<const CNode*, std::set<const Temp::CTemp*> > liveOut;
};