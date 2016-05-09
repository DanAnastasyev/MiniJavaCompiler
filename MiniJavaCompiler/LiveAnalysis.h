#pragma once

#include <Assembler/BaseInstruction.h>

#include <set>
#include <string>
#include <vector>
#include <map>


namespace Assembler {
	// ������� �����
	struct CNode {
		// �������� �����
		std::vector<int> in;
		// ��������� �����
		std::vector<int> out;
	};


	// ����
	class CGraph {
	public:
		explicit CGraph(int size);

		// �������� ����� � ���� �� ������� from � ������� to
		void AddEdge(int from, int to);

		// ���������� ������ � �����
		int Size() const;

		// �������� �������
		const CNode& GetNode(int index) const;

	protected:
		// ���� �����
		// ��� ���������� � ������ �������� ������ ������
		std::vector<CNode> nodes;
	};


	// ���� ������ ����������
	class CWorkFlowGraph : public CGraph {
	public:
		// �����������
		// ��������� �� ���� map ��� ���� - ��� ������� � �������� - ������ ������������ ������
		explicit CWorkFlowGraph(const std::list<const CBaseInstruction*>& asmFunction);

	private:
		// ������������ ����� ������� � ��������� �����
		std::map<std::string, int> labels;

		void buildLabelMap(const std::list<const CBaseInstruction*>& asmFunction);
		void addEdges(const std::list<const CBaseInstruction*>& asmFunction);
	};


	// ���� �������� live-in live-out (�� ������������� �� "Modern Compiler Implementation in Java 4 ed." ����� 10
	class CLiveInOutCalculator {
	public:
		// �����������
		// ��������� �� ���� map ��� ���� - ��� ������� � �������� - ������ ������������ ������
		explicit CLiveInOutCalculator(const std::list<const CBaseInstruction*>& asmFunction);

		// �������� ������ live-in ���������� ��� ��������� �������/����������
		const std::set<std::string>& GetLiveIn(int nodeIndex) const;

		// �������� ������ live-out ���������� ��� ��������� �������/����������
		const std::set<std::string>& GetLiveOut(int nodeIndex) const;

		// �������� ������ ����������� ���������� ��� ��������� �������/����������
		const std::set<std::string>& GetDefines(int nodeIndex) const;

		const std::set<std::string>& GetUses(int nodeIndex) const;

	private:
		// ���� ������ ����������
		CWorkFlowGraph workflow;

		// ��������� live-in � live-out ���������� ��� ������ �� ������ �����
		std::vector<std::set<std::string>> liveIn;
		std::vector<std::set<std::string>> liveOut;

		// ��������� defines � uses
		std::vector<std::set<std::string>> defines;
		std::vector<std::set<std::string>> uses;

		// ������ � ������������� ���������
		std::vector<const CBaseInstruction*> commands;

		bool theSame(const std::set<std::string>& x, const std::set<std::string>& y) const;
		void buildCommands(const std::list<const CBaseInstruction*>& asmFunction);
		void buildDefines(const std::list<const CBaseInstruction*>& asmFunction);
		void buildUses(const std::list<const CBaseInstruction*>& asmFunction);
	};
} // namespace Assembler