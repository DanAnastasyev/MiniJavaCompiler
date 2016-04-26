#pragma once

#include "IRTree/Statements.h"

class CStmListList {
public:
	CStmListList( IRTree::CStmListPtr _head, std::shared_ptr<const CStmListList> _tail ) :
		head( _head ), tail( _tail )
	{}

	IRTree::CStmListPtr GetHead() const { return head; }

	std::shared_ptr<const CStmListList>& GetTail() const { return tail; }

private:
	IRTree::CStmListPtr head;
	mutable std::shared_ptr<const CStmListList> tail;
};
typedef std::shared_ptr<const CStmListList> CStmListListPtr;


// Получает список канонических деревьев, выстраивает из них базовые блоки
class CBasicBlocks {
public:
	CBasicBlocks( IRTree::CStmListPtr stmsList );

	std::shared_ptr<const Temp::CLabel> GetDoneLabel() const { return doneLabel; }

	CStmListListPtr& GetBlocks() const { return blocks; }

	IRTree::CStmListPtr allStms;

private:
	mutable CStmListListPtr blocks;
	std::shared_ptr<const Temp::CLabel> doneLabel;
	CStmListListPtr lastBlock;
	IRTree::CStmListPtr lastStm;

	void addStm( IRTree::IStmPtr stm );

	void doStms( IRTree::CStmListPtr stmList );

	void makeBlocks( IRTree::CStmListPtr stmList );
};