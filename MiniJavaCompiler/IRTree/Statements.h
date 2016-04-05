#pragma once
#include <vector>
#include <memory>

#include "Frame/Temp.h"
#include "Visitors/Visitor.h"

namespace IRTree
{
	class IExpr;
	class CExprList;

	class IStm {
	public:
		virtual ~IStm() {}

		virtual void Accept( IIRTreeVisitor* visitor ) const = 0;

		virtual CExprList* Kids() const = 0;
		virtual IStm* Build(const CExprList* kids) const = 0;
	};

	// Copy result src into dst
	class CMove : public IStm {
	public:
		CMove( std::shared_ptr<const IExpr> dst, std::shared_ptr<const IExpr> src );

		std::shared_ptr<const IExpr> GetDestExpr() const;
		std::shared_ptr<const IExpr> GetSrcExpr() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const IExpr> destExpr;
		std::shared_ptr<const IExpr> srcExpr;
	};

	// Calculate exp and forget about result
	class CExpr : public IStm {
	public:
		CExpr( std::shared_ptr<const IExpr> expr );

		std::shared_ptr<const IExpr> GetExp( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const IExpr> expr;
	};

	// Jump to label
	class CJump : public IStm {
	public:
		CJump( std::shared_ptr<const IExpr> exp, const std::vector<std::shared_ptr<const Temp::CLabel>>& labels );
		CJump( std::shared_ptr<const Temp::CLabel> label );

		std::shared_ptr<const IExpr> GetJumpExpr() const;
		std::vector<std::shared_ptr<const Temp::CLabel>> GetLabels() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const IExpr> jmpExpr;
		std::vector<std::shared_ptr<const Temp::CLabel>> labels;
	};

	// Jump with condition
	class CCondJump : public IStm {
	public:
		CCondJump( std::shared_ptr<const IExpr> expr,
			std::shared_ptr<const Temp::CLabel> ifTrueLabel, std::shared_ptr<const Temp::CLabel> ifFalseLabel );

		std::shared_ptr<const IExpr> GetExpr() const;
		std::shared_ptr<const Temp::CLabel> GetIfTrueLabel( ) const;
		std::shared_ptr<const Temp::CLabel> GetIfFalseLabel( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const IExpr> expr;
		std::shared_ptr<const Temp::CLabel> ifTrueLabel;
		std::shared_ptr<const Temp::CLabel> ifFalseLabel;
	};

	// Calculate left than right sequentially
	class CSeq : public IStm {
	public:
		CSeq( std::shared_ptr<const IStm> left, std::shared_ptr<const IStm> right );

		std::shared_ptr<const IStm> GetLeftStm() const;
		std::shared_ptr<const IStm> GetRightStm() const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const IStm> leftStm;
		std::shared_ptr<const IStm> rightStm;
	};

	// Jump wil lead here every time
	class CLabel : public IStm {
	public:
		CLabel( std::shared_ptr<const Temp::CLabel> label );

		std::shared_ptr<const Temp::CLabel> GetLabel( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;

		virtual CExprList* Kids() const override;

		virtual IStm* Build(const CExprList* kids) const override;

	private:
		std::shared_ptr<const Temp::CLabel> label;
	};

	class CStmList {
	public:
		CStmList( std::shared_ptr<const IStm> head, std::shared_ptr<const CStmList> tail );
		
		std::shared_ptr<const IStm> GetHead() const;
		std::shared_ptr<const CStmList> GetTail() const;
	private:
		std::shared_ptr<const IStm> head;
		std::shared_ptr<const CStmList> tail;
	};

	// typedefs:
	typedef std::shared_ptr<const IStm> IStmPtr;
	typedef std::shared_ptr<const CMove> CMovePtr;
	typedef std::shared_ptr<const CExpr> CExprPtr;
	typedef std::shared_ptr<const CJump> CJumpPtr;
	typedef std::shared_ptr<const CCondJump> CCondJumpPtr;
	typedef std::shared_ptr<const CSeq> CSeqPtr;
	typedef std::shared_ptr<const CLabel> CLabelPtr;
	typedef std::shared_ptr<const CStmList> CStmListPtr;
}