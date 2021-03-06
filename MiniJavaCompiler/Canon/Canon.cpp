#include "Canon/Canon.h"
#include "Frame/Temp.h"

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))
#define NEW(clazz, ...) (std::make_shared<clazz>(clazz(__VA_ARGS__)))

CStmExprList CCanon::nopNull = CStmExprList( NEW( IRTree::CExpr, NEW( IRTree::CConst, 0 ) ), nullptr );

CMoveCall::CMoveCall( std::shared_ptr<const IRTree::CTemp> _dst, std::shared_ptr<const IRTree::CCall> _src ):
	dst(_dst),
	src(_src)
{
}

IRTree::CExprList* CMoveCall::Kids() const
{
	return src->Kids();
}

IRTree::IStm* CMoveCall::Build(const IRTree::CExprList* kids) const
{
	// ����� ������ ������ ���������� ������?
	return new IRTree::CMove( dst, IRTree::IExprPtr( src->Build( kids ) ) );
}

CExprCall::CExprCall( std::shared_ptr<const IRTree::CCall> _call ):
	call( _call )
{
}

IRTree::CExprList* CExprCall::Kids() const
{
	return call->Kids();
}

IRTree::IStm* CExprCall::Build(const IRTree::CExprList* kids) const
{
	// ������! ����� new
	return new IRTree::CExpr( IRTree::IExprPtr( call->Build( kids ) ) );
}

CStmExprList::CStmExprList( std::shared_ptr<const IRTree::IStm> _stm, std::shared_ptr<const IRTree::CExprList> _exprs ) :
	stm( _stm ),
	exprs( _exprs )
{
}

bool CCanon::isCommute( IRTree::IStmPtr a, IRTree::IExprPtr b )
{
	return isNop( a ) || INSTANCEOF( b, IRTree::CConst ) || INSTANCEOF( b, IRTree::CName );
}

bool CCanon::isNop( IRTree::IStmPtr a )
{
	auto exp = CAST( a, IRTree::CExpr );
	return exp != nullptr && INSTANCEOF( exp->GetExp(), IRTree::CConst );
}

IRTree::IStmPtr CCanon::createSeq( IRTree::IStmPtr firstStm, IRTree::IStmPtr secondStm )
{
	if( isNop( firstStm ) ) {
		return secondStm;
	}
	if( isNop( secondStm ) ) {
		return firstStm;
	}
	return std::make_shared<IRTree::CSeq>( firstStm, secondStm );
}

IRTree::IStmPtr CCanon::doStm( IRTree::CSeqPtr seq )
{
	return createSeq( doStm( seq->GetLeftStm() ), doStm( seq->GetRightStm() ) );
}

IRTree::IStmPtr CCanon::doStm( IRTree::CMovePtr move )
{
	if( INSTANCEOF( move->GetDestExpr(), IRTree::CTemp ) && INSTANCEOF( move->GetSrcExpr(), IRTree::CCall ) ) {
		return reorderStm( std::make_shared<CMoveCall>( 
			CAST( move->GetDestExpr(), IRTree::CTemp ), CAST( move->GetSrcExpr(), IRTree::CCall ) ) );
	}
	if( INSTANCEOF( move->GetDestExpr(), IRTree::CESeq ) ) {
		return doStm( CAST( NEW( IRTree::CSeq, CAST( move->GetDestExpr(), IRTree::CESeq )->GetStatement(),
			NEW( IRTree::CMove, CAST( move->GetDestExpr(), IRTree::CESeq )->GetExpression(), move->GetSrcExpr() ) ),
			IRTree::CSeq ) );
	}
	return reorderStm( move );
}

IRTree::IStmPtr CCanon::doStm( IRTree::CExprPtr expr )
{
	if( INSTANCEOF( expr->GetExp(), IRTree::CCall ) ) {
		return reorderStm( std::make_shared<CExprCall>( CAST( expr->GetExp(), IRTree::CCall ) ) );
	}
	return reorderStm( expr );
}

