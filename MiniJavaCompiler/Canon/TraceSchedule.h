#pragma once
#include "IRTree/Statements.h"
#include "IRTree/Expressions.h"
#include "BasicBlocks.h"

typedef std::shared_ptr<CBasicBlocks> CBlocksPtr;
namespace Temp {
	typedef std::shared_ptr<const Temp::CLabel> CLabelPtr;
}

class CDictionary {
public:
	CDictionary();

	IRTree::CStmListPtr Get( Temp::CLabelPtr key ) const;
	void Put( Temp::CLabelPtr key, const IRTree::CStmListPtr value ) const;
	void Remove( Temp::CLabelPtr key ) const;

	void operator=(std::nullptr_t);

private:
	std::shared_ptr<std::unordered_map<Temp::CLabelPtr, IRTree::CStmListPtr>> internalMap;
};

class CTraceSchedule {
public:
	explicit CTraceSchedule( CBlocksPtr );

	void Trace( IRTree::CStmListPtr l );

private:
	IRTree::CStmListPtr stms;
	CBlocksPtr blocks;
	CDictionary table;
	
	IRTree::CStmListPtr getLast( IRTree::CStmListPtr block ) const;
	IRTree::CStmListPtr getNext();
};