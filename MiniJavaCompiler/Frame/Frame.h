#pragma once

#include "SymbolsTable\Symbol.h"
#include "Frame\Temp.h"
#include "IRTree\Expressions.h"
#include "SymbolsTable\SymbolsTable.h"

#include <map>
#include <memory>

namespace Frame {
	// Интерфейс получения поддерева для переменной
	class IAccess {
	public:
		virtual const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const = 0;
		virtual ~IAccess() {}
	};

	// Реализация для переменных на стеке
	class CInFrame : public IAccess {
	public:
		CInFrame( int offset );

		const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const;
	private:
		int offset;
	};

	// Реализация для переменных в регистре
	class CInReg : public IAccess {
	public:
		CInReg();

		const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const;
	private:
		std::shared_ptr<Temp::CTemp> temp;
	};

	class CFrame {
	public:
		CFrame( const SymbolsTable::CClassInfo* classInfo, const SymbolsTable::CMethodInfo* methodInfo, const SymbolsTable::CTable* table );

		void SetStatements( std::shared_ptr<const IRTree::IStm> statements );
		std::shared_ptr<IAccess> GetVar( const CSymbol* name ) const;

		std::shared_ptr<Temp::CTemp> GetFramePtr() const;
		std::shared_ptr<Temp::CTemp> GetThisPtr() const;
		std::shared_ptr<Temp::CTemp> GetReturnPtr() const;

		const CSymbol* GetName() const;

		std::shared_ptr<const IRTree::IStm> GetRootStm() const;

		static const int WORD_SIZE = 4;
	private:
		std::map<std::string, std::shared_ptr<IAccess>> formals;
		std::map<std::string, std::shared_ptr<IAccess>> locals;

		const CSymbol* frameName;

		std::shared_ptr<const IRTree::IStm> root;

		// Указатель на этот фрейм
		std::shared_ptr<Temp::CTemp> framePtr;
		// Указатель на класс, у которого вызывается метод
		std::shared_ptr<Temp::CTemp> thisPtr;
		// Указатель на возвращаемое значение
		std::shared_ptr<Temp::CTemp> returnPtr;
	};
}
