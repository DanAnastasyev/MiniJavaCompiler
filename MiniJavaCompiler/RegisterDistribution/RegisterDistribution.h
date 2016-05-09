// Автор: Федюнин Валерий
// Набор классов для распределения регистров по переменным

#pragma once

#include <LifeAnalysis\LiveAnalysis.h>

#include <stack>
#include <set>


namespace Assembler {
	class CVariableNode {
	public:
		CVariableNode() {
			Color = -1;
			InStack = false;
		}
		int Color;
		bool InStack;
	};


	enum TEdgeType {
		ET_NoEdge = 0, ET_MoveEdge, ET_Edge
	};

	// Граф взаимосвязанности переменных.
	class CInterferenceGraph {
	public:
		explicit CInterferenceGraph(const std::list<const CBaseInstruction*>& asmFunction, const std::vector<const std::string>& registers);

		const std::list<const CBaseInstruction*>& GetCode() const;
		
		std::map<std::string, std::string> GetColors();

	private:
		// код функции
		std::list<const CBaseInstruction*> asmFunction;

		// таблица инцидентности
		std::vector<std::vector<TEdgeType>> edges;

		// узлы графа
		std::vector<CVariableNode> nodes;

		// соответствие между именами переменных и вершинами графа
		std::map<std::string, int> nodeMap;

		// граф с подсчитанными live-in live-out
		CLiveInOutCalculator liveInOut;

		// список доступных регистров
		std::vector<const std::string> registers;

		// стек убранных вершин
		std::stack<int> pulledNodes;

		// непокрашенные вершины
		std::set<int> uncoloredNodes;

		void addNode(const std::string& name);
		void addMoveEdge(const std::string& from, const std::string& to);
		void addEdge(const std::string& from, const std::string& to);

		bool paint();
		void regenerateCode();

		void addRegisterColors();
		bool hasNonColoredNonStackedNodes() const;
		int getColorableNode() const;
		int getMaxInterferingNode() const;
		int getNeighbourNum(int nodeIndex) const;
	};

} 