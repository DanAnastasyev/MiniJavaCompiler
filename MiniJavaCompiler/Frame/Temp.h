#pragma once

#include <memory>

#include <SymbolsTable/Symbol.h>
#include <memory>

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

	struct ITempMap {
		std::string TempMap( std::shared_ptr<const CTemp> t ) const;
	};

	class CTempList {
	public:
		CTempList( std::shared_ptr<const CTemp> _head, std::shared_ptr<const CTempList> _tail ) :
			tail( _tail ), head( _head )
		{}

	private:
		std::shared_ptr<const CTemp> head;
		std::shared_ptr<const CTempList> tail;
	};
	typedef std::shared_ptr<const Temp::CTempList> CTempListPtr;

	class CLabelList {
	public:
		CLabelList( std::shared_ptr<const CLabel> _head, std::shared_ptr<const CLabelList> _tail ) :
			tail( _tail ), head( _head )
		{}
		std::shared_ptr<const CLabel> Head( ) const;
		std::shared_ptr<const CLabelList> Tail( ) const;
	private:
		std::shared_ptr<const CLabel> head;
		std::shared_ptr<const CLabelList> tail;
	};
	typedef std::shared_ptr<const Temp::CLabelList> CLabelListPtr;
}