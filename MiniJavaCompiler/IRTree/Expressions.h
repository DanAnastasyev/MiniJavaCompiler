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
			PLUS, MINUS, MUL, DIV, AND, OR, LSHIFT, RSHIFT, ARSHIFT, XOR
		};
		enum CJUMP {
			EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
		};

		virtual ~IExpr() = 0;
	};

	typedef std::shared_ptr<IExpr> CExprPtr;

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
		CTemp( Temp::CTemp temp );
		Temp::CTemp& GetTemp() const;

	private:
		Temp::CTemp temp;
	};

	class CBinop : public IExpr {
	public:
		CBinop( const IExpr* left, BINOP binop, const IExpr* right );
		const CExprPtr GetLeft() const;
		const CExprPtr GetRight() const;

	private:
		BINOP binop;
		const CExprPtr left;
		const CExprPtr right;
	};

	class CMem : public IExpr {
	public:
		CMem( const IExpr* mem );
		const CExprPtr GetMem() const;

	private:
		const CExprPtr mem;
	};

	class CCall : public IExpr {
	public:
		CCall( const IExpr* returnExpr, const std::vector<IStm*> aguments );
		const CExprPtr GetReturnExpr() const;
		const std::vector<const CExprPtr>&  GetArguments();

	private:
		const CExprPtr returnExp;
		std::vector<const CExprPtr> arguments;
	};

	class CEseq : public IExpr {
	public:
		CEseq( IStm* statement, IExpr* expression );
		const CStmPtr GetStatement() const;
		const CExprPtr GetExpression() const;

	private:
		const CStmPtr statement;
		const CExprPtr expression;
	};
}