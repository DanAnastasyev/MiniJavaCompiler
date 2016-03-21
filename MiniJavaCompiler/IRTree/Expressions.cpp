#include "IRTree/Expressions.h"

namespace IRTree
{

	CConst::CConst( int _value ) :
		value( _value )
	{}

	int CConst::GetValue() const
	{
		return value;
	}

	void CConst::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CConst::Kids()
	{
		return nullptr;
	}

	IRTree::IExpr* CConst::Build( CExprList* kids )
	{
		return this;
	}

	CName::CName( std::shared_ptr<const Temp::CLabel> _name ) :
		name( _name )
	{}

	std::shared_ptr<const Temp::CLabel> CName::GetName() const
	{
		return name;
	}

	void CName::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CName::Kids()
	{
		return nullptr;
	}

	IRTree::IExpr* CName::Build( CExprList* kids )
	{
		return this;
	}

	CTemp::CTemp( const std::shared_ptr<Temp::CTemp> _temp ) :
		temp( _temp )
	{}

	std::shared_ptr<Temp::CTemp> CTemp::GetTemp() const
	{
		return temp;
	}

	void CTemp::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CTemp::Kids()
	{
		return nullptr;
	}

	IRTree::IExpr* CTemp::Build( CExprList* kids )
	{
		return this;
	}

	CBinop::CBinop( BINOP _binop, CExprPtr _left, CExprPtr _right ) :
		binop( _binop ),
		left( _left ),
		right( _right )
	{}

	IExpr::BINOP CBinop::GetBinOp( ) const
	{
		return binop;
	}

	const CExprPtr CBinop::GetLeft() const
	{
		return left;
	}

	const CExprPtr CBinop::GetRight() const
	{
		return right;
	}

	void CBinop::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CBinop::Kids()
	{
		return new IRTree::CExprList( left, std::make_shared<IRTree::CExprList>( right, nullptr ) );
	}

	IRTree::IExpr* CBinop::Build( CExprList* kids )
	{
		return new CBinop( binop, kids->GetHead(), kids->GetTail()->GetHead() );
	}

	CMem::CMem( CExprPtr _mem ) :
		mem( _mem )
	{}

	const CExprPtr CMem::GetMem() const
	{
		return mem;
	}

	void CMem::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CMem::Kids()
	{
		return new IRTree::CExprList( mem, nullptr );
	}

	IRTree::IExpr* CMem::Build( CExprList* kids )
	{
		return new CMem( kids->GetHead() );
	}

	CCall::CCall( CExprPtr _funcName, const std::shared_ptr<const CExprList> _arguments ) :
		funcName( _funcName ),
		arguments( _arguments )
	{}

	const CSymbol* CCall::GetFunctionName() const
	{
		return std::dynamic_pointer_cast<const CName>( funcName )->GetName()->GetName();
	}

	const std::shared_ptr<const IRTree::CExprList> CCall::GetArguments() const
	{
		return arguments;
	}

	void CCall::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CCall::Kids()
	{
		return new IRTree::CExprList( funcName, arguments );
	}

	IRTree::IExpr* CCall::Build( CExprList* kids )
	{
		return new CCall( kids->GetHead(), kids->GetTail() );
	}

	CESeq::CESeq( CStmPtr _statement, CExprPtr _expression ) :
		statement( _statement ),
		expression( _expression )
	{}

	const CStmPtr CESeq::GetStatement() const
	{
		return statement;
	}

	const CExprPtr CESeq::GetExpression() const
	{
		return expression;
	}

	void CESeq::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CESeq::Kids()
	{
		throw std::logic_error( "Kids() not applicable to ESEQ" );
	}

	IRTree::IExpr* CESeq::Build( CExprList* kids )
	{
		throw std::logic_error( "Build() not applicable to ESEQ" );
	}

	CExprList::CExprList( std::shared_ptr<const IExpr> _head, std::shared_ptr<const CExprList> _tail ) :
		head( _head ), tail( _tail )
	{}

	std::shared_ptr<const IExpr> CExprList::GetHead() const
	{
		return head;
	}

	std::shared_ptr<const CExprList> CExprList::GetTail() const
	{
		return tail;
	}

}