#pragma once
#include <vector>
#include <memory>

#include "Frame\Temp.h"
#include "Visitors/Visitor.h"

namespace IRTree
{
	class IExpr;

	class IStm {
	public:
		virtual ~IStm() {}

		virtual void Accept( IIRTreeVisitor* visitor ) const = 0;
	};

	typedef std::shared_ptr<const IStm> CStmPtr;

	class CMove : public IStm {
	public:
		CMove( std::shared_ptr<const IExpr> dst, std::shared_ptr<const IExpr> src );

		std::shared_ptr<const IExpr> GetDestExpr( ) const;
		std::shared_ptr<const IExpr> GetSrcExpr( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const IExpr> destExpr;
		std::shared_ptr<const IExpr> srcExpr;
	};

	class CExpr : public IStm {
	public:
		CExpr( std::shared_ptr<const IExpr> exp );

		std::shared_ptr<const IExpr> GetExp( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const IExpr> exp;
	};

	class CJump : public IStm {
	public:
		CJump( std::shared_ptr<const IExpr> exp, const std::vector<std::shared_ptr<const Temp::CLabel>>& labels );
		CJump( std::shared_ptr<const Temp::CLabel> label );

		std::shared_ptr<const IExpr> GetJumpExpr() const;
		std::vector<std::shared_ptr<const Temp::CLabel>> GetLabels() const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const IExpr> jmpExpr;
		std::vector<std::shared_ptr<const Temp::CLabel>> labels;
	};

	class CCondJump : public IStm {
	public:
		CCondJump( int binOp, std::shared_ptr<const IExpr> left, std::shared_ptr<const IExpr> right, 
			std::shared_ptr<const Temp::CLabel> ifTrueLabel, std::shared_ptr<const Temp::CLabel> ifFalseLabel );

		std::shared_ptr<const IExpr> GetLeftExpr() const;
		std::shared_ptr<const IExpr> GetRightExpr() const;
		int GetBinOp( ) const;
		std::shared_ptr<const Temp::CLabel> GetIfTrueLabel( ) const;
		std::shared_ptr<const Temp::CLabel> GetIfFalseLabel( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const IExpr> leftExpr;
		std::shared_ptr<const IExpr> rightExpr;
		int binOp;
		std::shared_ptr<const Temp::CLabel> ifTrueLabel;
		std::shared_ptr<const Temp::CLabel> ifFalseLabel;
	};

	class CSeq : public IStm {
	public:
		CSeq( std::shared_ptr<const IStm> left, std::shared_ptr<const IStm> right );

		std::shared_ptr<const IStm> GetLeftStm() const;
		std::shared_ptr<const IStm> GetRightStm() const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const IStm> leftStm;
		std::shared_ptr<const IStm> rightStm;
	};

	class CLabel : public IStm {
	public:
		CLabel( std::shared_ptr<const Temp::CLabel> label );

		std::shared_ptr<const Temp::CLabel> GetLabel( ) const;

		void Accept( IIRTreeVisitor* visitor ) const override;
	private:
		std::shared_ptr<const Temp::CLabel> label;
	};
}