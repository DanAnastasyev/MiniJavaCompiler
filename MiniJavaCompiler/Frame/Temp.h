#pragma once

#include <memory>

#include <SymbolsTable/Symbol.h>
#include <memory>

namespace Temp {
	// Временная переменная
	class CTemp {
	public:
		// Новая переменная с уникальным именем
		CTemp();

		// Новая переменная с заданным именем
		explicit CTemp( const CSymbol* _symbol );
		
		const CSymbol* GetName() const;
	private:
		// Счётчик для создания уникальных имён
		static int nextUniqueId;
		const CSymbol* name;
	};

	// Метка - точка перехода в коде
	class CLabel {
	public:
		// Создать метку с уникальным именем
		CLabel();

		// Создать метку с заданным именем
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