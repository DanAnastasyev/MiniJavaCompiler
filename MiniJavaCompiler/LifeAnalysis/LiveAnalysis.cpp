#include "LiveAnalysis.h"
#include <assert.h>


namespace Assembler {

	CGraph::CGraph( int size ) : nodes( size ) {}


	void CGraph::AddEdge( int from, int to )
	{
		assert( from < int( nodes.size() ) );
		assert( to < int( nodes.size() ) );

		nodes[from].out.push_back( to );
		nodes[to].in.push_back( from );
	}


	int CGraph::Size() const
	{
		return nodes.size();
	}


	const CNode& CGraph::GetNode( int index ) const
	{
		return nodes[index];
	}

	CWorkFlowGraph::CWorkFlowGraph( const std::list<const CBaseInstruction*>& asmFunction ) : CGraph( asmFunction.size() )
	{
		buildLabelMap( asmFunction );
		addEdges( asmFunction );
	}

	void CWorkFlowGraph::buildLabelMap( const std::list<const CBaseInstruction*>& asmFunction )
	{
		int nodeIndex = 0;
		for( auto cmd : asmFunction ) {
			const CLabel* label = dynamic_cast<const CLabel*>( cmd );
			if( label != nullptr ) {
				assert( label->JumpTargets() != nullptr );
				assert( label->JumpTargets()->Head() != nullptr );
				labels.insert( std::make_pair( label->JumpTargets()->Head()->GetName()->GetString(), nodeIndex ) );
			}
			++nodeIndex;
		}
	}


	void CWorkFlowGraph::addEdges( const std::list<const CBaseInstruction*>& asmFunction )
	{
		int nodeIndex = 0;
		for( auto cmd : asmFunction ) {
			const COper* oper = dynamic_cast<const COper*>( cmd );
			if( oper != nullptr  &&  oper->JumpTargets() != nullptr  &&  oper->JumpTargets()->Head() != nullptr ) {
				AddEdge( nodeIndex, labels[oper->JumpTargets()->Head()->GetName()->GetString()] );
			}
			if( nodeIndex + 1 < Size() ) {
				AddEdge( nodeIndex, nodeIndex + 1 );
			}
			nodeIndex++;
		}
	}


	namespace {
		class CTopSort {
		public:
			static std::vector<int> topSort( const CGraph& graph, int start )
			{
				res.clear();
				marked.clear();
				marked.resize( graph.Size(), false );

				dfs( graph, start );

				std::reverse( res.begin(), res.end() );
				return res;
			}

		private:
			static std::vector<int> res;
			static std::vector<char> marked;

			static void dfs( const CGraph& graph, int nodeIndex )
			{
				marked[nodeIndex] = true;
				auto node = graph.GetNode( nodeIndex );

				for( auto neighbour : node.out ) {
					if( !marked[neighbour] ) {
						dfs( graph, neighbour );
					}
				}

				res.push_back( nodeIndex );
			}
		};

		std::vector<int> CTopSort::res = std::vector<int>();
		std::vector<char> CTopSort::marked = std::vector<char>();
	}

	CLiveInOutCalculator::CLiveInOutCalculator( const std::list<const CBaseInstruction*>& asmFunction ) :
		workflow( asmFunction ), liveIn( workflow.Size() ), liveOut( workflow.Size() )
	{
		bool setsChanged = true;
		int mainFuncIndex = 0;
		std::vector<int> revTopsort = CTopSort::topSort( workflow, mainFuncIndex );
		buildCommands( asmFunction );
		buildDefines( asmFunction );
		buildUses( asmFunction );

		std::reverse( revTopsort.begin(), revTopsort.end() );
		while( setsChanged ) {
			setsChanged = false;
			for( auto nodeIndex : revTopsort ) {
				std::set<std::string> newLiveIn = liveOut[nodeIndex];
				auto currList = commands[nodeIndex]->DefindedVars();
				while( currList != nullptr  &&  currList->Head() != nullptr ) {
					auto inSet = newLiveIn.find( currList->Head()->GetName()->GetString() );
					if( inSet != newLiveIn.end() ) {
						newLiveIn.erase( inSet );
					}
					currList = currList->Tail();
				}
				currList = commands[nodeIndex]->UsedVars();
				while( currList != nullptr  &&  currList->Head() != nullptr ) {
					newLiveIn.insert( currList->Head()->GetName()->GetString() );
					currList = currList->Tail();
				}
				std::set<std::string> newLiveOut;
				for( auto succ : workflow.GetNode( nodeIndex ).out ) {
					for( auto var : liveIn[succ] ) {
						newLiveOut.insert( var );
					}
				}
				if( !theSame( newLiveIn, liveIn[nodeIndex] ) || !theSame( newLiveOut, liveOut[nodeIndex] ) ) {
					setsChanged = true;
					liveIn[nodeIndex] = newLiveIn;
					liveOut[nodeIndex] = newLiveOut;
				}
			}
		}
	}


	const std::set<std::string>& CLiveInOutCalculator::GetLiveIn( int nodeIndex ) const
	{
		assert( nodeIndex < int( liveIn.size() ) );

		return liveIn[nodeIndex];
	}


	const std::set<std::string>& CLiveInOutCalculator::GetLiveOut( int nodeIndex ) const
	{
		assert( nodeIndex < int( liveOut.size() ) );

		return liveOut[nodeIndex];
	}


	const std::set<std::string>& CLiveInOutCalculator::GetDefines( int nodeIndex ) const
	{
		assert( nodeIndex < int( defines.size() ) );

		return defines[nodeIndex];
	}


	const std::set<std::string>& CLiveInOutCalculator::GetUses( int nodeIndex ) const
	{
		assert( nodeIndex < int( uses.size() ) );

		return uses[nodeIndex];
	}



	bool CLiveInOutCalculator::theSame( const std::set<std::string>& x, const std::set<std::string>& y ) const
	{
		if( x.size() != y.size() ) {
			return false;
		}

		auto xIt = x.begin();
		auto yIt = y.begin();

		while( xIt != x.end() ) {
			if( *xIt != *yIt ) {
				return false;
			}
			++xIt;
			++yIt;
		}

		return true;
	}


	void CLiveInOutCalculator::buildCommands( const std::list<const CBaseInstruction*>& asmFunction )
	{
		commands.clear();
		std::copy( asmFunction.begin(), asmFunction.end(), commands.begin() );
	}


	void CLiveInOutCalculator::buildDefines( const std::list<const CBaseInstruction*>& asmFunction )
	{
		defines.resize( asmFunction.size() );
		int cmdIndex = 0;
		for( auto cmd : asmFunction ) {
			auto def = cmd->DefindedVars();
			while( def != nullptr  &&  def->Head() != nullptr ) {
				defines[cmdIndex].insert( def->Head()->GetName()->GetString() );
				def = def->Tail();
			}
			++cmdIndex;
		}
	}


	void CLiveInOutCalculator::buildUses( const std::list<const CBaseInstruction*>& asmFunction )
	{
		uses.resize( asmFunction.size() );
		int cmdIndex = 0;
		for( auto cmd : asmFunction ) {
			auto def = cmd->UsedVars();
			while( def != nullptr && def->Head() != nullptr ) {
				uses[cmdIndex].insert( def->Head()->GetName()->GetString() );
				def = def->Tail();
			}
			++cmdIndex;
		}
	}
}