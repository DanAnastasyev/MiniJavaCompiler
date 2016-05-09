#pragma once

#include <Assembler/BaseInstruction.h>

#include <set>
#include <string>
#include <vector>
#include <map>


namespace Assembler {
	// Вершина графа
	struct CNode {
		// входящие ребра
		std::vector<int> in;
		// исходящие ребра
		std::vector<int> out;
	};


	// Граф
	class CGraph {
	public:
		explicit CGraph(int size);

		// добавить ребро в граф из вершины from в вершину to
		void AddEdge(int from, int to);

		// Количество вершин в графе
		int Size() const;

		// получить вершину
		const CNode& GetNode(int index) const;

	protected:
		// узлы графа
		// вся информация о ребрах хранится внутри вершин
		std::vector<CNode> nodes;
	};


	// Граф потока управления
	class CWorkFlowGraph : public CGraph {
	public:
		// Конструктор
		// принимает на вход map где ключ - имя функции а значение - список ассемблерных команд
		explicit CWorkFlowGraph(const std::list<const CBaseInstruction*>& asmFunction);

	private:
		// соответствие между метками и вершинами графа
		std::map<std::string, int> labels;

		void buildLabelMap(const std::list<const CBaseInstruction*>& asmFunction);
		void addEdges(const std::list<const CBaseInstruction*>& asmFunction);
	};


	// Граф подсчета live-in live-out (за подробностями см "Modern Compiler Implementation in Java 4 ed." глава 10
	class CLiveInOutCalculator {
	public:
		// Конструктор
		// принимает на вход map где ключ - имя функции а значение - список ассемблерных команд
		explicit CLiveInOutCalculator(const std::list<const CBaseInstruction*>& asmFunction);

		// получить список live-in переменных для некоторой вершины/инструкции
		const std::set<std::string>& GetLiveIn(int nodeIndex) const;

		// получить список live-out переменных для некоторой вершины/инструкции
		const std::set<std::string>& GetLiveOut(int nodeIndex) const;

		// получить список объявляемых переменных для некоторой вершины/инструкции
		const std::set<std::string>& GetDefines(int nodeIndex) const;

		const std::set<std::string>& GetUses(int nodeIndex) const;

	private:
		// граф потока управления
		CWorkFlowGraph workflow;

		// множества live-in и live-out переменных для каждой из вершин графа
		std::vector<std::set<std::string>> liveIn;
		std::vector<std::set<std::string>> liveOut;

		// множества defines и uses
		std::vector<std::set<std::string>> defines;
		std::vector<std::set<std::string>> uses;

		// вектор с ассемблерными командами
		std::vector<const CBaseInstruction*> commands;

		bool theSame(const std::set<std::string>& x, const std::set<std::string>& y) const;
		void buildCommands(const std::list<const CBaseInstruction*>& asmFunction);
		void buildDefines(const std::list<const CBaseInstruction*>& asmFunction);
		void buildUses(const std::list<const CBaseInstruction*>& asmFunction);
	};
} // namespace Assembler