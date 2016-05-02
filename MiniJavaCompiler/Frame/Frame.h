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

		// ��������� �� ���� �����
		std::shared_ptr<Temp::CTemp> framePtr;
		// ��������� �� �����, � �������� ���������� �����
		std::shared_ptr<Temp::CTemp> thisPtr;
		// ��������� �� ������������ ��������
		std::shared_ptr<Temp::CTemp> returnPtr;
	};


	// ��������� ��������� ��������� ��� ����������
	class IAccess {
	public:
		virtual const IRTree::IExprPtr GetExp( const Frame::CFrame& framePtr ) const = 0;
		virtual ~IAccess() {}
	};

	// ���������� ��� ���������� �� �����
	class CInFrame : public IAccess {
	public:
		explicit CInFrame( int offset );

		const IRTree::IExprPtr GetExp( const Frame::CFrame& framePtr ) const;
	private:
		const int offset;
	};

	// ���������� ��� ���������� � ��������
	//class CInReg : public IAccess {
	//public:
	//	explicit CInReg();

	//	const IRTree::IExprPtr GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const;
	//private:
	//	std::shared_ptr<Temp::CTemp> temp;
	//};

	// ���������� ��� ���������� � ��������
	class CInObject : public IAccess {
	public:
		explicit CInObject( int _offset ) : offset( _offset ) {}

		const IRTree::IExprPtr GetExp( const Frame::CFrame& frameRegPtr ) const;
	private:
		const int offset;
	};

	// ���������� ��� ���������� � ��������
	class CFormalParamInStack : public IAccess {
	public:
		explicit CFormalParamInStack( int _offset ) : offset( _offset ) {}

		const IRTree::IExprPtr GetExp( const Frame::CFrame& frameRegPtr ) const;
	private:
		// ���������� ����� ���������, ����������� ��� ���������� �������� �������
		// ������� �������� � 1
		// 0 ������������� ��� this ������� �� ������������ ���������� ����� ����
		const int offset;
	};
}
