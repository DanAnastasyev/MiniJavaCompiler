#pragma once

class CLexicalScanner {
public:
	CLexicalScanner();

	const char* getToken( const char* token, const char* yytext );
	const char* getValue( const char* token, const char* yytext );
	const char* getNewLine( const char* yytext );
	const char* getSpace( const char* yytext );
	const char* getErrorToken( const char* yytext );
private:
	int lineNumber;
	int offsetInLineNumber;
};

inline CLexicalScanner::CLexicalScanner() : lineNumber(0), offsetInLineNumber(0) {}