#pragma once

namespace IRTree
{
	class IStm {
	public:
		virtual ~IStm() = 0;
	};

	class CMove : public IStm {
		CMove()
	};

	class CJump : public IStm {

	};

	class CCompJump : public IStm {

	};

	class CSeq : public IStm {

	};

	class CLabel : public IStm {

	};
}