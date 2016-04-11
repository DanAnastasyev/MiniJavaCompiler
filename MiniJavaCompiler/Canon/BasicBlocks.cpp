#include "BasicBlocks.h"

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)

using namespace IRTree;

CBasicBlocks::CBasicBlocks( CStmListPtr stmsList )
{
	makeBlocks( stmsList );
}

void CBasicBlocks::addStm( IRTree::IStmPtr stm )
{
	// ?
	lastStm = std::make_shared<CStmList>( stm, nullptr );
}

void CBasicBlocks::doStms( CStmListPtr stmList )
{
	if( stmList == nullptr ) {
		doStms( std::make_shared<const CStmList>( std::make_shared<const CJump>( doneLabel ), nullptr ) );
	} else if( INSTANCEOF( stmList->GetHead(), CJump ) || INSTANCEOF( stmList->GetHead(), CCondJump ) ) {
		addStm( stmList->GetHead() );
		makeBlocks( stmList->GetTail() );
	} else if( INSTANCEOF( stmList->GetHead(), CLabel ) ) {
		doStms( std::make_shared<const CStmList>( std::make_shared<const CJump>(
			std::dynamic_pointer_cast< const CLabel >( stmList->GetHead() )->GetLabel() ), stmList ) );
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
		lastStm = std::make_shared<const CStmList>( stmList->GetHead(), nullptr );
		if( lastBlock == nullptr ) {
			lastBlock = blocks = std::make_shared<const CStmListList>( lastStm, nullptr );
		} else {
			// ?
		}
		doStms( stmList->GetTail() );
	}
	// Если её нет, дописываем
	makeBlocks( std::make_shared<const CStmList>( std::make_shared<const CLabel>( std::make_shared<const Temp::CLabel>() ), stmList ) );
}
