#include "IRTree\Frame.h"

namespace Frame {

	CFrame::CFrame(const CSymbol* _name, int _formalsCount) :
		name(_name),
		formalsCount(_formalsCount)
	{}
}