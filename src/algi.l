/* Analyseur lexical pour le langage ALGI */
/* Base sur le cahier des charges et le fichier mot.txt */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algi.tab.h"
#include "ast.h"

/* Variables globales */
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

/* Fonction pour rapporter les erreurs */
void yyerror(const char* s);

/* Compteur de lignes */
int current_line = 1;
int current_column = 1;

%}

%option noyywrap
%option yylineno
%option nodefault

/* Definitions des expressions regulières */
INTEGER     [0-9]+
FLOAT       [0-9]+\.[0-9]+
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
STRING      \"([^\"\\]|\\.)*\"
COMMENT_LINE  \/\/[^\n]*
COMMENT_BLOCK  \/\*([^*]|\*+[^*/])*\*+\/
WHITESPACE  [ \t\r\n]+

%x STRING_STATE

%%



"ke"            { return KEYWORD_IF; }
"kommenu"       { return KEYWORD_ELSE; }
"w"             { return KEYWORD_WHILE; }
"pou"           { return KEYWORD_FOR; }
"mi"            { return KEYWORD_END; }

"he"            { return KEYWORD_LET; }
"fonksion"      { return KEYWORD_FUNCTION; }
"kwouo"         { return KEYWORD_CLASS; }
"feh"           { return KEYWORD_NEW; }

"tem"           { return KEYWORD_RETURN; }
"peu"           { return KEYWORD_BREAK; }
"tela"          { return KEYWORD_TRY; }
"tatem"         { return KEYWORD_CATCH; }

"tehap"         { return KEYWORD_PRINT; }
"keksie"        { return KEYWORD_INPUT; }
"sanwe"         { return KEYWORD_EVAL; }
"waan"          { return KEYWORD_SUM; }
"tie"           { return KEYWORD_SUBTRACT; }
"denden"        { return KEYWORD_EQUALS; }
"nwagniehwe"    { return KEYWORD_SORT; }
"ge"            { return KEYWORD_TYPEOF; }
"ganfa"         { return KEYWORD_OPEN; }
"stegnie"       { return KEYWORD_EXIT; }

"nene"          { return BOOLEAN_TRUE; }
"pok"           { return BOOLEAN_FALSE; }
"n"             { return NULL_VALUE; }

"e"             { return LOGICAL_AND; } 
"ou"            { return LOGICAL_OR; }
"poke"          { return LOGICAL_NOT; }

"est"           { return OPERATOR_IS; }
"dans"          { return OPERATOR_IN; }
"comme"         { return OPERATOR_AS; }

"bi"            { return KEYWORD_LIST; }

"saa"           { return TYPE_INT; } 
"nka'tche"      { return TYPE_FLOAT; }
"reyel"         { return TYPE_FLOAT; }
"djanghom"      { return TYPE_STRING; }


"=="            { return OP_EQUAL; }
"!="            { return OP_NOT_EQUAL; }
"<="            { return OP_LE; }
">="            { return OP_GE; }
"<"             { return '<'; }
">"             { return '>'; }
"="             { return '='; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"^"             { return '^'; }


"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }
"["             { return '['; }
"]"             { return ']'; }
";"             { return ';'; }
","             { return ','; }
"."             { return '.'; }
":"             { return ':'; }


{INTEGER}       { 
                    yylval.number = atof(yytext); 
                    return NUMBER; 
                }
{FLOAT}         { 
                    yylval.number = atof(yytext); 
                    return NUMBER; 
                }


{STRING}        { 
                    char* str = strdup(yytext + 1);
                    str[strlen(str) - 1] = '\0';
                    yylval.string = str;
                    return STRING_LITERAL; 
                }


{IDENTIFIER}    { 
                    yylval.identifier = strdup(yytext); 
                    return IDENTIFIER; 
                }


{COMMENT_LINE}  { /* Ignorer les commentaires de ligne */ }
{COMMENT_BLOCK} { /* Ignorer les commentaires de bloc */ }


{WHITESPACE}    { /* Ignorer les espaces */ }


.               { 
                    char msg[256];
                    sprintf(msg, "Caractère inattendu '%s' (code: %d)", yytext, yytext[0]);
                    yyerror(msg);
                }

%%

/*  FONCTION POUR RAPPORTER LES ERREURS  */
void yyerror(const char* s) {
    fprintf(stderr, "❌ Erreur lexicale à la ligne %d: %s\n", yylineno, s);
}


