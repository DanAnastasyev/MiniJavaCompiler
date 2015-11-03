#include "ErrorStorage.h"

void CErrorStorage::PutError( const std::string& error )
{
	errors.push_back( error );
}