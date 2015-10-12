%option noyywrap
%option yylineno

%{
#pragma once
#include "parser.tab.hpp"
#define YY_DECL extern "C" int yylex()
%}

LETTER			[a-zA-Z]
DIGIT			[0-9]
ID				({LETTER})({LETTER}|{DIGIT}|_)*
INT				-?{DIGIT}+
BOOL			"true"|"false"
BINOP			[-+*/{}()\]\[!;,=.<]
SPACE 			[ \t]+
LINE_SEPARATOR	[\n]+

%%

if          {    return IF;	}
else		{ 	 return ELSE;	}
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
System.out.println	{	return PRINTLN;	}
this		{	 return THIS;	}
length		{	 return LENGTH;	}
new			{	 return NEW;	}
"&&"		{	 return AND;	}
{BINOP}		{	 return yytext[0];	}
{BOOL}		{	 return	BOOLEAN_VAL; }
{INT}		{	 return INTEGER_VAL; }
{ID}		{	 return IDENTIFIER; }
{SPACE}		{ }
{LINE_SEPARATOR} {	}
.			{	}

%%