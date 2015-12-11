#include "IRTree/Statements.h"
#include "Expressions.h"

namespace IRTree
{
	CMove::CMove( std::shared_ptr<const IExpr> dst, std::shared_ptr<const IExpr> src ) :
		destExpr( dst ),
		srcExpr( src )
	{}

	CExpr::CExpr( std::shared_ptr<const IExpr> _exp ) :
		exp( _exp )
	{}

	CJump::CJump( std::shared_ptr<const IExpr> _exp, const std::vector<std::shared_ptr<const Temp::CLabel>>& _labels ) :
		jmpExpr( _exp ),
		labels( _labels )
	{}

	CJump::CJump( std::shared_ptr<const Temp::CLabel> label ) :
		jmpExpr( new CName(label) ),
		labels( {label} )
	{}



	CCondJump::CCondJump( int _binOp, std::shared_ptr<const IExpr> _left, std::shared_ptr<const IExpr> _right,
		std::shared_ptr<const Temp::CLabel> _ifTrueLabel, std::shared_ptr<const Temp::CLabel> _ifFalseLabel ) :
		binOp( _binOp ),
		leftExpr( _left ),
		rightExpr( _right ),
		ifTrueLabel( _ifTrueLabel ),
		ifFalseLabel( _ifFalseLabel )
	{}

	CSeq::CSeq( std::shared_ptr<const IStm> left, std::shared_ptr<const IStm> right ) :
		leftStm( left ),
		rightStm( right )
	{}

	CLabel::CLabel( std::shared_ptr<const Temp::CLabel> _label ) :
		label( _label )
	{}

}