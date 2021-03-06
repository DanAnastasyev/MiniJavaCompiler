#include "IRTree/Statements.h"
#include "Expressions.h"
#include <memory>

namespace IRTree
{
	CMove::CMove( std::shared_ptr<const IExpr> dst, std::shared_ptr<const IExpr> src ) :
		destExpr( dst ),
		srcExpr( src )
	{}

	std::shared_ptr<const IExpr> CMove::GetDestExpr() const
	{
		return destExpr;
	}

	std::shared_ptr<const IExpr> CMove::GetSrcExpr() const
	{
		return srcExpr;
	}

	void CMove::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CMove::Kids() const
	{
		if( std::dynamic_pointer_cast<const IRTree::CMem>( destExpr ) != nullptr ) {
			return new CExprList( std::dynamic_pointer_cast<const IRTree::CMem>( destExpr )->GetMem(), std::make_shared<const CExprList>( srcExpr, nullptr ) );
		} else {
			return new CExprList( srcExpr, nullptr );
		}
	}

	IStm* CMove::Build(const CExprList* kids) const
	{
		if( std::dynamic_pointer_cast< const IRTree::CMem >( destExpr ) != nullptr ) {
			return new IRTree::CMove( std::make_shared<IRTree::CMem>( kids->GetHead() ), kids->GetTail()->GetHead() );
		} else {
			return new IRTree::CMove( destExpr, kids->GetHead() );
		}
	}

	CExpr::CExpr( std::shared_ptr<const IExpr> _expr ) :
		expr( _expr )
	{}

	std::shared_ptr<const IExpr> CExpr::GetExp() const
	{
		return expr;
	}

	void CExpr::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CExpr::Kids() const
	{
		return new IRTree::CExprList( expr, nullptr );
	}

	IStm* CExpr::Build(const CExprList* kids) const
	{
		return new IRTree::CExpr( kids->GetHead() );
	}

	CJump::CJump( std::shared_ptr<const IExpr> expr, std::shared_ptr<const Temp::CLabelList> _targets ) :
		jmpExpr( expr ),
		targets( _targets )
	{
	}

	CJump::CJump( std::shared_ptr<const Temp::CLabel> label ) :
		jmpExpr( new CName( label ) ),
		targets( std::make_shared<Temp::CLabelList>( label, nullptr ) )
	{
	}

std::shared_ptr<const IExpr> CJump::GetJumpExpr() const
	{
		return jmpExpr;
	}

	std::shared_ptr<const Temp::CLabelList> CJump::GetTargets() const
	{
		return targets;
	}

	void CJump::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CJump::Kids() const
	{
		return new IRTree::CExprList( jmpExpr, nullptr );
	}

	IStm* CJump::Build(const CExprList* kids) const
	{
		return new IRTree::CJump( kids->GetHead(), targets );
	}

	CCondJump::CCondJump(
		std::shared_ptr<const IExpr> _expr,
		std::shared_ptr<const Temp::CLabel> _ifTrueLabel,
		std::shared_ptr<const Temp::CLabel> _ifFalseLabel ) :
		expr( _expr ),
		ifTrueLabel( _ifTrueLabel ),
		ifFalseLabel( _ifFalseLabel )
	{}

	std::shared_ptr<const IExpr> CCondJump::GetExpr() const
	{
		return expr;
	}

	std::shared_ptr<const Temp::CLabel> CCondJump::GetIfTrueLabel( ) const
	{
		return ifTrueLabel;
	}

	std::shared_ptr<const Temp::CLabel> CCondJump::GetIfFalseLabel() const
	{
		return ifFalseLabel;
	}

	void CCondJump::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CCondJump::Kids() const {
		return new IRTree::CExprList( expr, nullptr );
	}

	IStm* CCondJump::Build(const CExprList* kids) const
	{
		return new IRTree::CCondJump( expr, ifTrueLabel, ifFalseLabel );
	}

	int CCondJump::NotRel( int binop )
	{
		switch( binop ) {
			case IExpr::LESS: return IExpr::GE;
			case IExpr::GE: return IExpr::LESS;
			case IExpr::XOR: return IExpr::XOR;
		}
		//switch( binop ) {
		//	case EQ: return NE;
		//	case NE: return EQ;
		//	case LT: return GE;
		//	case GT: return LE;
		//	case LE: return GT;
		//	case GE: return LT;
		//	case ULT: return UGE;
		//	case ULE: return UGT;
		//	case UGT: return ULE;
		//	case UGE: return ULT;
		//	default: throw std::runtime_error( "Bad relop in CJUMP.NotRel()" );
		//}
	}

CSeq::CSeq( std::shared_ptr<const IStm> left, std::shared_ptr<const IStm> right ) :
		leftStm( left ),
		rightStm( right )
	{}

	std::shared_ptr<const IStm> CSeq::GetLeftStm() const
	{
		return leftStm;
	}

	std::shared_ptr<const IStm> CSeq::GetRightStm() const
	{
		return rightStm;
	}

	void CSeq::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}


	CExprList* CSeq::Kids() const
	{
		throw std::logic_error( "Kids() const not applicable to SEQ" );
	}

	IStm* CSeq::Build(const CExprList* kids) const
	{
		throw std::logic_error( "Build() not applicable to SEQ" );
	}

	CLabel::CLabel( std::shared_ptr<const Temp::CLabel> _label ) :
		label( _label )
	{}

	std::shared_ptr<const Temp::CLabel> CLabel::GetLabel() const
	{
		return label;
	}

	void CLabel::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CLabel::Kids() const
	{
		return nullptr;
	}

	IStm* CLabel::Build(const CExprList* kids) const
	{
		return new CLabel( label );
	}

	CStmList::CStmList( std::shared_ptr<const IStm> _head, std::shared_ptr<const CStmList> _tail ) :
		head(_head), tail( _tail )
	{}

	std::shared_ptr<const IStm>& CStmList::GetHead() const
	{
		return head;
	}

	std::shared_ptr<const CStmList>& CStmList::GetTail() const
	{
		return tail;
	}
}