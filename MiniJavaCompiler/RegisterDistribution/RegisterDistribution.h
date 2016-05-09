#pragma once

#include <LifeAnalysis\LiveAnalysis.h>

#include <stack>
#include <set>


namespace Assembler {
	class CVariableNode {
	public:
		CVariableNode() 
		{
			Color = -1;
			InStack = false;
		}
		int Color;
		bool InStack;
	};


	enum TEdgeType {
		ET_NoEdge = 0, ET_MoveEdge, ET_Edge
	};

	class CInterferenceGraph {
	public:
		explicit CInterferenceGraph( const std::list<const CBaseInstruction*>& asmFunction, const std::vector<std::string>& registers );

		const std::list<const CBaseInstruction*>& GetCode() const;

		std::map<std::string, std::string> GetColors();

	private:
		std::list<const CBaseInstruction*> asmFunction;
		std::vector<std::vector<TEdgeType>> edges;
		std::vector<CVariableNode> nodes;
		std::map<std::string, int> nodeMap;
		CLiveInOutCalculator liveInOut;
		std::vector<std::string> registers;
		std::stack<int> pulledNodes;
		std::set<int> uncoloredNodes;

		void addNode( const std::string& name );
		void addMoveEdge( const std::string& from, const std::string& to );
		void addEdge( const std::string& from, const std::string& to );

		bool paint();
		void regenerateCode();

		void addRegisterColors();
		bool hasNonColoredNonStackedNodes() const;
		int getColorableNode() const;
		int getMaxInterferingNode() const;
		int getNeighbourNum( int nodeIndex ) const;
	};
}