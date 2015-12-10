#pragma once

#include "SymbolsTable\Symbol.h"
#include "Frame\Temp.h"
#include "IRTree\Expressions.h"
#include "SymbolsTable\SymbolsTable.h"

#include <map>
#include <memory>

namespace Frame {
	// ��������� ��������� ��������� ��� ����������
	class IAccess {
	public:
		virtual const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const = 0;
		virtual ~IAccess() {}
	};

	// ���������� ��� ���������� �� �����
	class CInFrame : public IAccess {
	public:
		CInFrame( int offset );

		const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const;
	private:
		int offset;
	};

	// ���������� ��� ���������� � ��������
	class CInReg : public IAccess {
	public:
		CInReg();

		const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const;
	private:
		std::shared_ptr<Temp::CTemp> temp;
	};

	class CFrame {
	public:
		CFrame( const CSymbol* _name, int formalsCount, std::shared_ptr<const IRTree::IStm> _root );
		CFrame( const SymbolsTable::CClassInfo* classInfo, const SymbolsTable::CMethodInfo* methodInfo, const SymbolsTable::CTable* table );

		void AddStatements( std::shared_ptr<const IRTree::IStm> statements );
		bool ContainsFormal( const CSymbol* name ) const;
		void AddFormal( const CSymbol* name, std::shared_ptr<IAccess> formal );
		std::shared_ptr<IAccess> GetVar( const CSymbol* name ) const;

		std::shared_ptr<Temp::CTemp> GetFramePtr() const;
		std::shared_ptr<Temp::CTemp> GetThisPtr() const;
		std::shared_ptr<Temp::CTemp> GetReturnPtr() const;

		static const int WORD_SIZE = 4;
	private:
		std::map<const CSymbol*, std::shared_ptr<IAccess>> formals;
		std::map<const CSymbol*, std::shared_ptr<IAccess>> temporires;
		std::map<const CSymbol*, std::shared_ptr<IAccess>> locals;

		const CSymbol* frameName;

		std::shared_ptr<const IRTree::IStm> root;

		// ��������� �� ���� �����
		std::shared_ptr<Temp::CTemp> framePtr;
		// ��������� �� �����, � �������� ���������� �����
		std::shared_ptr<Temp::CTemp> thisPtr;
		// ��������� �� ������������ ��������
		std::shared_ptr<Temp::CTemp> returnPtr;
	};
}
