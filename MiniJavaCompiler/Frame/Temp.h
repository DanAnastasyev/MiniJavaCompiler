#pragma once

#include <string>
#include "SymbolsTable\Symbol.h"

namespace Temp {
	// ��������� ����������
	class CTemp {
	public:
		// ����� ���������� � ���������� ������
		CTemp();

		// ����� ���������� � �������� ������
		explicit CTemp( const CSymbol& _symbol );
		
		const std::string& GetName() const;
	private:
		// ������� ��� �������� ���������� ���
		static int nextUniqueId;
		std::string name;
	};

	// ����� - ����� �������� � ����
	class CLabel {
	public:
		// ������� ����� � ���������� ������
		CLabel();

		// ������� ����� � �������� ������
		explicit CLabel( const CSymbol* label );

		const CSymbol* Name() const
		{
			return name;
		}

	private:
		static int nextUniqueId;
		const CSymbol* name;
	};
}