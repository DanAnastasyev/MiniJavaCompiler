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
			EQ=8, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
		};

		virtual ~IExpr() {};

		virtual void Accept( IIRTreeVisitor* visitor ) const = 0;

		virtual CExprList* Kids();
		virtual IExpr* Build( CExprList* kids );
	};

	typedef std::shared_ptr<const IExpr> CExprPtr;

	// Const expression
	class CConst : public IExpr {
	public:
		CConst( int value );
		int GetValue() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		int value;
	};

	// Name for jumps
	class CName : public IExpr {
	public:
		CName( std::shared_ptr<const Temp::CLabel> name );
		std::shared_ptr<const Temp::CLabel> GetName() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		std::shared_ptr<const Temp::CLabel> name;
	};

	// Temprorary variable
	class CTemp : public IExpr {
	public:
		CTemp( const std::shared_ptr<Temp::CTemp> temp );
		std::shared_ptr<Temp::CTemp> GetTemp( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		std::shared_ptr<Temp::CTemp> temp;
	};

	// Binary operator
	class CBinop : public IExpr {
	public:
		CBinop( BINOP binop, CExprPtr left, CExprPtr right );

		BINOP GetBinOp() const;
		const CExprPtr GetLeft() const;
		const CExprPtr GetRight() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		BINOP binop;
		const CExprPtr left;
		const CExprPtr right;
	};

	// Reading memory with address returned by expression
	class CMem : public IExpr {
	public:
		CMem( CExprPtr mem );
		const CExprPtr GetMem() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		const CExprPtr mem;
	};


	// Method call with args 
	class CCall : public IExpr {
	public:
		CCall( CExprPtr funcName, const std::shared_ptr<const CExprList> arguments );

		const CSymbol* GetFunctionName() const;
		const std::shared_ptr<const CExprList> GetArguments() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		CExprPtr funcName;
		std::shared_ptr<const CExprList> arguments;
	};

	// Do statement than process and return expression
	class CESeq : public IExpr {
	public:
		CESeq( CStmPtr statement, CExprPtr expression );
		const CStmPtr GetStatement() const;
		const CExprPtr GetExpression() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() override;

		virtual IExpr* Build( CExprList* kids ) override;

	private:
		const CStmPtr statement;
		const CExprPtr expression;
	};

	class CExprList {
	public:
		CExprList( const CExprPtr head, std::shared_ptr<const CExprList> tail );

		std::shared_ptr<const IExpr> GetHead() const;
		std::shared_ptr<const CExprList> GetTail() const;
	private:
		std::shared_ptr<const IExpr> head;
		std::shared_ptr<const CExprList> tail;
	};
}