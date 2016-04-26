#include "Canon/TraceSchedule.h"

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))
#define NEW(clazz, ...) (std::make_shared<clazz>(clazz(__VA_ARGS__)))

using namespace IRTree;

CDictionary::CDictionary() :
	internalMap( std::make_shared<std::unordered_map<Temp::CLabelPtr, CStmListPtr>>() )
{
}

CStmListPtr CDictionary::Get( Temp::CLabelPtr key ) const
{
	return internalMap->at( key );
}

void CDictionary::Put( Temp::CLabelPtr key, CStmListPtr value ) const
{
	(*internalMap)[key] = value;
}

void CDictionary::Remove( Temp::CLabelPtr key ) const
{
	internalMap->erase( key );
}

void CDictionary::operator=(std::nullptr_t)
{
	internalMap = nullptr;
}

CTraceSchedule::CTraceSchedule( CBlocksPtr blocks ) :
	blocks( blocks )
{
	for( auto list = blocks->GetBlocks(); list; list = list->GetTail() ) {
		table.Put( CAST(list->GetHead()->GetHead(), CLabel)->GetLabel(), list->GetHead() );
	}
	stms = getNext();
	table = nullptr;
}

CStmListPtr CTraceSchedule::getLast( CStmListPtr block ) const {
	auto l = block;
	while( l->GetTail()->GetTail() ) {
		l = l->GetTail();
	}
	return l;
}

void CTraceSchedule::Trace( CStmListPtr list )
{
	while( true ) {
		auto label = CAST( list->GetHead(), CLabel );
		table.Remove( label->GetLabel() );
		auto last = getLast( list );
		auto stm = last->GetTail()->GetHead();
		if( INSTANCEOF( stm, CJump ) ) {
			auto jump = CAST( stm, CJump );

			auto target = CAST( table.Get( jump->GetTargets()->Head() ), IRTree::CStmList );
			if( jump->GetTargets()->Tail() == nullptr && target != nullptr ) {
				last->GetTail() = target;
				list = target;
			} else {
				last->GetTail()->GetTail() = getNext();
				return;
			}

		} else if( INSTANCEOF( stm, CCondJump ) ) {
			auto jump = CAST( stm, CCondJump );
			auto trueLabel = CAST( table.Get( jump->GetIfTrueLabel() ), CStmList );
			auto falseLabel = CAST( table.Get( jump->GetIfFalseLabel() ), CStmList );
			if( falseLabel != nullptr ) {
				last->GetTail()->GetTail() = falseLabel;
				list = falseLabel;
			} else if( trueLabel != nullptr ) {
				auto expr = CAST( jump->GetExpr(), CBinop );
				if( expr == nullptr ) {
					throw std::exception( "azaza" );
				}
				last->GetTail()->GetHead() = NEW( CCondJump,
					NEW( CBinop, IExpr::BINOP( CCondJump::NotRel( expr->GetBinOp() ) ), expr->GetLeft(), expr->GetRight() ),
					jump->GetIfFalseLabel(),
					jump->GetIfTrueLabel() );
				last->GetTail()->GetTail() = trueLabel;
				list = trueLabel;
			} else {
				auto ff = NEW( Temp::CLabel );

				last->GetTail()->GetHead() = NEW( CCondJump, jump->GetExpr(), jump->GetIfTrueLabel(), ff );
				last->GetTail()->GetTail() = NEW( CStmList,
					NEW( CLabel, ff ),
					NEW( CStmList, NEW( CJump, jump->GetIfFalseLabel() ), getNext() ) );
				return;
			}
		} else {
			throw new std::runtime_error( "Bad basic block in TraceSchedule" );
		}
	}
}

CStmListPtr CTraceSchedule::getNext()
{
	if( blocks->GetBlocks() == nullptr ) {
		return NEW( CStmList, NEW( CLabel, blocks->GetDoneLabel() ), nullptr );
	}
	auto stmList = blocks->GetBlocks()->GetHead();
	auto label = CAST( stmList->GetHead(), CLabel );
	if( table.Get( label->GetLabel() ) ) {
		Trace( stmList );
		return stmList;
	}
	blocks->GetBlocks() = blocks->GetBlocks()->GetTail();
	return getNext();
}