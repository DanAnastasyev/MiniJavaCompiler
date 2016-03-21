#include <Canon/Canon.h>

bool CCanon::isCommute( std::shared_ptr<const IRTree::IStm> a, std::shared_ptr<const IRTree::IStm> b )
{
	return isNop( a )
		|| std::dynamic_pointer_cast<const IRTree::CName>( b ) != 0
		|| std::dynamic_pointer_cast<const IRTree::CName>( b ) != 0;
}

bool CCanon::isNop( std::shared_ptr<const IRTree::IStm> a )
{
	auto exp = std::dynamic_pointer_cast<const IRTree::CExpr>( a );
	return exp != 0 && std::dynamic_pointer_cast<const IRTree::CConst>( exp->GetExp() ) != 0;
}

std::shared_ptr<const IRTree::IStm> CCanon::createSeq( std::shared_ptr<const IRTree::IStm> firstStm, std::shared_ptr<const IRTree::IStm> secondStm )
{
	if( isNop( firstStm ) ) {
		return secondStm;
	}
	if( isNop( secondStm ) ) {
		return firstStm;
	}
	return std::make_shared<IRTree::CSeq>( firstStm, secondStm );
}

