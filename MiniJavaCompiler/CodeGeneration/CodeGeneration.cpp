#include <CodeGeneration/CodeGeneration.h>

using namespace IRTree;
using namespace Assembler;

#define INSTANCEOF(instance, clazz) (std::dynamic_pointer_cast<const clazz>( instance ) != nullptr)
#define CAST(instance, clazz) (std::dynamic_pointer_cast<const clazz>(instance))

std::list<const Assembler::CBaseInstruction*> CCodeGeneration::getList( Assembler::CBaseInstructionList* list )
{
	std::list<const Assembler::CBaseInstruction*> newList;
	for( ; list != nullptr; list = list->tail ) {
		newList.push_back( list->head );
	}
	return newList;
}

std::list<const Assembler::CBaseInstruction*> CCodeGeneration::GenerateCode( IRTree::CStmListPtr stmList )
{
	while( stmList != nullptr ) {
		auto stm = stmList->GetHead();
		munchStm( stm );
		stmList = stmList->GetTail();
	}
	Assembler::CBaseInstructionList* list = instructList;
	instructList = last = nullptr;
	return getList( list );
}

void CCodeGeneration::emit( Assembler::CBaseInstruction* instruct )
{
	if( last != nullptr ) {
		last = last->tail = new Assembler::CBaseInstructionList( instruct, nullptr );
	} else {
		last = instructList = new Assembler::CBaseInstructionList( instruct, nullptr );
	}
}

void CCodeGeneration::munchStm(CSeqPtr stm)
{
	munchStm( stm->GetLeftStm() );
	munchStm( stm->GetRightStm() );
}

