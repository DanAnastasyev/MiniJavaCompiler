#pragma once

#include "IRTree/Expressions.h"
#include "IRTree/Statements.h"

class CCanon {
public:

private:
	// ќпределение коммутативности узлов
	static bool isCommute( std::shared_ptr<const IRTree::IStm> a, std::shared_ptr<const IRTree::IStm> b );
	// явл€етс€ ли это экспрешеном с константным значением
	static bool isNop( std::shared_ptr<const IRTree::IStm> a );

	static std::shared_ptr<const IRTree::IStm> createSeq( std::shared_ptr<const IRTree::IStm> firstStm, std::shared_ptr<const IRTree::IStm> secondStm );
};