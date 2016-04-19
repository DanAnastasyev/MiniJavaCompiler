#pragma once

#include <memory>

#include "SymbolsTable\Symbol.h"

namespace Temp {
	// ��������� ����������
	class CTemp {
	public:
		// ����� ���������� � ���������� ������
		CTemp();

		// ����� ���������� � �������� ������
		explicit CTemp( const CSymbol* _symbol );
		
		const CSymbol* GetName() const;
	private:
		// ������� ��� �������� ���������� ���
		static int nextUniqueId;
		const CSymbol* name;
	};

	// ����� - ����� �������� � ����
	class CLabel {
	public:
		// ������� ����� � ���������� ������
		CLabel();

		// ������� ����� � �������� ������
		explicit CLabel( const CSymbol* label );

		const CSymbol* GetName() const;

	private:
		static int nextUniqueId;
		const CSymbol* name;
	};

	class CLabelList {
	public:
		explicit CLabelList( std::shared_ptr<const CLabel> head, std::shared_ptr<const CLabelList> tail = nullptr );

		std::shared_ptr<const CLabel> Head() const;
		std::shared_ptr<const CLabelList> Tail() const;

	private:
		std::shared_ptr<const CLabel> head;
		std::shared_ptr<const CLabelList> tail;
	};
}