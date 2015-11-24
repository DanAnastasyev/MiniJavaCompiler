#pragma once

namespace IRTree
{
	class IStm;

	class IExpr {
	public:
		virtual ~IExpr() = 0;
	};

	class CConst : public IExpr {
		
	};

	class CName : public IExpr {

	};

	class CTemp : public IExpr {

	};

	class CBinop : public IExpr {

	};

	class CMem : public IExpr {

	};

	class CCall : public IExpr {

	};

	class CEseq : public IExpr {
		
	};
}