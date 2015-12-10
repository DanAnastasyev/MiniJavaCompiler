#pragma once
#include <vector>
#include <memory>

#include "Frame\Temp.h"

namespace IRTree
{
	class IExpr;

	class IStm {
	public:
		virtual ~IStm() = 0;
	};

	typedef std::shared_ptr<const IStm> CStmPtr;

	class CMove : public IStm {
	public:
		CMove( std::shared_ptr<const IExpr> dst, std::shared_ptr<const IExpr> src );

		std::shared_ptr<const IExpr> GetDestExpr( ) const;
		std::shared_ptr<const IExpr> GetSrcExpr( ) const;
	private:
		std::shared_ptr<const IExpr> destExpr;
		std::shared_ptr<const IExpr> srcExpr;
	};

	class CJump : public IStm {
	public:
		CJump( std::shared_ptr<const IExpr> exp, const std::vector<Temp::CLabel>& labels );
		CJump( std::shared_ptr<const Temp::CLabel> label );

		std::shared_ptr<const IExpr> GetJumpExpr() const;
		std::vector<Temp::CLabel> GetLabel() const;
	private:
		std::shared_ptr<const IExpr> jmpExpr;
		std::vector<Temp::CLabel> labels;
	};

	class CCondJump : public IStm {
	public:
		CCondJump( IExpr::CJUMP binOp, std::shared_ptr<const IExpr> left, std::shared_ptr<const IExpr> right, 
			std::shared_ptr<const Temp::CLabel> ifTrueLabel, std::shared_ptr<const Temp::CLabel> ifFalseLabel );

		std::shared_ptr<const IExpr> GetLeftExpr() const;
		std::shared_ptr<const IExpr> GetRightExpr() const;
		IExpr::CJUMP GetBinOp( ) const;
		Temp::CLabel GetIfTrueLabel() const;
		Temp::CLabel GetIfFalseLabel() const;
	private:
		std::shared_ptr<const IExpr> leftExpr;
		std::shared_ptr<const IExpr> rightExpr;
		IExpr::CJUMP binOp;
		std::shared_ptr<const Temp::CLabel> ifTrueLabel;
		std::shared_ptr<const Temp::CLabel> ifFalseLabel;
	};

	class CSeq : public IStm {
	public:
		CSeq( std::shared_ptr<const IStm> left, std::shared_ptr<const IStm> right );

		std::shared_ptr<const IStm> GetLeftStm() const;
		std::shared_ptr<const IStm> GetRightStm() const;
	private:
		std::shared_ptr<const IStm> leftStm;
		std::shared_ptr<const IStm> rightStm;
	};

	class CLabel : public IStm {
	public:
		CLabel( std::shared_ptr<const Temp::CLabel> label );

		Temp::CLabel GetLabel() const;
	private:
		std::shared_ptr<const Temp::CLabel> label;
	};
}