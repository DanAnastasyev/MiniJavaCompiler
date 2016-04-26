#include "BasicBlocks.h"

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))
#define NEW(clazz, ...) (std::make_shared<clazz>(clazz(__VA_ARGS__)))

using namespace IRTree;

CBasicBlocks::CBasicBlocks( CStmListPtr stmsList )
{
	doneLabel = NEW( Temp::CLabel );
	makeBlocks( stmsList );
}

void CBasicBlocks::addStm( IRTree::IStmPtr stm )
{
	lastStm = lastStm->GetTail() = NEW( CStmList, stm, nullptr );
}

void CBasicBlocks::doStms( CStmListPtr stmList )
{
	if( stmList == nullptr ) {
		doStms( NEW( CStmList, NEW( CJump, doneLabel ), nullptr ) );
	} else if( INSTANCEOF( stmList->GetHead(), CJump ) || INSTANCEOF( stmList->GetHead(), CCondJump ) ) {
		addStm( stmList->GetHead() );
		makeBlocks( stmList->GetTail() );
	} else if( INSTANCEOF( stmList->GetHead(), CLabel ) ) {
		doStms( NEW( CStmList, NEW( CJump, CAST( stmList->GetHead(), CLabel )->GetLabel() ), stmList ) );
	} else {
		addStm( stmList->GetHead() );
		doStms( stmList->GetTail() );
	}
}

void CBasicBlocks::makeBlocks( CStmListPtr stmList )
{
	if( stmList == nullptr ) {
		return;
	}
	// Базовый блок должен начинаться с LABEL
	if( INSTANCEOF( stmList->GetHead(), CLabel ) ) {
		lastStm = NEW( CStmList, stmList->GetHead(), nullptr );
		if( lastBlock == nullptr ) {
			lastBlock = blocks = NEW( CStmListList, lastStm, nullptr );
		} else {
			lastBlock = lastBlock->GetTail() = NEW( CStmListList, lastStm, nullptr );
		}
		doStms( stmList->GetTail() );
	}
	// Если её нет, дописываем
	else {
		makeBlocks( NEW( CStmList, NEW( CLabel, NEW( Temp::CLabel ) ), stmList ) );
	}
}
