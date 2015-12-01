#include "Frame\Frame.h"

namespace Frame {

	CInFrame::CInFrame( int _offset ) :
		offset( _offset )
	{}

	const IRTree::CExprPtr CInFrame::GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return IRTree::CExprPtr(new IRTree::CMem(IRTree::CExprPtr( new IRTree::CBinop(
			IRTree::CExprPtr( new IRTree::CTemp( framePtr ) ),
			IRTree::CBinop::PLUS,
			IRTree::CExprPtr(new IRTree::CConst( machineOffset ) ) ) ) ) );
	}

	CInReg::CInReg( int _offset ) :
		offset( _offset )
	{}

	const IRTree::CExprPtr CInReg::GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return IRTree::CExprPtr(new IRTree::CMem(IRTree::CExprPtr( new IRTree::CBinop(
			IRTree::CExprPtr( new IRTree::CTemp( frameRegPtr ) ),
			IRTree::CBinop::PLUS,
			IRTree::CExprPtr( new IRTree::CConst( machineOffset ) ) ) ) ) );
	}


}