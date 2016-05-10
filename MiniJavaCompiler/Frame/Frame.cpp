#include "Frame/Frame.h"
#include "CodeGeneration/CodeGeneration.h"

namespace Frame {

	CInFrame::CInFrame( int _offset ) :
		offset( _offset )
	{}

	const IRTree::IExprPtr CInFrame::GetExp( const Frame::CFrame& framePtr ) const
	{
		return IRTree::IExprPtr( new IRTree::CMem( std::shared_ptr<const IRTree::IExpr>( 
			new IRTree::CBinop( IRTree::CBinop::MINUS,
			std::shared_ptr<const IRTree::IExpr>( new IRTree::CTemp( framePtr.GetFramePtr() ) ),
			IRTree::IExprPtr( new IRTree::CBinop( IRTree::CBinop::MUL, 
			IRTree::IExprPtr( new IRTree::CConst( offset + 1 ) ), IRTree::IExprPtr( new IRTree::CConst( framePtr.WORD_SIZE ) ) ) ) ) ) ) );
	}

	const IRTree::IExprPtr CInObject::GetExp( const Frame::CFrame& frameRegPtr ) const
	{
		return IRTree::IExprPtr( new IRTree::CMem(
			IRTree::IExprPtr( new IRTree::CBinop(
			IRTree::CBinop::PLUS, IRTree::IExprPtr( new IRTree::CTemp( frameRegPtr.GetThisPtr() ) ),
			IRTree::IExprPtr( new IRTree::CBinop( IRTree::CBinop::MUL,
			IRTree::IExprPtr( new IRTree::CConst( offset ) ), 
			IRTree::IExprPtr( new IRTree::CConst( frameRegPtr.WORD_SIZE ) ) ) ) ) ) ) );
	}

	const IRTree::IExprPtr CFormalParamInStack::GetExp( const Frame::CFrame& frameRegPtr ) const
	{
		return IRTree::IExprPtr( new IRTree::CMem(
			IRTree::IExprPtr( new IRTree::CBinop(
			IRTree::CBinop::PLUS, IRTree::IExprPtr( new IRTree::CTemp( frameRegPtr.GetFramePtr() ) ),
			IRTree::IExprPtr( new IRTree::CBinop( IRTree::CBinop::MUL,
			IRTree::IExprPtr( new IRTree::CConst( offset ) ),
			IRTree::IExprPtr( new IRTree::CConst( frameRegPtr.WORD_SIZE ) ) ) ) ) ) ) );
	}

	CFrame::CFrame( const SymbolsTable::CClassInfo* classInfo, const SymbolsTable::CMethodInfo* methodInfo,
		const SymbolsTable::CTable* table ) :
		registers( { "EAX", "EBX", "ECX", "EDX", "EEX", "EFX" } )
	{
		frameName = CSymbol::GetSymbol( classInfo->GetName() + "__" + methodInfo->GetName() );
		do {
			// Поля класса
			for( int i = 0; i < classInfo->GerVars().size(); ++i ) {
				locals[classInfo->GerVars()[i]->GetName()] = std::shared_ptr<IAccess>( new CInObject( i ) );
			}
		} while( classInfo->GetBaseClass() != nullptr );

		// Локальные переменные метода
		for( int i = 0; i < methodInfo->GetLocals().size(); ++i ) {
			locals[methodInfo->GetLocals()[i]->GetName()] = std::shared_ptr<IAccess>( new CInFrame( i ) );
		}
		// Параметры метода
		for( int i = 0; i < methodInfo->GetParams().size(); ++i ) {
			formals[methodInfo->GetParams()[i]->GetName()] = std::shared_ptr<IAccess>( new CFormalParamInStack( i ) );
		}

		framePtr = std::make_shared<Temp::CTemp>( CSymbol::GetSymbol( frameName->GetString() + "__FP" ) );
		thisPtr = std::make_shared<Temp::CTemp>( CSymbol::GetSymbol( frameName->GetString() + "__TP" ) );
		returnPtr = std::make_shared<Temp::CTemp>( CSymbol::GetSymbol( frameName->GetString() + "__RP" ) );
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

	const CSymbol* CFrame::GetName() const
	{
		return frameName;
	}

	const std::vector<const std::string>& CFrame::Registers() const {
		return registers;
	}

	size_t CFrame::GetLocalCount() const
	{
		return locals.size();
	}

	std::shared_ptr<const IRTree::IStm> CFrame::GetRootStm() const
	{
		return root;
	}

	std::list<const Assembler::CBaseInstruction*> CFrame::GenerateCode( IRTree::CStmListPtr stmList ) const
	{
		return Assembler::CCodeGeneration( this ).GenerateCode( stmList );
	}
}
