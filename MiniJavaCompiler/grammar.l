%option noyywrap
%option yylineno

%{
	#pragma once
	#include "LexicalScanner/LexicalScanner.h"
	#define YY_DECL extern "C" int yylex()

	CLexicalScanner lexicalScanner;
%}

LETTER			[a-zA-Z]
DIGIT			[0-9]
ID				({LETTER})({LETTER}|{DIGIT}|_)*
INT				-?{DIGIT}+
BOOL			"true"|"false"
BINOP			[-+*/]
SPACE 			[ \t]+
LINE_SEPARATOR	[\n]+

%%

if          {    lexicalScanner.getToken("IF", yytext);	}
else		{ 	 lexicalScanner.getToken("ELSE", yytext);	}
while		{	 lexicalScanner.getToken("WHILE", yytext);	}
return		{	 lexicalScanner.getToken("RETURN", yytext);	}
public		{	 lexicalScanner.getToken("PUBLIC", yytext);	}
class		{	 lexicalScanner.getToken("CLASS", yytext);	}
static		{	 lexicalScanner.getToken("STATIC", yytext);	}
void		{	 lexicalScanner.getToken("VOID", yytext);	}
main		{	 lexicalScanner.getToken("MAIN", yytext);	}
int 		{    lexicalScanner.getToken("INT", yytext);	}
boolean		{    lexicalScanner.getToken("BOOL", yytext);	}
String		{	 lexicalScanner.getToken("STRING", yytext);	}
System.out.println	{	lexicalScanner.getToken("PRINTLN", yytext);	}
new			{	 lexicalScanner.getToken("NEW", yytext);	}
"{"			{	 lexicalScanner.getToken("{", yytext);	}
"}"			{	 lexicalScanner.getToken("}", yytext);	}
"("			{	 lexicalScanner.getToken("(", yytext);	}
")"			{	 lexicalScanner.getToken(")", yytext);	}
"["			{	 lexicalScanner.getToken("[", yytext);	}
"]"			{	 lexicalScanner.getToken("]", yytext);	}
"<"			{	 lexicalScanner.getToken("<", yytext);	}
"<="		{	 lexicalScanner.getToken("LESSEQ", yytext);	}
">"			{	 lexicalScanner.getToken(">", yytext);	}
">="		{	 lexicalScanner.getToken("MOREEQ", yytext);	}
"=="		{	 lexicalScanner.getToken("EQ", yytext);	}
"!="		{	 lexicalScanner.getToken("NONEQ", yytext);	}
"="			{	 lexicalScanner.getToken("=", yytext);	}
"&&"		{	 lexicalScanner.getToken("AND", yytext);	}
"||"		{	 lexicalScanner.getToken("OR", yytext);	}
"+"			{	 lexicalScanner.getToken("+", yytext);	}
"-"			{	 lexicalScanner.getToken("-", yytext);	}
"*"			{	 lexicalScanner.getToken("*", yytext);	}
"/"			{	 lexicalScanner.getToken("/", yytext);	}
";"			{	 lexicalScanner.getToken(";", yytext);	}
","			{	 lexicalScanner.getToken(",", yytext);	}
"!"			{	 lexicalScanner.getToken("!", yytext);	}
"."			{	 lexicalScanner.getToken(".", yytext);	}
{BOOL}		{	 lexicalScanner.getValue("BOOL", yytext); }
{INT}		{	 lexicalScanner.getValue("INT", yytext); }
{ID}		{	 lexicalScanner.getValue("ID", yytext); }
{SPACE}		{	 lexicalScanner.getSpace(yytext); }
{LINE_SEPARATOR} {	lexicalScanner.getNewLine(yytext);	}
.			{	 lexicalScanner.getErrorToken(yytext); }

%%
int YYtext();