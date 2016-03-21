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

	IRTree::CExprList* CMove::Kids()
	{
		if( std::dynamic_pointer_cast<const IRTree::CMem>( destExpr ) != 0 ) {
			return new CExprList( std::dynamic_pointer_cast<const IRTree::CMem>( destExpr ), std::make_shared<const CExprList>( srcExpr, nullptr ) );
		} else {
			return new CExprList( srcExpr, nullptr );
		}
	}

	IRTree::IStm* CMove::Build( CExprList* kids )
	{
		if( std::dynamic_pointer_cast< const IRTree::CMem >( destExpr ) != 0 ) {
			return new IRTree::CMove( std::dynamic_pointer_cast<const IRTree::CMem>( kids->GetHead() ), kids->GetTail()->GetHead() );
		} else {
			return new IRTree::CMove( destExpr, kids->GetHead() );
		}
	}

	CExpr::CExpr( std::shared_ptr<const IExpr> _exp ) :
		exp( _exp )
	{}

	std::shared_ptr<const IExpr> CExpr::GetExp() const
	{
		return exp;
	}

	void CExpr::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CExpr::Kids()
	{
		return new IRTree::CExprList( exp, nullptr );
	}

	IRTree::IStm* CExpr::Build( CExprList* kids )
	{
		return new IRTree::CExpr( kids->GetHead() );
	}

	CJump::CJump( std::shared_ptr<const IExpr> _exp, const std::vector<std::shared_ptr<const Temp::CLabel>>& _labels ) :
		jmpExpr( _exp ),
		labels( _labels )
	{}

	CJump::CJump( std::shared_ptr<const Temp::CLabel> label ) :
		jmpExpr( new CName(label) ),
		labels( {label} )
	{}

	std::shared_ptr<const IExpr> CJump::GetJumpExpr() const
	{
		return jmpExpr;
	}

	std::vector<std::shared_ptr<const Temp::CLabel>> CJump::GetLabels() const
	{
		return labels;
	}

	void CJump::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CJump::Kids()
	{
		return new IRTree::CExprList( jmpExpr, nullptr );
	}

	IRTree::IStm* CJump::Build( CExprList* kids )
	{
		return new IRTree::CJump( kids->GetHead(), labels );
	}

	CCondJump::CCondJump( int _binOp, std::shared_ptr<const IExpr> _left, std::shared_ptr<const IExpr> _right,
		std::shared_ptr<const Temp::CLabel> _ifTrueLabel, std::shared_ptr<const Temp::CLabel> _ifFalseLabel ) :
		binOp( _binOp ),
		leftExpr( _left ),
		rightExpr( _right ),
		ifTrueLabel( _ifTrueLabel ),
		ifFalseLabel( _ifFalseLabel )
	{}

	std::shared_ptr<const IExpr> CCondJump::GetLeftExpr() const
	{
		return leftExpr;
	}

	std::shared_ptr<const IExpr> CCondJump::GetRightExpr() const
	{
		return rightExpr;
	}

	int CCondJump::GetBinOp() const
	{
		return binOp;
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
	

	IRTree::CExprList* CCondJump::Kids()
	{
		return new IRTree::CExprList( leftExpr, std::make_shared<const CExprList>( rightExpr, nullptr ) );
	}

	IRTree::IStm* CCondJump::Build( CExprList* kids )
	{
		return new IRTree::CCondJump( binOp, kids->GetHead(), kids->GetTail()->GetHead(), ifTrueLabel, ifFalseLabel );
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


	IRTree::CExprList* CSeq::Kids()
	{
		throw std::logic_error( "Kids() not applicable to SEQ" );
	}

	IRTree::IStm* CSeq::Build( CExprList* kids )
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

	IRTree::CExprList* CLabel::Kids()
	{
		return nullptr;
	}

	IRTree::IStm* CLabel::Build( CExprList* kids )
	{
		return this;
	}

	CStmList::CStmList( std::shared_ptr<const IStm> _head, std::shared_ptr<const CStmList> _tail ) :
		head(_head), tail( _tail )
	{}

	std::shared_ptr<const IStm> CStmList::GetHead() const
	{
		return head;
	}

	std::shared_ptr<const CStmList> CStmList::GetTail() const
	{
		return tail;
	}

}