void CCodeGeneration::munchStm( CMovePtr stm )
{
	// MOVE( mem(...) )
	if( INSTANCEOF( stm->GetDestExpr(), CMem ) ) {
		auto destMem = std::dynamic_pointer_cast<const CMem>( stm->GetDestExpr() );
		
		if( INSTANCEOF( destMem->GetMem(), CBinop ) ) {
			auto binOp = std::dynamic_pointer_cast<const CBinop>( destMem->GetMem() );
			if( binOp->GetBinOp() == IExpr::PLUS || binOp->GetBinOp() == IExpr::MINUS ) {
				// MOVE( mem( BINOP(PLUS, e1, CONST(i)), e2 ) )
				if( INSTANCEOF( binOp->GetLeft( ), CConst ) || INSTANCEOF( binOp->GetRight( ), CConst ) ) {
					IExprPtr binOpExpr;
					CConstPtr constantExpr;
					if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
						binOpExpr = binOp->GetRight();
						constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft() );
					} else {
						binOpExpr = binOp->GetLeft();
						constantExpr = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() );
					}
					emit( new Assembler::COper( std::string( "mov ['d0" ) +
						( ( binOp->GetBinOp() == IExpr::PLUS ) ? "+" : "-" ) +
						std::to_string( constantExpr->GetValue() ) +
						std::string( "], 's0\n" ),
						nullptr,
						std::make_shared<const Temp::CTempList>(
						munchExp( binOpExpr ),
						std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
				} else {
					// MOVE( mem( BINOP( PLUS, e1, e2 ) ), e3 )
					emit( new Assembler::COper( std::string( "mov ['d0], 's0\n" ),
						std::make_shared<const Temp::CTempList>( munchExp( binOp ), nullptr ),
						std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
				}
			}
		} else if( INSTANCEOF( destMem->GetMem(), CConst ) ) {
			// MOVE( mem( CONST(i) ), e2 )
			CConstPtr constantExpr = std::dynamic_pointer_cast<const CConst>( destMem->GetMem() );
			emit( new Assembler::COper( std::string( "mov ['d0+" ) +
				std::to_string( constantExpr->GetValue( ) ) +
				std::string( "], 's0\n" ),
				nullptr,
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
		} else if( INSTANCEOF( destMem->GetMem(), CTemp ) ) {
			// MOVE( mem( TEMP ), e2 )
			emit( new Assembler::COper( std::string( "mov ['d0], 's0\n" ),
				std::make_shared<const Temp::CTempList>( munchExp( destMem->GetMem() ), nullptr ),
				std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
		} else if( INSTANCEOF( destMem->GetMem( ), CMem ) ) {
			if( INSTANCEOF( stm->GetSrcExpr(), CMem ) ) {
				// MOVE( mem(e1), mem(e2) )
				emit( new Assembler::COper( std::string( "mov ['d0], ['s0]\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>( 
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			} else {
				// MOVE( mem(e1), e2 )
				emit( new Assembler::COper( std::string( "mov ['d0], 's0\n" ),
					nullptr,
					std::make_shared<const Temp::CTempList>(
					munchExp( stm->GetSrcExpr() ),
					std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) ) );
			}
		}
	} else if( INSTANCEOF( stm->GetDestExpr(), CTemp ) ) { 
		// MOVE(TEMP(i), e2)
		CTempPtr temp = std::dynamic_pointer_cast<const CTemp>( stm->GetDestExpr() );
		emit( new Assembler::COper( "mov 'd0, 's0\n",
			std::make_shared<const Temp::CTempList>( temp->GetTemp(), nullptr ),
			std::make_shared<const Temp::CTempList>( munchExp( stm->GetSrcExpr() ), nullptr ) ) );
	}
}

void CCodeGeneration::munchStm(IRTree::CLabelPtr stm)
{
	emit( new Assembler::CLabel( stm->GetLabel()->GetName()->GetString() + std::string(":\n"), stm->GetLabel() ) );
}

void CCodeGeneration::munchStm( IRTree::CExprPtr stm )
{
	munchExp( stm->GetExp() );
}

void CCodeGeneration::munchStm( IRTree::CJumpPtr stm )
{
	emit( new Assembler::COper( "jmp 'j0\n", nullptr, nullptr, stm->GetTargets() ) );
}

void CCodeGeneration::munchStm( IRTree::CCondJumpPtr stm )
{
	if( !INSTANCEOF( stm->GetExpr(), CBinop ) ) {
		throw std::exception( "wrong conditional jump" );
	}
	auto binOp = CAST( stm->GetExpr(), CBinop );
	auto leftTemp = munchExp( binOp->GetLeft() );
	auto rightTemp = munchExp( binOp->GetRight() );
	emit( new Assembler::COper( "cmp 's0, 's1\n", nullptr, std::make_shared<const Temp::CTempList>( leftTemp,
		std::make_shared<const Temp::CTempList>( rightTemp, nullptr ) ) ) );

	// TODO считаем, что CCondJump может идти только по < и >= (появляется в TraceSchedule)
	std::string oper;
	switch( binOp->GetBinOp() ) {
		case IExpr::LESS: oper = "jl"; break;
		case IExpr::GE: oper = "jge"; break;
	}
	emit( new Assembler::COper( oper + " 'l0\n", nullptr, nullptr, 
		std::make_shared<const Temp::CLabelList>( stm->GetIfTrueLabel(), nullptr ) ) );
}

void CCodeGeneration::munchStm( IRTree::IStmPtr stm )
{
	if( INSTANCEOF( stm, CSeq ) ) {
		munchStm( CAST( stm, CSeq ) );
	} else if( INSTANCEOF( stm, IRTree::CMove ) ) {
		munchStm( CAST( stm, IRTree::CMove ) );
	} else if( INSTANCEOF( stm, IRTree::CLabel ) ) {
		munchStm( CAST( stm, IRTree::CLabel ) );
	} else if( INSTANCEOF( stm, CExpr ) ) {
		munchStm( CAST( stm, CExpr ) );
	} else if( INSTANCEOF( stm, CJump ) ) {
		munchStm( CAST( stm, CJump ) );
	} else if( INSTANCEOF( stm, CCondJump ) ) {
		munchStm( CAST( stm, CCondJump ) );
	}
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CMemPtr expr )
{
	auto temp = std::make_shared<const Temp::CTemp>();
	emit( new Assembler::CMove( "mov 'd0, ['s0]\n", temp, munchExp( expr->GetMem() ) ) );
	return temp;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExpJump( IRTree::CBinopPtr binOp )
{
	auto temp = std::make_shared<Temp::CTemp>();
	emit( new Assembler::COper( "mov 'd0, 0\n", std::make_shared<const Temp::CTempList>( temp, nullptr ), nullptr ) );

	auto left = std::make_shared<Temp::CTemp>();
	auto right = std::make_shared<Temp::CTemp>();
	emit( new Assembler::CMove( "mov 'd0, 's0\n", left, munchExp( binOp->GetLeft() ) ) );
	emit( new Assembler::CMove( "mov 'd0, 's0\n", right, munchExp( binOp->GetRight() ) ) );

	auto source = std::make_shared<const Temp::CTempList>( left, std::make_shared<const Temp::CTempList>( right, nullptr ) );

	emit( new COper( "cmp 's0, s1\n", nullptr, source ) );

	auto label = std::make_shared<const Temp::CLabel>( );

	emit( new COper( "jnl 'l0\n", nullptr, nullptr, std::make_shared<const Temp::CLabelList>( label, nullptr ) ) );
	emit( new COper( "mov 'd0, 1\n", std::make_shared<const Temp::CTempList>( temp, nullptr ), nullptr ) );
	emit( new CLabel( label->GetName()->GetString() + ":\n", label ) );

	return temp;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CBinopPtr binOp )
{
	if( binOp->GetBinOp() == CBinop::LESS || binOp->GetBinOp() == CBinop::GE ) {
		munchExpJump( binOp );
	}

	if( INSTANCEOF( binOp->GetLeft(), CConst ) && INSTANCEOF( binOp->GetRight(), CConst ) ) {
		// const-const
		int leftVal = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft( ) )->GetValue();
		int rightVal = std::dynamic_pointer_cast<const CConst>( binOp->GetRight( ) )->GetValue();
		auto temp = std::make_shared<const Temp::CTemp>();
		// пишем в frame->GetEAX() левую константу
		emit( new Assembler::CMove( "mov 'd0, " + std::to_string( leftVal ) + "\n", frame->GetEAX(), nullptr ) );
		if( binOp->GetBinOp() == CBinop::MUL ) {
			emit( new Assembler::CMove( "mov 'd0, 0\n", frame->GetEDX(), nullptr ) );
		}
		if( binOp->GetBinOp() == CBinop::PLUS ) {
			emit( new Assembler::COper( "add 'd0, " + std::to_string( rightVal ) + "\n",
				std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr ), 
				nullptr ) );
		} else if( binOp->GetBinOp( ) == CBinop::MINUS ) {
			emit( new Assembler::COper( "sub 'd0, " + std::to_string( rightVal ) + "\n",
				std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr ),
				nullptr ) );
		} else if( binOp->GetBinOp() == CBinop::MUL ) {
			emit( new Assembler::COper( "mul " + std::to_string( rightVal ) + "\n", 
				std::make_shared<const Temp::CTempList>( frame->GetEAX(), std::make_shared<const Temp::CTempList>( frame->GetEDX(), nullptr ) ), 
				nullptr ) );
		}
		emit( new Assembler::CMove( "mov 'd0, 's0\n\n", temp, frame->GetEAX() ) );
		return temp;
	}
	if( INSTANCEOF( binOp->GetLeft(), CConst ) ) {
		// const-expr
		int leftVal = std::dynamic_pointer_cast<const CConst>( binOp->GetLeft( ) )->GetValue( );
		auto temp = std::make_shared<const Temp::CTemp>( );
		auto rightTemp = munchExp( binOp->GetRight() );
		// пишем в frame->GetEAX() левую константу
		emit( new Assembler::CMove( "mov 'd0, " + std::to_string( leftVal ) + "\n", frame->GetEAX(), nullptr ) );
		// запоминаем правое
		emit( new Assembler::CMove( "mov 'd0, 's0\n", temp, rightTemp ) );
		if( binOp->GetBinOp( ) == CBinop::MUL ) {
			emit( new Assembler::CMove( "mov 'd0, 0\n", frame->GetEDX(), nullptr ) );
		}
		std::shared_ptr<const Temp::CTempList> usedRegisters;
		if( binOp->GetBinOp( ) == CBinop::PLUS ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
			emit( new Assembler::COper( "add 'd0, 's0\n", usedRegisters,
				std::make_shared<const Temp::CTempList>( temp, nullptr ) ) );
		} else if( binOp->GetBinOp( ) == CBinop::MINUS ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
			emit( new Assembler::COper( "sub 'd0, 's0\n", usedRegisters,
				std::make_shared<const Temp::CTempList>( temp, nullptr ) ) );
		} else if( binOp->GetBinOp( ) == CBinop::MUL ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), std::make_shared<const Temp::CTempList>( frame->GetEDX(), nullptr ) );
			emit( new Assembler::COper( "mul 's0\n", usedRegisters,
				std::make_shared<const Temp::CTempList>( temp, nullptr ) ) );
		}
		auto temp2 = std::make_shared<const Temp::CTemp>();
		emit( new Assembler::CMove( "mov 'd0, 's0\n\n", temp2, usedRegisters->Head() ) );
		return temp2;
	}
	if( INSTANCEOF( binOp->GetRight(), CConst ) ) {
		// expr-const
		int rightVal = std::dynamic_pointer_cast<const CConst>( binOp->GetRight() )->GetValue();
		auto leftTemp = munchExp( binOp->GetLeft() );
		auto temp = std::make_shared<const Temp::CTemp>();
		// запоминаем левое
		emit( new Assembler::CMove( "mov 'd0, 's0\n", frame->GetEAX(), leftTemp ) );
		if( binOp->GetBinOp( ) == CBinop::MUL ) {
			emit( new Assembler::CMove( "mov 'd0, 0\n", frame->GetEDX(), nullptr ) );
		}
		std::shared_ptr<const Temp::CTempList> usedRegisters;
		if( binOp->GetBinOp( ) == CBinop::PLUS ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
			emit( new Assembler::COper( "add 'd0, " + std::to_string( rightVal ) + "\n", usedRegisters,
				nullptr ) );
		} else if( binOp->GetBinOp( ) == CBinop::MINUS ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
			emit( new Assembler::COper( "sub 'd0, " + std::to_string( rightVal ) + "\n", usedRegisters,
				nullptr ) );
		} else if( binOp->GetBinOp( ) == CBinop::MUL ) {
			usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), std::make_shared<const Temp::CTempList>( frame->GetEDX(), nullptr ) );
			emit( new Assembler::COper( "mul " + std::to_string( rightVal ) + "\n", usedRegisters,
				nullptr ) );
		}
		emit( new Assembler::CMove( "mov 'd0, 's0\n\n", temp, usedRegisters->Head() ) );
		return temp;
	}
	// expr-expr
	auto temp1 = std::make_shared<const Temp::CTemp>();
	auto temp2 = std::make_shared<const Temp::CTemp>();
	auto leftTemp = munchExp( binOp->GetLeft() );
	auto rightTemp = munchExp( binOp->GetRight() );
	// пишем в frame->GetEAX() левую константу
	emit( new Assembler::CMove( "mov 'd0, 's0\n", frame->GetEAX(), leftTemp ) );
	// запоминаем правое
	emit( new Assembler::CMove( "mov 'd0, 's0\n", temp2, rightTemp ) );
	if( binOp->GetBinOp( ) == CBinop::MUL ) {
		emit( new Assembler::CMove( "mov 'd0, 0\n", frame->GetEDX(), nullptr ) );
	}
	std::shared_ptr<const Temp::CTempList> usedRegisters;
	if( binOp->GetBinOp( ) == CBinop::PLUS ) {
		usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
		emit( new Assembler::COper( "add 'd0, 's0\n", usedRegisters,
			std::make_shared<const Temp::CTempList>( temp2, nullptr ) ) );
	} else if( binOp->GetBinOp( ) == CBinop::MINUS ) {
		usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), nullptr );
		emit( new Assembler::COper( "sub 'd0, 's0\n", usedRegisters,
			std::make_shared<const Temp::CTempList>( temp2, nullptr ) ) );
	} else if( binOp->GetBinOp( ) == CBinop::MUL ) {
		usedRegisters = std::make_shared<const Temp::CTempList>( frame->GetEAX(), std::make_shared<const Temp::CTempList>( frame->GetEDX(), nullptr ) );
		emit( new Assembler::COper( "mul 's0\n", usedRegisters,
			std::make_shared<const Temp::CTempList>( temp2, nullptr ) ) );
	}
	emit( new Assembler::CMove( "mov 'd0, 's0\n\n", temp1, usedRegisters->Head( ) ) );
	return temp1;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CConstPtr constantExpr )
{
	// TODO проверить
	std::shared_ptr<const Temp::CTemp> temp( new Temp::CTemp );
	
	emit( new Assembler::COper( std::string( "mov 'd0, " ) +
		std::to_string( constantExpr->GetValue() ) +
		std::string( "\n" ),
		std::make_shared<const Temp::CTempList>( temp, nullptr ), nullptr ) );

	return temp;
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CTempPtr expr )
{
	return expr->GetTemp();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CNamePtr expr )
{
	return std::make_shared<const Temp::CTemp>();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::CCallPtr expr )
{
	auto temps = munchArgs( expr->GetArguments() );
	//if( !temps.empty() ) {
	//	for( auto t : temps ) {
	//		emit( new Assembler::COper( "push 's0\n", nullptr,
	//			std::make_shared<const Temp::CTempList>( t, nullptr ) ) );
	//	}
	//}
	emit( new Assembler::COper( "call 'l0\n",
		std::make_shared<const Temp::CTempList>( frame->GetEAX( ), std::make_shared<const Temp::CTempList>( frame->GetEDX( ), nullptr ) ),
		nullptr,
		std::make_shared<const Temp::CLabelList>( std::make_shared<const Temp::CLabel>( expr->GetFunctionName() ), nullptr ) ) );
	return frame->GetEAX();
}

std::shared_ptr<const Temp::CTemp> CCodeGeneration::munchExp( IRTree::IExprPtr expr )
{
	if( INSTANCEOF( expr, CMem ) ) {
		return munchExp( CAST( expr, CMem ) );
	}
	if( INSTANCEOF( expr, CBinop ) ) {
		return munchExp( CAST( expr, CBinop ) );
	}
	if( INSTANCEOF( expr, CConst ) ) {
		return munchExp( CAST( expr, CConst ) );
	}
	if( INSTANCEOF( expr, CTemp ) ) {
		return munchExp( CAST( expr, CTemp ) );
	}
	if( INSTANCEOF( expr, CName ) ) {
		return munchExp( CAST( expr, CName ) );
	}
	if( INSTANCEOF( expr, CCall ) ) {
		return munchExp( CAST( expr, CCall ) );
	}
}

std::list<std::shared_ptr<const Temp::CTemp>> CCodeGeneration::munchArgs( std::shared_ptr<const IRTree::CExprList> args )
{
	std::list<std::shared_ptr<const Temp::CTemp>> temps;
	while( args ) {
		temps.push_back( munchExp( args->GetHead( ) ) );
		emit( new Assembler::COper( "push 's0\n", nullptr,
			std::make_shared<const Temp::CTempList>( temps.back(), nullptr ) ) );
		//emit( new Assembler::COper( "mov 'd0 's0\n", 
		//	std::make_shared<const Temp::CTempList>( temps.back(), nullptr ), 
		//	std::make_shared<const Temp::CTempList>( munchExp( args->GetHead() ), nullptr ) ) );
		args = args->GetTail();
	}
	return temps;
}
