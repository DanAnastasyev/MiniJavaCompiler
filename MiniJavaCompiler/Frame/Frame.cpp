#include "Frame\Frame.h"

namespace Frame {

	CInFrame::CInFrame( int _offset ) :
		offset( _offset )
	{}

	const IRTree::CExprPtr CInFrame::GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return IRTree::CExprPtr( new IRTree::CMem( std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::CBinop::PLUS,
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CTemp( framePtr ) ),
			IRTree::CExprPtr(new IRTree::CConst( machineOffset ) ) ) ) ) );
	}

	CInReg::CInReg()
	{}

	const IRTree::CExprPtr CInReg::GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const
	{
		return IRTree::CExprPtr( new IRTree::CTemp( temp ) );
	}


}