#include <vector>
#include <memory>
#include "RuleClasses/RuleClasses.h"
#include <string>

namespace SymbolsTable {
	class CVarInfo {
	public:
		CVarInfo( const std::string& name, const CStandardType::StandardType type ) : varName( name ), standartType( type ) {}
		CVarInfo( const std::string& name, CUserType* type ) : varName( name ), userType( type ) {}

		std::string GetName() const;
	private:
		std::string varName;
		CStandardType::StandardType standartType;
		std::shared_ptr<CUserType> userType;
	};

	class CMethodInfo {
	public:
		CMethodInfo( const std::string& name, const CStandardType::StandardType type ) : methodName( name ), returnType( new CVarInfo( "", type ) ) {}
		CMethodInfo( const std::string& name, CUserType* type ) : methodName( name ), returnType( new CVarInfo( "", type ) ) {}

		bool AddParamVar( const std::string& varName, const CStandardType::StandardType type );
		bool AddParamVar( const std::string& varName, CUserType* type );
		bool AddLocalVar( const std::string& varName, const CStandardType::StandardType type );
		bool AddLocalVar( const std::string& varName, CUserType* type );

		std::string GetName() const;
		CVarInfo* GetVar( const std::string& varName ) const;
		CVarInfo* GetReturnType() const;
	private:
		std::string methodName;
		std::shared_ptr<CVarInfo> returnType;
		std::vector<std::shared_ptr<CVarInfo>> params;
		std::vector<std::shared_ptr<CVarInfo>> locals;
	};

	class CClassInfo {
	public:
		CClassInfo( const std::string& name ) : className( name ) {}

		bool AddVar( const std::string& varName, const CStandardType::StandardType type );
		bool AddVar( const std::string& varName, CUserType* type );
		bool AddMethod( const std::string& methodName, const CStandardType::StandardType type );
		bool AddMethod( const std::string& methodName, CUserType* type );

		std::string GetName() const;
		CMethodInfo* GetMethod( const std::string& methodName ) const;
		CVarInfo* GetVar( const std::string varName ) const;
		std::vector<std::shared_ptr<CMethodInfo>> GetMethods() const;
		std::vector<std::shared_ptr<CVarInfo>> GerVars() const;
	private:
		std::string className;
		std::vector<std::shared_ptr<CVarInfo>> vars;
		std::vector<std::shared_ptr<CMethodInfo>> methods;
	};

	class CTable {
	public:
		bool AddClass( const std::string& className );

		CClassInfo* GetClass( const std::string& className );
	private:
		std::vector<std::shared_ptr<CClassInfo>> classes;
	};
}