#include "FlowGraph.h"
#include "Assembler/BaseInstruction.h"

const std::set<const Temp::CTemp*> AssemFlowGraph::GetDefSet( const CNode* node )
{
	auto varList = nodeToInstructionTable[node]->DefinedVars();
	std::set<const Temp::CTemp*> result;
	for( ; varList != 0; varList = varList->Tail( ) ) {
		result.insert( varList->Head().get() );
	}
	return result;
}

const std::set<const Temp::CTemp*> AssemFlowGraph::GetUseSet( const CNode* node )
{
	auto varList = nodeToInstructionTable[node]->UsedVars( );
	std::set<const Temp::CTemp*> result;
	for( ; varList != 0; varList = varList->Tail() ) {
		result.insert( varList->Head().get() );
	}
	return result;
}

bool AssemFlowGraph::IsMove( const CNode* node )
{
	const Assembler::CMove* move = dynamic_cast <const Assembler::CMove*> ( nodeToInstructionTable[node] );
	return ( move != 0 );
}

AssemFlowGraph::AssemFlowGraph( Assembler::CBaseInstructionList* instructions )
{
	// Составим словарь с возможностью получать по лейблу соответствующую инструкцию.
	const Assembler::CBaseInstruction* currentInstr = 0;
	for( const Assembler::CBaseInstructionList* p = instructions; p != 0; p = p->tail ) {
		currentInstr = p->head;
		const Assembler::CLabel* label = dynamic_cast<const Assembler::CLabel*>( currentInstr );
		if( label != 0 ) {
			labelToInstructionTable.insert( std::pair<const Temp::CLabel*, const Assembler::CBaseInstruction*>( label->JumpTargets()->Head().get(), currentInstr ) );
		}
	}
	CNode* prevNode = 0;
	CNode* currentNode = 0;
	// Добавляем в граф вершины по одной и сразу проводим рёбра от предыдущей вершины к следующей, если у неё 
	// пуст список JumpTargets.
	for( const Assembler::CBaseInstructionList* p = instructions; p != 0; p = p->tail ) {
		currentInstr = p->head;
		prevNode = currentNode;
		currentNode = new CNode( this );
		nodeToInstructionTable.insert( std::pair<const CNode*, const Assembler::CBaseInstruction*>( currentNode, currentInstr ) );
		instructionToNodeTable.insert( std::pair<const Assembler::CBaseInstruction*, CNode*>( currentInstr, currentNode ) );
		if( prevNode != 0 ) {
			const Assembler::CBaseInstruction* prevInstr = nodeToInstructionTable[prevNode];
			if( prevInstr->JumpTargets() == 0 ) {
				AddEdge( prevNode, currentNode );
			}
		}
	}
	// Теперь добавим в граф все рёбра в вершины из списков JumpTargets для каждой инструкции.
	for( const Assembler::CBaseInstructionList* p = instructions; p != 0; p = p->tail ) {
		currentInstr = p->head;
		CNode* currentNode = instructionToNodeTable[currentInstr];
		auto jt = currentInstr->JumpTargets();
		if( jt != 0 ) {
			for( ; jt != 0; jt = jt->Tail() ) {
				auto currentLabel = jt->Head();
				const Assembler::CBaseInstruction* instrOut = labelToInstructionTable[currentLabel.get()];
				if( instrOut != nullptr ) {
					CNode* nodeOut = instructionToNodeTable[instrOut];
					AddEdge( currentNode, nodeOut );
				}
			}
		}
	}
	// Граф построен.
	
	CNodeList* nodeList = this->GetNodes();
	CNodeList* revNodeList = nodeList->Reverse();
	SetNodes(revNodeList);
	int i = 0;
	for( CNodeList* n = revNodeList; n != 0; n = n->GetNext() ) {
		liveIn[n->GetNode()] = this->GetUseSet( n->GetNode() );
	}
	bool isAnythingChanged = true;
	while( isAnythingChanged ) {
		isAnythingChanged = false;
		for( CNodeList* p = revNodeList; p != 0; p = p->GetNext() ) {
			const CNode* currentNode = p->GetNode();
			CNodeList* predList = currentNode->GetPreds();
			for( CNodeList* pp = predList; pp != 0; pp = pp->GetNext() ) {
				const CNode* currentPred = pp->GetNode();
				for( std::set<const Temp::CTemp*>::iterator it = liveIn[currentNode].begin(); it != liveIn[currentNode].end(); it++ ) {
					if( liveOut[currentPred].insert( *it ).second ) {
						isAnythingChanged = true;
					}
				}
			}
			for( std::set<const Temp::CTemp*>::iterator it = liveOut[currentNode].begin(); it != liveOut[currentNode].end(); it++ ) {
				std::set<const Temp::CTemp*> currentDef = GetDefSet( currentNode );
				if( currentDef.find( *it ) == currentDef.end() ) {
					if( liveIn[currentNode].insert( *it ).second ) {
						isAnythingChanged = true;
					}
				}
			}
		}
	}
	nodeList = revNodeList->Reverse();
	SetNodes(nodeList);
}

const Assembler::CBaseInstruction* AssemFlowGraph::GetInstruction( const CNode* node )
{
	return nodeToInstructionTable[node];
}

std::set<const Temp::CTemp*> AssemFlowGraph::GetLiveIn( const CNode* node )
{
	return liveIn[node];
}

std::set<const Temp::CTemp*> AssemFlowGraph::GetLiveOut( const CNode* node )
{
	return liveOut[node];
}