IRTree::IStmPtr CCanon::doStm( IRTree::IStmPtr stm )
{
	if( INSTANCEOF( stm, IRTree::CSeq ) ) {
		return doStm( CAST( stm, IRTree::CSeq ) );
	}
	if( INSTANCEOF( stm, IRTree::CMove ) ) {
		return doStm( CAST( stm, IRTree::CMove ) );
	}
	if( INSTANCEOF( stm, IRTree::CExpr ) ) {
		return doStm( CAST( stm, IRTree::CExpr ) );
	}
	return reorderStm( stm );
}

IRTree::IStmPtr CCanon::reorderStm( IRTree::IStmPtr stm )
{
	std::shared_ptr<CStmExprList> list = reorder( stm->Kids() );
	return createSeq( list->Stm(), IRTree::IStmPtr( stm->Build( list->Exprs().get() ) ) );
}

IRTree::CESeqPtr CCanon::doExp( IRTree::CESeqPtr eseq )
{
	IRTree::IStmPtr stm = doStm( eseq->GetStatement() );
	IRTree::CESeqPtr expr = doExp( eseq->GetExpression() );
	return NEW( IRTree::CESeq, createSeq( stm, expr->GetStatement() ), expr->GetExpression() );
}

IRTree::CESeqPtr CCanon::doExp( IRTree::IExprPtr eseq )
{
	if( INSTANCEOF( eseq, IRTree::CESeq ) ) {
		return doExp( CAST( eseq, IRTree::CESeq ) );
	}
	return reorderExp( eseq );
}

IRTree::CESeqPtr CCanon::reorderExp( IRTree::IExprPtr expr )
{
	std::shared_ptr<CStmExprList> list = reorder( expr->Kids() );
	return std::make_shared<const IRTree::CESeq>( list->Stm(), IRTree::IExprPtr( expr->Build( list->Exprs().get() ) ) );
}

std::shared_ptr<CStmExprList> CCanon::reorder( const IRTree::CExprList* exprs )
{
	if( exprs == nullptr ) {
		return std::make_shared<CStmExprList>( nopNull );
	}
	IRTree::IExprPtr head = exprs->GetHead();
	if( INSTANCEOF( head, IRTree::CCall ) ) {
		std::shared_ptr<Temp::CTemp> tmp = NEW( Temp::CTemp );
		IRTree::IExprPtr expr = NEW( IRTree::CESeq, NEW( IRTree::CMove, NEW( IRTree::CTemp, tmp ), head ), NEW( IRTree::CTemp, tmp ) );
		return reorder( new IRTree::CExprList( expr, exprs->GetTail() ) );
	}
	IRTree::CESeqPtr eseq = doExp( head );
	std::shared_ptr<const CStmExprList> tail( reorder( exprs->GetTail().get() ) );
	if( isCommute( tail->Stm(), eseq->GetExpression() ) ) {
		return NEW( CStmExprList, createSeq( eseq->GetStatement(), tail->Stm() ), 
			NEW( IRTree::CExprList, eseq->GetExpression(), tail->Exprs() ) );
	}
	std::shared_ptr<Temp::CTemp> tmp = NEW( Temp::CTemp );
	return NEW( CStmExprList, createSeq( eseq->GetStatement(),
		createSeq( NEW( IRTree::CMove, NEW( IRTree::CTemp, tmp ), eseq->GetExpression() ), tail->Stm() ) ),
		NEW( IRTree::CExprList, NEW( IRTree::CTemp, tmp ), tail->Exprs() ) );
}

IRTree::CStmListPtr CCanon::linear( IRTree::CSeqPtr seq, IRTree::CStmListPtr list )
{
	return linear( seq->GetLeftStm(), linear( seq->GetRightStm(), list ) );
}

IRTree::CStmListPtr CCanon::linear( IRTree::IStmPtr stm, IRTree::CStmListPtr list )
{
	if( INSTANCEOF( stm, IRTree::CSeq ) ) {
		return linear( CAST( stm, IRTree::CSeq ), list );
	}
	return NEW( IRTree::CStmList, stm, list );
}

IRTree::CStmListPtr CCanon::Linearize( IRTree::IStmPtr stm )
{
	return linear( doStm( stm ), nullptr );
}