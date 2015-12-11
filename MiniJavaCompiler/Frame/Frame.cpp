#include "Frame\Frame.h"

namespace Frame {

	CInFrame::CInFrame( int _offset ) :
		offset( _offset )
	{}

	const IRTree::CExprPtr CInFrame::GetExp( const std::shared_ptr<Temp::CTemp> framePtr ) const
	{
		int machineOffset = offset * CFrame::WORD_SIZE;

		return IRTree::CExprPtr( new IRTree::CMem( std::shared_ptr<const IRTree::IExpr>( new IRTree::CBinop( IRTree::CBinop::PLUS,
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CTemp( framePtr ) ),
			IRTree::CExprPtr( new IRTree::CConst( machineOffset ) ) ) ) ) );
	}

	CInReg::CInReg()
	{}

	const IRTree::CExprPtr CInReg::GetExp( const std::shared_ptr<Temp::CTemp> frameRegPtr ) const
	{
		return IRTree::CExprPtr( new IRTree::CTemp( temp ) );
	}

	CFrame::CFrame( const SymbolsTable::CClassInfo* classInfo, const SymbolsTable::CMethodInfo* methodInfo, 
		const SymbolsTable::CTable* table )
	{
		frameName = CSymbol::GetSymbol( classInfo->GetName() + "$" + methodInfo->GetName() );
		do {
			for( auto var : classInfo->GerVars() ) {
				locals[var->GetName()] = std::shared_ptr<IAccess>( new CInReg() );
			}
		} while( classInfo->GetBaseClass() != NULL );

		for( auto var : methodInfo->GetLocals() ) {
			locals[var->GetName()] = std::shared_ptr<IAccess>( new CInReg( ) );
		}
		for( auto var : methodInfo->GetParams() ) {
			formals[var->GetName()] = std::shared_ptr<IAccess>( new CInReg( ) );
		}
	}

	void CFrame::SetStatements( std::shared_ptr<const IRTree::IStm> statements )
	{
		root = statements;
	}

	std::shared_ptr<IAccess> CFrame::GetVar( const CSymbol* name ) const
	{
		if( formals.find( name->GetString() ) != formals.end() ) {
			return formals.find( name->GetString() )->second;
		}
		if( locals.find( name->GetString() ) != locals.end() ) {
			return locals.find( name->GetString() )->second;
		}
		return 0;
	}

	std::shared_ptr<Temp::CTemp> CFrame::GetFramePtr() const
	{
		return framePtr;
	}

	std::shared_ptr<Temp::CTemp> CFrame::GetThisPtr() const
	{
		return thisPtr;
	}

	std::shared_ptr<Temp::CTemp> CFrame::GetReturnPtr() const
	{
		return returnPtr;
	}

}