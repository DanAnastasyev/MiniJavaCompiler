#include <iostream>

#include "LexicalScanner.h"

const char* CLexicalScanner::getToken( const char* token, const char* yytext )
{
	std::cout << token << " at " << lineNumber << ',' << offsetInLineNumber << std::endl;
	offsetInLineNumber += strlen( yytext );
	return token;
}

const char* CLexicalScanner::getValue( const char* token, const char* yytext )
{
	std::cout << token << "(" << yytext << ")";
	std::cout << " at " << lineNumber << "," << offsetInLineNumber << std::endl;
	offsetInLineNumber += strlen( yytext );
	return token;
}

const char* CLexicalScanner::getNewLine( const char* yytext )
{
	lineNumber += strlen( yytext );
	offsetInLineNumber = 0;
	return yytext;
}

const char* CLexicalScanner::getSpace( const char* yytext )
{
	offsetInLineNumber += strlen( yytext );
	return yytext;
}

const char* CLexicalScanner::getErrorToken( const char* yytext )
{
	std::cerr << "ERROR: unexpected token '" << yytext << "' at line " << lineNumber << " in position " << offsetInLineNumber << std::endl;
	return yytext;
}