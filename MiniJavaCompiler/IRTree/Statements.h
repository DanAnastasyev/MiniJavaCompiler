#pragma once
#include <memory>

namespace IRTree
{
	class IStm {
	public:
		virtual ~IStm() = 0;
	};

	typedef std::shared_ptr<IStm> CStmPtr;

	class CMove : public IStm {

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