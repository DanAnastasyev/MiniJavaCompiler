#include "RuleClasses.h"

CProgram::CProgram( IMainClass* _mainClass, IClassDeclList* _classList, CPosition& position ) :
	mainClass( _mainClass ),
	classList( _classList ),
	position( position )
{
}

const IMainClass* CProgram::GetMainClass() const
{
	return mainClass.get();
}

const IClassDeclList* CProgram::GetClassDeclList() const
{
	return classList.get();
}

void CProgram::Accept( IVisitor* visitor ) const
{
	visitor->Visit( this );
}