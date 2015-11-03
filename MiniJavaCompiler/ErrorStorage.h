#pragma once
#include <string>
#include <vector>

class CErrorStorage {
public:
	void PutError( const std::string& error );

private:
	std::vector<std::string> errors;
};