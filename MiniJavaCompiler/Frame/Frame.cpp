#include "Frame\Frame.h"

namespace Frame {

	CInFrame::CInFrame( int _offset ) :
		offset( _offset )
	{}

	const IRTree::IExpr* CInFrame::GetExp( const Temp::CTemp* framePtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return new IRTree::CMem( std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( 
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CTemp( framePtr ) ),
			IRTree::CBinop::PLUS,
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CConst( machineOffset ) ) ) ) );
	}

	CInReg::CInReg( int _offset ) :
		offset( _offset )
	{}

	const IRTree::IExpr* CInReg::GetExp( const Temp::CTemp* frameRegPtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return new IRTree::CMem( std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop(
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CTemp( frameRegPtr ) ),
			IRTree::CBinop::PLUS,
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CConst( machineOffset ) ) ) ) );
	}


}