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
		CInReg( int offset );

		const IRTree::CExprPtr GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const;
	private:
		int offset;
	};

	class CFrame {
	public:
		CFrame( const std::string& _name );
		CFrame( const SymbolsTable::CClassInfo& classInfo, const SymbolsTable::CMethodInfo& methodInfo, const SymbolsTable::CTable& table );

		bool ContainsFormal( const std::string& name ) const;
		void AddFormal( const std::string& name, std::shared_ptr<IAccess> formal );
		std::shared_ptr<IAccess> GetFormal( const std::string& name ) const;

		std::shared_ptr<Temp::CTemp> GetFramePtr() const;
		std::shared_ptr<Temp::CTemp> GetThisPtr() const;
		std::shared_ptr<Temp::CTemp> GetReturnPtr() const;

		static const int WORD_SIZE = 4;
	private:
		std::map<std::string, std::shared_ptr<IAccess>> formals;

		std::string frameName;

		// ��������� �� ���� �����
		std::shared_ptr<Temp::CTemp> framePtr;
		// ��������� �� �����, � �������� ���������� �����
		std::shared_ptr<Temp::CTemp> thisPtr;
		// ��������� �� ������������ ��������
		std::shared_ptr<Temp::CTemp> returnPtr;
	};
}
