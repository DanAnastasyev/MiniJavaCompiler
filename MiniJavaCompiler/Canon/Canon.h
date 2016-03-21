#pragma once

#include "IRTree/Expressions.h"
#include "IRTree/Statements.h"

class CCanon {
public:

private:
	// ќпределение коммутативности узлов
	static bool isCommute( const IRTree::IStm* a, const IRTree::IExpr* b );
	// явл€етс€ ли это экспрешеном с константным значением
	static bool isNop( const IRTree::IStm* a );

	static IRTree::IStm createSeq( const IRTree::IStm* firstStm, const IRTree::IStm* secondStm );
};