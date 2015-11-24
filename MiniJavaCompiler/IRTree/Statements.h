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

	class CMove : public IStm {
	public:
		CMove( IExpr* dst, IExpr* src );
	private:
		std::shared_ptr<IExpr> destExpr;
		std::shared_ptr<IExpr> srcExpr;
	};

	class CJump : public IStm {
	public:
		CJump( IExpr* exp, const std::vector<Temp::CLabel>& labels );

	private:
		std::shared_ptr<IExpr> jmpExpr;
		std::vector<Temp::CLabel> labels;
	};

	class CCompJump : public IStm {
	public:
		CCompJump( int op, IExpr* left, IExpr* right, const Temp::CLabel& ifTrue, const Temp::CLabel& ifFalse );
	private:
		std::shared_ptr<IExpr> leftExpr;
		std::shared_ptr<IExpr> rightExpr;
		int binOp;
		Temp::CLabel ifTrue;
		Temp::CLabel ifFalse;
	};

	class CSeq : public IStm {

	};

	class CLabel : public IStm {

	};
}