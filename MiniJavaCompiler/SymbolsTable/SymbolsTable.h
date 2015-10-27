#include <vector>
#include <memory>
#include "RuleClasses.h"
#include <map>

namespace SymbolsTable {
	
	class CTable {
	public:

	private:
		std::map<std::string, std::shared_ptr<CClassInfo>> classes;
	};

	class CClassInfo {
	public:
	private:
		std::map<std::string, std::shared_ptr<CVarInfo>> vars;
		std::map<std::string, std::shared_ptr<CMethodInfo>> methods;
	};

	class CMethodInfo {
	public:
	private:
		CStandardType::StandardType returnType;
		std::map<std::string, std::shared_ptr<CVarInfo>> params;
		std::map<std::string, std::shared_ptr<CVarInfo>> locals;
	};

	class CVarInfo {
	public:
	private:
		CStandardType::StandardType standartType;
		CUserType userType;
	};

}