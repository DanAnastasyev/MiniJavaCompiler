#pragma once

#include <string>
#include "SymbolsTable\Symbol.h"

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

		const CSymbol* Name() const
		{
			return name;
		}

	private:
		static int nextUniqueId;
		const CSymbol* name;
	};
}