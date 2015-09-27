#include "iostream"

extern "C" {
	int yylex();
}

int main( int argc, char **argv ) {
	freopen( argv[1], "r", stdin );
	yylex();
	return 0;
}