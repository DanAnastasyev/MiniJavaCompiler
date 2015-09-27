#pragma once

class CLexicalScanner {
public:
	CLexicalScanner();

	void getToken( const char* token, const char* yytext );
	void getValue( const char* token, const char* yytext );
	void getNewLine( const char* yytext );
	void getSpace( const char* yytext );
	void getErrorToken( const char* yytext );
private:
	int lineNumber;
	int offsetInLineNumber;
};

inline CLexicalScanner::CLexicalScanner() : lineNumber(0), offsetInLineNumber(0) {}