#include <iostream>

#include "LexicalScanner.h"

void CLexicalScanner::getToken( const char* token, const char* yytext ) 
{
	std::cout << token << " at " << lineNumber << ',' << offsetInLineNumber << std::endl;
	offsetInLineNumber += strlen( yytext );
}

void CLexicalScanner::getValue( const char* token, const char* yytext ) 
{
	std::cout << token << "(" << yytext << ")";
	std::cout << " at " << lineNumber << "," << offsetInLineNumber << std::endl;
	offsetInLineNumber += strlen( yytext );
}

void CLexicalScanner::getNewLine( const char* yytext ) 
{
	lineNumber += strlen( yytext );
	offsetInLineNumber = 0;
}

void CLexicalScanner::getSpace( const char* yytext ) 
{
	offsetInLineNumber += strlen( yytext );
}

void CLexicalScanner::getErrorToken( const char* yytext ) 
{
	std::cerr << "ERROR: unexpected token '" << yytext << "' at line " << lineNumber << " in position " << offsetInLineNumber << std::endl;
}