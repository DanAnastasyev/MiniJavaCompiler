#pragma once

#include <string>
#include <vector>

class CErrorStorage {
public:
	void PutError( const std::string& error );
	std::vector<std::string> GetAllErrors() const;

private:
	std::vector<std::string> errors;
};