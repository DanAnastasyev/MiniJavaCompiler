#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Statements.h"
#include "Frame/Temp.h"

namespace IRTree
{
	class IStm;

	class IExpr {
	public:
		enum BINOP {
			PLUS, MINUS, MUL, DIV, AND, OR, LESS, XOR
		};
		enum CJUMP {
			EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
		};

		virtual ~IExpr() = 0;
	};

	typedef std::shared_ptr<const IExpr> CExprPtr;

	class CConst : public IExpr {
	public:
		CConst( int value );
		int GetValue() const;

	private:
		int value;
	};

	class CName : public IExpr {
	public:
		CName( const std::string& name );
		std::string GetName() const;

	private:
		std::string name;
	};

	class CTemp : public IExpr {
	public:
		CTemp( const std::shared_ptr<Temp::CTemp> temp );
		Temp::CTemp GetTemp() const;

	private:
		std::shared_ptr<Temp::CTemp> temp;
	};

	class CBinop : public IExpr {
	public:
		CBinop( BINOP binop, CExprPtr left, CExprPtr right );
		const CExprPtr GetLeft() const;
		const CExprPtr GetRight() const;

	private:
		BINOP binop;
		const CExprPtr left;
		const CExprPtr right;
	};

	class CMem : public IExpr {
	public:
		CMem( CExprPtr mem );
		const CExprPtr GetMem() const;

	private:
		const CExprPtr mem;
	};

	class CCall : public IExpr {
	public:
		CCall( const CSymbol* funcName, const std::vector<CExprPtr>& arguments );

		const CSymbol* GetFunctionName() const;
		std::vector<CExprPtr> GetArguments() const;
	private:
		const CSymbol* funcName;
		std::vector<CExprPtr> arguments;
	};

	class CESeq : public IExpr {
	public:
		CESeq( CStmPtr statement, CExprPtr expression );
		const CStmPtr GetStatement() const;
		const CExprPtr GetExpression() const;

	private:
		const CStmPtr statement;
		const CExprPtr expression;
	};
}