#pragma once

#include <map>
#include <memory>

#include "SymbolsTable/Symbol.h"
#include "Frame/Temp.h"
#include "IRTree/Expressions.h"
#include "SymbolsTable/SymbolsTable.h"

namespace Assembler {
	class CBaseInstructionList;
}

namespace Frame {
	class IAccess;

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

		Assembler::CBaseInstructionList* GenerateCode( IRTree::CStmListPtr stmList ) const;

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


	// Интерфейс получения поддерева для переменной
	class IAccess {
	public:
		virtual const IRTree::IExprPtr GetExp( const Frame::CFrame& framePtr ) const = 0;
		virtual ~IAccess() {}
	};

	// Реализация для переменных на стеке
	class CInFrame : public IAccess {
	public:
		explicit CInFrame( int offset );

		const IRTree::IExprPtr GetExp( const Frame::CFrame& framePtr ) const;
	private:
		const int offset;
	};

	// Реализация для переменных в регистре
	//class CInReg : public IAccess {
	//public:
	//	explicit CInReg();

	//	const IRTree::IExprPtr GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const;
	//private:
	//	std::shared_ptr<Temp::CTemp> temp;
	//};

	// Реализация для переменных в регистре
	class CInObject : public IAccess {
	public:
		explicit CInObject( int _offset ) : offset( _offset ) {}

		const IRTree::IExprPtr GetExp( const Frame::CFrame& frameRegPtr ) const;
	private:
		const int offset;
	};

	// Реализация для переменных в регистре
	class CFormalParamInStack : public IAccess {
	public:
		explicit CFormalParamInStack( int _offset ) : offset( _offset ) {}

		const IRTree::IExprPtr GetExp( const Frame::CFrame& frameRegPtr ) const;
	private:
		// Порядковый номер аргумента, переданного как формальный параметр функции
		// Считать начинаем с 1
		// 0 резервируется для this который мы договорились передавать через стек
		const int offset;
	};
}
