#include "IRTree/Expressions.h"

namespace IRTree
{

	CConst::CConst( int _value ) :
		value( _value )
	{}

	int CConst::GetValue() const
	{
		return value;
	}

	CName::CName( std::shared_ptr<const Temp::CLabel> _name ) :
		name( _name )
	{}

	CTemp::CTemp( const std::shared_ptr<Temp::CTemp> _temp ) :
		temp( _temp )
	{}

	CBinop::CBinop( BINOP _binop, CExprPtr _left, CExprPtr _right ) :
		binop( _binop ),
		left( _left ),
		right( _right )
	{}

	CMem::CMem( CExprPtr _mem ) :
		mem( _mem )
	{}

	CCall::CCall( const CSymbol* _funcName, const std::vector<CExprPtr>& _arguments ) :
		funcName( _funcName ),
		arguments( _arguments )
	{}

	CESeq::CESeq( CStmPtr _statement, CExprPtr _expression ) :
		statement( _statement ),
		expression( _expression )
	{}

}