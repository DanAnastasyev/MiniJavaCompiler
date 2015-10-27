%option noyywrap
%option yylineno

%{
	#pragma once
	#include "parser.tab.hpp"
	#include <iostream>
	#include <cstring>
	#define YY_DECL extern "C" int yylex()
	void yyerror(std::shared_ptr<IProgram>&, const char*);
	int yy_line = 1;
    int yy_column = 1;
	void onNextToken() 
	{
		for( char* symbol = yytext; *symbol != '\0'; ++symbol ) {
			if( *symbol == '\n' ) {
				++yy_line;
				yy_column = 1;
			} else if( *symbol == '\t' ) {
				yy_column += 4;
			} else {
				++yy_column;
			}
		}
        yylloc.first_line = yylloc.last_line;
        yylloc.first_column = yylloc.last_column;
        yylloc.last_line = yy_line;
        yylloc.last_column = yy_column;
    }

	#define YY_USER_ACTION onNextToken();
%}

%%
if          {	return IF;	}
else		{	return ELSE;	}
while		{	 return WHILE;	}
return		{	 return RETURN;	}
extends		{	 return EXTENDS; }	
public		{	 return PUBLIC;	}
class		{	 return CLASS;	}
static		{	 return STATIC;	}
void		{	 return VOID;	}
main		{	 return MAIN;	}
int 		{    return INT;	}
boolean		{    return BOOL;	}
String		{	 return STRING;	}
System.out.println	{	 return PRINTLN;	}
this		{	 return THIS;	}
length		{	 return LENGTH;	}
new			{	 return NEW;	}
"&&"		{	 return AND;	}
[-+*/{}()\]\[!;,=.<]	{	return yytext[0];	}
"true"|"false"		{	yylval.val = new char[strlen(yytext)]; strcpy( yylval.val, yytext ); return BOOLEAN_VAL; }
[0-9]+		{	yylval.val = new char[strlen(yytext)]; strcpy( yylval.val, yytext ); return INTEGER_VAL; }
[a-zA-Z][a-z0-9A-Z_]*	{	 yylval.val = new char[strlen(yytext)]; strcpy( yylval.val, yytext ); return IDENTIFIER; }
[ \t]		{  }
"\n"		{  } 
[/][/].*\n  {  } // comment
.			{ /*yyerror("Unknown symbol\n");*/ }

%%