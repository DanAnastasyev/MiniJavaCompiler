#include "iostream"

extern int yyparse();

int main( int argc, char **argv )
{
	freopen( argv[1], "r", stdin );
	yyparse();
	return 0;
}