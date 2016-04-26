#include "IRTree/Expressions.h"

namespace IRTree {
	CConst::CConst( int _value ) :
		value( _value )
	{
	}

	int CConst::GetValue() const
	{
		return value;
	}

	void CConst::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CConst::Kids() const
	{
		return nullptr;
	}

	IExpr* CConst::Build( const CExprList* kids ) const
	{
		return new CConst( value );
	}

	CName::CName( std::shared_ptr<const Temp::CLabel> _name ) :
		name( _name )
	{
	}

	std::shared_ptr<const Temp::CLabel> CName::GetName() const
	{
		return name;
	}

	void CName::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CName::Kids() const
	{
		return nullptr;
	}

	IExpr* CName::Build( const CExprList* kids ) const
	{
		return new CName( name );
	}

	CTemp::CTemp( std::shared_ptr<const Temp::CTemp> _temp ) :
		temp( _temp )
	{
	}

	std::shared_ptr<const Temp::CTemp> CTemp::GetTemp() const
	{
		return temp;
	}

	void CTemp::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	IRTree::CExprList* CTemp::Kids() const
	{
		return nullptr;
	}

	IRTree::IExpr* CTemp::Build( const CExprList* kids ) const
	{
		return new CTemp( temp );
	}

	CBinop::CBinop( BINOP _binop, IExprPtr _left, IExprPtr _right ) :
		binop( _binop ),
		left( _left ),
		right( _right )
	{
	}

	int CBinop::GetBinOp( ) const {
		return binop;
	}

	std::shared_ptr<const IExpr> CBinop::GetLeft() const
	{
		return left;
	}

	std::shared_ptr<const IExpr> CBinop::GetRight() const
	{
		return right;
	}

	void CBinop::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CBinop::Kids() const
	{
		return new IRTree::CExprList( left, std::make_shared<IRTree::CExprList>( right, nullptr ) );
	}

	IExpr* CBinop::Build( const CExprList* kids ) const
	{
		return new CBinop( BINOP(binop), kids->GetHead(), kids->GetTail()->GetHead() );
	}

	CMem::CMem( IExprPtr _mem ) :
		mem( _mem )
	{
	}

	std::shared_ptr<const IExpr> CMem::GetMem() const
	{
		return mem;
	}

	void CMem::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CMem::Kids() const
	{
		return new IRTree::CExprList( mem, nullptr );
	}

	IExpr* CMem::Build( const CExprList* kids ) const
	{
		return new CMem( kids->GetHead() );
	}

	CCall::CCall( IExprPtr _funcName, const std::shared_ptr<const CExprList> _arguments ) :
		funcName( _funcName ),
		arguments( _arguments )
	{
	}

	const CSymbol* CCall::GetFunctionName() const
	{
		return std::dynamic_pointer_cast<const CName>(funcName)->GetName()->GetName();
	}

	const std::shared_ptr<const CExprList>& CCall::GetArguments() const
	{
		return arguments;
	}

	void CCall::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CCall::Kids() const
	{
		return new IRTree::CExprList( funcName, arguments );
	}

	IExpr* CCall::Build( const CExprList* kids ) const
	{
		return new CCall( kids->GetHead(), kids->GetTail() );
	}

	CESeq::CESeq( IStmPtr _statement, IExprPtr _expression ) :
		statement( _statement ),
		expression( _expression )
	{
	}

	IStmPtr CESeq::GetStatement() const
	{
		return statement;
	}

	std::shared_ptr<const IExpr> CESeq::GetExpression() const
	{
		return expression;
	}

	void CESeq::Accept( IIRTreeVisitor* visitor ) const
	{
		visitor->Visit( this );
	}

	CExprList* CESeq::Kids() const
	{
		throw std::logic_error( "Kids() not applicable to ESEQ" );
	}

	IExpr* CESeq::Build( const CExprList* kids ) const
	{
		throw std::logic_error( "Build() not applicable to ESEQ" );
	}

	CExprList::CExprList( std::shared_ptr<const IExpr> _head, std::shared_ptr<const CExprList> _tail ) :
		head( _head ), tail( _tail )
	{
	}

	std::shared_ptr<const IExpr> CExprList::GetHead() const
	{
		return head;
	}

	std::shared_ptr<const CExprList> CExprList::GetTail() const
	{
		return tail;
	}
}