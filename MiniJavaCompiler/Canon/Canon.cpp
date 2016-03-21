#include <Canon/Canon.h>

bool CCanon::isCommute( const IRTree::IStm* a, const IRTree::IExpr* b )
{
	return isNop( a )
		|| dynamic_cast<const IRTree::CName*>( b ) != 0
		|| dynamic_cast<const IRTree::CConst*>( b ) != 0;
}

bool CCanon::isNop( const IRTree::IStm* a )
{
	const IRTree::CExpr* exp = dynamic_cast<const IRTree::CExpr*>( a );
	return exp != 0 && std::dynamic_pointer_cast<const IRTree::CConst*>( exp->GetExp() ) != 0;
}

