#pragma once

#include <Frame/Frame.h>
#include <Frame/Temp.h>

namespace Assembler {

	class CBaseInstruction {
	public:
		CBaseInstruction( const std::string& assemblerLine ) :
			assemblerInstruction( assemblerLine )
		{}

		virtual ~CBaseInstruction() {}

		virtual Temp::CTempListPtr UsedVars() const = 0;
		virtual Temp::CTempListPtr DefindedVars() const = 0;
		virtual Temp::CLabelListPtr JumpTargets() const = 0;

		std::string Format( Temp::CTempListPtr varsMapping ) const;

		std::string GetAssemblerInstruction() const { return assemblerInstruction; }
		void SetAssemblerInstruction( const std::string& assemblerInstr ) { assemblerInstruction = assemblerInstr; }
	protected:
		std::string assemblerInstruction;
	private:
		std::shared_ptr<const Temp::CTemp> getNthTemp( Temp::CTempListPtr list, int i );
		std::shared_ptr<const Temp::CLabel> getNthLabel( Temp::CLabelListPtr list, int i );
	};
	typedef std::shared_ptr<const CBaseInstruction> CBaseInstructionPtr;

	class CBaseInstructionList {
	public:
		CBaseInstructionList( CBaseInstruction* _head, CBaseInstructionList* _tail ) :
			head( _head ), tail( _tail )
		{}

		CBaseInstruction* head;
		CBaseInstructionList* tail;
	};
	typedef std::shared_ptr<const CBaseInstructionList> CBaseInstructionListPtr;

	class CLabel : public CBaseInstruction {
	public:
		CLabel( const std::string& instructLine, std::shared_ptr<const Temp::CLabel> _label ) :
			CBaseInstruction( instructLine ), label( _label )
		{}

		Temp::CTempListPtr UsedVars() const override
		{
			return nullptr;
		}

		Temp::CTempListPtr DefindedVars() const override
		{
			return nullptr;
		}

		Temp::CLabelListPtr JumpTargets() const override
		{
			return std::make_shared<Temp::CLabelList>( label, nullptr );
		}
	private:
		std::shared_ptr<const Temp::CLabel> label;
	};

	class CMove : public CBaseInstruction {
	public:
		CMove( const std::string& assemblerLine, std::shared_ptr<const Temp::CTemp> dest,
			std::shared_ptr<const Temp::CTemp> src ) :
			CBaseInstruction( assemblerLine ), destTemp( dest ), srcTemp( src )
		{}

		Temp::CTempListPtr UsedVars() const override
		{
			return std::make_shared<Temp::CTempList>( srcTemp, nullptr );
		}

		Temp::CTempListPtr DefindedVars() const override
		{
			return std::make_shared<Temp::CTempList>( destTemp, nullptr );
		}

		Temp::CLabelListPtr JumpTargets() const override
		{
			return nullptr;
		}

	private:
		std::shared_ptr<const Temp::CTemp> destTemp;
		std::shared_ptr<const Temp::CTemp> srcTemp;
	};

	class COper : public CBaseInstruction {
	public:
		COper( const std::string& assemblerLine, Temp::CTempListPtr dest, Temp::CTempListPtr src, Temp::CLabelListPtr jump ) :
			CBaseInstruction( assemblerLine ), destTempList( dest ), srcTempList( src ), jumpTargets( jump )
		{}

		COper( const std::string& assemblerLine, Temp::CTempListPtr dest, Temp::CTempListPtr src ) :
			CBaseInstruction( assemblerLine ), destTempList( dest ), srcTempList( src )
		{}

		Temp::CTempListPtr UsedVars() const override
		{
			return srcTempList;
		}

		Temp::CTempListPtr DefindedVars() const override
		{
			return destTempList;
		}

		Temp::CLabelListPtr JumpTargets() const override
		{
			return jumpTargets;
		}
		std::string GetOperator() const { return assemblerInstruction.substr(0, assemblerInstruction.find(' ')); }
	private:
		Temp::CLabelListPtr jumpTargets;
		Temp::CTempListPtr destTempList;
		Temp::CTempListPtr srcTempList;
	};
}