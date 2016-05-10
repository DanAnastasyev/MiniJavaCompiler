#include "Assembler/BaseInstruction.h"
#include <iostream>

std::string Assembler::CBaseInstruction::Format( const std::map<std::string, std::string>& varsMapping ) const
{
	Temp::CTempListPtr src = UsedVars();
	Temp::CTempListPtr dst = DefindedVars();
	Temp::CLabelListPtr jumps = JumpTargets();

	std::string instr;
	for( int i = 0; i < assemblerInstruction.length(); ++i ) {
		if( assemblerInstruction[i] == '\'' ) {
			bool breakFlag = false;
			switch( assemblerInstruction[++i] ) {
				case 's': {
					int n = assemblerInstruction[++i] - '0';
					instr += varsMapping.at( getNthTemp( src, n )->GetName()->GetString() );
					break;
				}
				case 'd': {
					int n = assemblerInstruction[++i] - '0';
					instr += varsMapping.at( getNthTemp( dst, n )->GetName()->GetString() );
					break;
				}
				case 'j': {
					int n = assemblerInstruction[++i] - '0';
					instr += getNthLabel( jumps, n )->GetName()->GetString();
					break;
				}
				case 'l': {
					instr += (static_cast<const COper*>(this))->JumpTargets()->Head()->GetName()->GetString() + '\n';
					breakFlag = true;
					break;
				}
			}
			if( breakFlag ) {
				break;
			}
		} else {
			instr += assemblerInstruction[i];
		}
	}

	return instr;
}

std::shared_ptr<const Temp::CTemp> Assembler::CBaseInstruction::getNthTemp( Temp::CTempListPtr list, int i ) const
{
	if( i == 0 ) {
		return list->Head();
	}
	return getNthTemp( list->Tail(), i - 1 );
}

std::shared_ptr<const Temp::CLabel> Assembler::CBaseInstruction::getNthLabel( Temp::CLabelListPtr list, int i ) const
{
	if( i == 0 ) {
		return list->Head( );
	}
	return getNthLabel( list->Tail(), i - 1 );
}
