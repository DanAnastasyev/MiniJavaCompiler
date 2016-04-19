#pragma once
#include <memory>

#include "Statements.h"
#include "Frame/Temp.h"

namespace IRTree {
	class IStm;

	class IExpr {
	public:
		enum BINOP {
			PLUS, MINUS, MUL, DIV, AND, OR, LESS, XOR
		};

		virtual ~IExpr()
		{
		};

		virtual void Accept( IIRTreeVisitor* visitor ) const = 0;

		virtual CExprList* Kids() const = 0;
		virtual IExpr* Build( const CExprList* kids ) const = 0;
	};

	// Const expression
	class CConst : public IExpr {
	public:
		CConst( int value );
		int GetValue() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		int value;
	};

	// Name for jumps
	class CName : public IExpr {
	public:
		CName( std::shared_ptr<const Temp::CLabel> name );
		std::shared_ptr<const Temp::CLabel> GetName() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		std::shared_ptr<const Temp::CLabel> name;
	};

	// Temprorary variable
	class CTemp : public IExpr {
	public:
		CTemp( const std::shared_ptr<Temp::CTemp> temp );
		std::shared_ptr<Temp::CTemp> GetTemp() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		std::shared_ptr<Temp::CTemp> temp;
	};

	// Binary operator
	class CBinop : public IExpr {
	public:
		CBinop( BINOP binop, std::shared_ptr<const IExpr> left, std::shared_ptr<const IExpr> right );

		int GetBinOp() const;
		std::shared_ptr<const IExpr> GetLeft() const;
		std::shared_ptr<const IExpr> GetRight() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		int binop;
		const std::shared_ptr<const IExpr> left;
		const std::shared_ptr<const IExpr> right;
	};

	// Reading memory with address returned by expression
	class CMem : public IExpr {
	public:
		CMem( std::shared_ptr<const IExpr> mem );
		std::shared_ptr<const IExpr> GetMem() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		const std::shared_ptr<const IExpr> mem;
	};

	// Method call with args 
	class CCall : public IExpr {
	public:
		CCall( std::shared_ptr<const IExpr> funcName, const std::shared_ptr<const CExprList> arguments );

		const CSymbol* GetFunctionName() const;
		const std::shared_ptr<const CExprList>& GetArguments() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		std::shared_ptr<const IExpr> funcName;
		std::shared_ptr<const CExprList> arguments;
	};

	// Do statement than process and return expression
	class CESeq : public IExpr {
	public:
		CESeq( IStmPtr statement, std::shared_ptr<const IExpr> expression );
		IStmPtr GetStatement() const;
		std::shared_ptr<const IExpr> GetExpression() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IExpr* Build( const CExprList* kids ) const override;

	private:
		const IStmPtr statement;
		const std::shared_ptr<const IExpr> expression;
	};

	class CExprList {
	public:
		CExprList( const std::shared_ptr<const IExpr> head, std::shared_ptr<const CExprList> tail );

		std::shared_ptr<const IExpr> GetHead() const;
		std::shared_ptr<const CExprList> GetTail() const;
	private:
		std::shared_ptr<const IExpr> head;
		std::shared_ptr<const CExprList> tail;
	};

	// typedefs:
	typedef std::shared_ptr<const IExpr> IExprPtr;
	typedef std::shared_ptr<const CConst> CConstPtr;
	typedef std::shared_ptr<const CName> CNamePtr;
	typedef std::shared_ptr<const CTemp> CTempPtr;
	typedef std::shared_ptr<const CBinop> CBinopPtr;
	typedef std::shared_ptr<const CMem> CMemPtr;
	typedef std::shared_ptr<const CCall> CCallPtr;
	typedef std::shared_ptr<const CESeq> CESeqPtr;
}