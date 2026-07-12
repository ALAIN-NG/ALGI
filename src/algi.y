// Analyseur syntaxique pour le langage ALGI
// Basé sur la grammaire EBNF du cahier des charges §4

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol.h"
#include "codegen.h"
#include "codegen_asm.h"

// Déclarations externes
extern int yylex();
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
void yyerror(const char* s);

// Variables globales
ASTNode* program_root = NULL;
SymbolTable* global_symbols = NULL;
int has_error = 0;

// Fonctions de gestion des erreurs
void report_error(const char* msg) {
    fprintf(stderr, "❌ Erreur à la ligne %d: %s\n", yylineno, msg);
    has_error = 1;
}

%}

// ===== DÉFINITION DES TOKENS =====
// Mots-clés de structure
%token KEYWORD_IF KEYWORD_ELSE KEYWORD_WHILE KEYWORD_FOR KEYWORD_END

// Mots-clés de déclaration
%token KEYWORD_LET KEYWORD_FUNCTION KEYWORD_CLASS KEYWORD_NEW

// Mots-clés de contrôle
%token KEYWORD_RETURN KEYWORD_BREAK KEYWORD_TRY KEYWORD_CATCH

// Fonctions natives
%token KEYWORD_PRINT KEYWORD_INPUT KEYWORD_EVAL
%token KEYWORD_SUM KEYWORD_SUBTRACT KEYWORD_EQUALS
%token KEYWORD_SORT KEYWORD_TYPEOF KEYWORD_OPEN KEYWORD_EXIT

// Opérateurs logiques
%token LOGICAL_AND LOGICAL_OR LOGICAL_NOT

// Opérateurs spéciaux
%token OPERATOR_IS OPERATOR_IN OPERATOR_AS

// Opérateurs de comparaison
%token OP_EQUAL OP_NOT_EQUAL OP_LE OP_GE

// Types
%token TYPE_INT TYPE_FLOAT TYPE_STRING

// Littéraux
%token BOOLEAN_TRUE BOOLEAN_FALSE NULL_VALUE
%token KEYWORD_LIST

// Valeurs
%token NUMBER STRING_LITERAL IDENTIFIER

// ===== TYPES DES NON-TERMINAUX =====
%code requires {
    #include "ast.h"
}

%union {
    double number;
    char* string;
    char* identifier;
    ASTNode* node;
    ASTNode** node_list;
    int int_value;
}


%type <node> program instruction declaration assignment if_statement
%type <node> while_loop for_loop function_def return_statement
%type <node> break_statement print_statement input_statement
%type <node> expression logical_or logical_and equality comparison
%type <node> addition multiplication unary postfix primary
%type <node> block
%type <node> try_catch class_def new_instance list_literal


%type <node_list> argument_list
%type <node_list> expression_list
%type <node_list> parameter_list
%type <node_list> instructions

%type <int_value> type_annotation
%type <identifier> IDENTIFIER
%type <string> STRING_LITERAL
%type <number> NUMBER

// ===== PRÉCÉDENCE DES OPÉRATEURS =====
%right '='

%left LOGICAL_OR
%left LOGICAL_AND

%left OP_EQUAL OP_NOT_EQUAL

%left '<' '>' OP_LE OP_GE OPERATOR_IS OPERATOR_IN

%left '+' '-'

%left '*' '/' '%'

%right LOGICAL_NOT
%right UMINUS
// ===== GRAMMAIRE =====
%start program

%%

program
    : instructions
    {
        global_symbols = create_symbol_table();

        $$ = create_program();

        for (int i = 0; $1 && $1[i]; i++)
        {
            if ($1[i] != NULL)
                add_instruction($$, $1[i]);
        }

        program_root = $$;
    }
;
instructions
    : instruction
    {
        $$ = malloc(2 * sizeof(ASTNode*));

        if ($1)
        {
            $$[0] = $1;
            $$[1] = NULL;
        }
        else
        {
            $$[0] = NULL;
        }
    }
    | instructions instruction
    {
        if ($2 == NULL)
        {
            $$ = $1;
        }
        else
        {
            int count = 0;

            while ($1 && $1[count])
                count++;

            $$ = realloc($1, (count + 2) * sizeof(ASTNode*));

            $$[count] = $2;
            $$[count + 1] = NULL;
        }
}
    ;

instruction
    : declaration
    | assignment
    | if_statement
    | while_loop
    | for_loop
    | function_def
    | class_def
    | return_statement
    | break_statement
    | print_statement
    | input_statement
    | try_catch
    | expression ';' { $$ = $1; }
    | ';' { $$ = NULL; }
    ;

// ===== DÉCLARATIONS =====
declaration
    : KEYWORD_LET IDENTIFIER '=' expression {
        $$ = create_declaration($2, $4);
        // Ajouter à la table des symboles
        if (global_symbols) {
            add_symbol(global_symbols, $2, SYM_VARIABLE, AST_TYPE_UNKNOWN);
        }
    }
    | KEYWORD_LET IDENTIFIER ':' type_annotation '=' expression {
        $$ = create_declaration($2, $6);

        if (global_symbols) {
            add_symbol(global_symbols, $2, SYM_VARIABLE, $4);
        }
    }
    ;

type_annotation
    : TYPE_INT      { $$ = AST_TYPE_INT; }
    | TYPE_FLOAT    { $$ = AST_TYPE_FLOAT; }
    | TYPE_STRING   { $$ = AST_TYPE_STRING; }
    | KEYWORD_LIST  { $$ = AST_TYPE_LIST; }
    | IDENTIFIER    { $$ = AST_TYPE_UNKNOWN; }
;

// ===== ASSIGNATIONS =====
assignment
    : IDENTIFIER '=' expression
    {
        $$ = create_assignment($1,$3);

        if(global_symbols && !find_symbol(global_symbols,$1))
        {
            char msg[256];
            sprintf(msg,"Variable '%s' non déclarée",$1);
            report_error(msg);
        }
    }

    | postfix '=' expression
    {
        // BUG CORRIGÉ: l'ancien code faisait create_assignment(NULL,$3)
        // (qui stocke déjà $3 dans node->left) PUIS écrasait node->left
        // avec $1 (la cible), perdant totalement l'expression $3 à
        // assigner. On stocke maintenant la cible postfixe dans
        // node->right (libre pour ce nœud) afin de conserver la RHS
        // dans node->left.
        $$ = create_assignment(NULL, $3);
        $$->right = $1;
    }
;

// ===== STRUCTURES DE CONTRÔLE =====
if_statement
    : KEYWORD_IF '(' expression ')' block KEYWORD_END {
        $$ = create_if($3, $5, NULL);
    }
    | KEYWORD_IF '(' expression ')' block KEYWORD_ELSE block KEYWORD_END {
        $$ = create_if($3, $5, $7);
    }
    ;

while_loop
    : KEYWORD_WHILE '(' expression ')' block KEYWORD_END {
        $$ = create_while($3, $5);
    }
    ;

for_loop
    : KEYWORD_FOR '(' IDENTIFIER OPERATOR_IN expression ')' block KEYWORD_END {
        $$ = create_for($3, $5, $7);
    }
    ;

// ===== FONCTIONS =====
function_def
    : KEYWORD_FUNCTION IDENTIFIER '(' parameter_list ')' block KEYWORD_END {
        // BUG CORRIGÉ: l'ancien code appelait create_function($2, NULL, 0, $6),
        // ignorant purement et simplement la liste de paramètres $4 !
        // Résultat: aucune fonction ALGI définie avec des paramètres ne
        // pouvait fonctionner correctement (les paramètres étaient
        // silencieusement perdus). On convertit ici $4 (tableau de nœuds
        // NODE_IDENTIFIER, terminé par NULL) en tableau de noms (char**).
        int param_count = 0;
        while ($4 && $4[param_count]) param_count++;

        char** param_names = NULL;
        if (param_count > 0) {
            param_names = (char**)malloc(param_count * sizeof(char*));
            for (int i = 0; i < param_count; i++) {
                param_names[i] = strdup($4[i]->data.identifier_name);
                free_ast($4[i]);
            }
        }
        if ($4) free($4);

        $$ = create_function($2, param_names, param_count, $6);
        if (global_symbols) {
            add_symbol(global_symbols, $2, SYM_FUNCTION, AST_TYPE_UNKNOWN);
        }
    }
    ;

parameter_list
    : /* vide */ {
        $$ = NULL;
    }
    | IDENTIFIER {
        $$ = malloc(2 * sizeof(ASTNode*));

        $$[0] = create_identifier($1);
        $$[1] = NULL;
    }
    | parameter_list ',' IDENTIFIER {
        int count = 0;

        while ($1 && $1[count])
            count++;

        $$ = realloc($1, (count + 2) * sizeof(ASTNode*));

        $$[count] = create_identifier($3);
        $$[count + 1] = NULL;
    }
    ;

// ===== CLASSES =====
class_def
    : KEYWORD_CLASS IDENTIFIER block KEYWORD_END {
        $$ = create_class($2, $3);
        if (global_symbols) {
            add_symbol(global_symbols, $2, SYM_CLASS, AST_TYPE_UNKNOWN);
        }
    }
    ;

new_instance
    : KEYWORD_NEW IDENTIFIER '(' argument_list ')' {
        $$ = create_new_instance($2, $4);
    }
    ;

// ===== INSTRUCTIONS DE CONTRÔLE =====
return_statement
    : KEYWORD_RETURN ';'
    {
        $$ = create_return(NULL);
    }

    | KEYWORD_RETURN expression ';'
    {
        $$ = create_return($2);
    }

    | KEYWORD_RETURN expression
    {
        // Forme sans point-virgule, utilisée dans tout le reste du
        // langage (cf. déclarations, affectations...).
        $$ = create_return($2);
    }

    | KEYWORD_RETURN
    {
        $$ = create_return(NULL);
    }
;

break_statement
    : KEYWORD_BREAK {
        $$ = create_break();
    }
    ;

// ===== FONCTIONS NATIVES =====
print_statement
    : KEYWORD_PRINT '(' expression ')' {
        $$ = create_print($3);
    }
    ;

input_statement
    : KEYWORD_INPUT '(' expression ')' {
        $$ = create_input($3);
    }
    ;

try_catch
    : KEYWORD_TRY block KEYWORD_CATCH '(' IDENTIFIER ')' block KEYWORD_END {
        $$ = create_try_catch($2, $5, $7);
    }
    ;

// ===== BLOCS =====
block
    : '{' instructions '}' {
        $$ = create_block($2);
    }
    | instructions {
        // BUG CORRIGÉ: l'ancienne règle n'acceptait qu'une seule
        // instruction (via 'instruction' au singulier) pour un bloc sans
        // accolades, alors que tout le langage ALGI est écrit sans
        // accolades avec PLUSIEURS instructions entre le mot-clé de
        // structure (w, ke, pou...) et 'mi'. On utilise donc 'instructions'
        // (pluriel, une ou plusieurs instructions).
        $$ = create_block(NULL);
        for (int i = 0; $1 && $1[i]; i++) {
            if ($1[i] != NULL)
                add_instruction($$, $1[i]);
        }
    }
    ;

// ===== EXPRESSIONS =====
expression
    : assignment
    | logical_or
    ;

logical_or
    : logical_and
    | logical_or LOGICAL_OR logical_and {
        $$ = create_binary_op(OP_OR, $1, $3);
    }
    ;

logical_and
    : equality
    | logical_and LOGICAL_AND equality {
        $$ = create_binary_op(OP_AND, $1, $3);
    }
    ;

equality
    : comparison
    | equality OP_EQUAL comparison {
        $$ = create_binary_op(OP_EQ, $1, $3);
    }
    | equality OP_NOT_EQUAL comparison {
        $$ = create_binary_op(OP_NE, $1, $3);
    }
    ;

comparison
    : addition
    | comparison '<' addition {
        $$ = create_binary_op(OP_LT, $1, $3);
    }
    | comparison '>' addition {
        $$ = create_binary_op(OP_GT, $1, $3);
    }
    | comparison OP_LE addition {
        $$ = create_binary_op(AST_OP_LE, $1, $3);
    }
    | comparison OP_GE addition {
        $$ = create_binary_op(AST_OP_GE, $1, $3);
    }
    | comparison OPERATOR_IS addition {
        $$ = create_binary_op(OP_IS, $1, $3);
    }
    | comparison OPERATOR_IN addition {
        $$ = create_binary_op(OP_IN, $1, $3);
    }
    ;

addition
    : multiplication
    | addition '+' multiplication {
        $$ = create_binary_op(OP_ADD, $1, $3);
    }
    | addition '-' multiplication {
        $$ = create_binary_op(OP_SUB, $1, $3);
    }
    ;

multiplication
    : unary
    | multiplication '*' unary {
        $$ = create_binary_op(OP_MUL, $1, $3);
    }
    | multiplication '/' unary {
        $$ = create_binary_op(OP_DIV, $1, $3);
    }
    | multiplication '%' unary {
        $$ = create_binary_op(OP_MOD, $1, $3);
    }
    ;

unary
    : postfix
    | '-' unary %prec UMINUS {
        $$ = create_unary_op(OP_NEG, $2);
    }
    | LOGICAL_NOT unary {
        $$ = create_unary_op(OP_NOT, $2);
    }
    ;

postfix
    : primary
    | postfix '(' argument_list ')' {
        $$ = create_call($1, $3, 0);
    }
    | postfix '[' expression ']' {
        $$ = create_index_access($1, $3);
    }
    | postfix '.' IDENTIFIER {
        $$ = create_field_access($1, $3);
    }
    ;

primary
    : NUMBER {
        $$ = create_number($1);
    }
    | STRING_LITERAL {
        $$ = create_string($1);
    }
    | BOOLEAN_TRUE {
        $$ = create_boolean(1);
    }
    | BOOLEAN_FALSE {
        $$ = create_boolean(0);
    }
    | NULL_VALUE {
        $$ = create_null();
    }
    | IDENTIFIER {
        $$ = create_identifier($1);
    }
    | '(' expression ')' {
        $$ = $2;
    }
    | list_literal
    | new_instance
    ;

// ===== LISTES =====
list_literal
    : KEYWORD_LIST '[' expression_list ']' {
        $$ = create_list($3, 0);
    }
    | KEYWORD_LIST '[' ']' {
        $$ = create_list(NULL, 0);
    }
    ;

// ===== LISTES D'ARGUMENTS =====
argument_list
    : /* vide */ { $$ = NULL; }
    | expression {
        $$ = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$[0] = $1;
        $$[1] = NULL;
    }
    | argument_list ',' expression {
        // Ajouter un argument
        int count = 0;
        while ($1 && $1[count]) count++;
        $$ = (ASTNode**)realloc($1, (count + 2) * sizeof(ASTNode*));
        $$[count] = $3;
        $$[count + 1] = NULL;
    }
    ;

expression_list
    : expression {
        $$ = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        $$[0] = $1;
        $$[1] = NULL;
    }
    | expression_list ',' expression {
        int count = 0;
        while ($1 && $1[count]) count++;
        $$ = (ASTNode**)realloc($1, (count + 2) * sizeof(ASTNode*));
        $$[count] = $3;
        $$[count + 1] = NULL;
    }
    ;

%%

// ===== FONCTION PRINCIPALE =====
int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier.algi>\n", argv[0]);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -c <fichier.algi>  Compiler vers C\n");
        fprintf(stderr, "  -a <fichier.algi>  Compiler vers assembleur\n");
        fprintf(stderr, "  -h                 Afficher l'aide\n");
        return 1;
    }
    
    const char* filename = argv[1];
    int compile_c = 1;
    int compile_asm = 0;
    
    // Analyse des options
    if (argc > 2) {
        if (strcmp(argv[1], "-c") == 0) {
            compile_c = 1;
            compile_asm = 0;
            filename = argv[2];
        } else if (strcmp(argv[1], "-a") == 0) {
            compile_c = 0;
            compile_asm = 1;
            filename = argv[2];
        }
    }
    
    FILE* input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "❌ Erreur: impossible d'ouvrir %s\n", filename);
        return 1;
    }
    
    yyin = input;
    
    printf("🔍 Compilation de %s...\n", filename);
    printf("📐 Analyse syntaxique...\n");
    
    // Analyser
    yyparse();
    
    if (has_error) {
        fprintf(stderr, "❌ Compilation échouée à cause d'erreurs de syntaxe\n");
        fclose(input);
        return 1;
    }
    
    fclose(input);
    
    if (program_root) {
        printf("✅ AST construit avec succès\n");
        
        // Afficher l'AST en mode debug
        // print_ast(program_root, 0);
        
        if (compile_c) {
            printf("⚙️  Génération du code C...\n");
            char output_c[256];
            sprintf(output_c, "%s.c", filename);
            // Enlever l'extension .algi
            char* dot = strrchr(output_c, '.');
            if (dot && strcmp(dot, ".algi.c") == 0) {
                // Déjà bon
            } else {
                char* ext = strstr(output_c, ".algi");
                if (ext) {
                    strcpy(ext, ".c");
                }
            }
            generate_code(program_root, output_c);
            printf("✅ Code C généré dans %s\n", output_c);
        }
        
        if (compile_asm) {
            printf("⚙️  Génération du code assembleur x86...\n");
            char output_asm[256];
            sprintf(output_asm, "%s.asm", filename);
            generate_asm(program_root, output_asm);
            printf("✅ Code assembleur généré dans %s\n", output_asm);
        }
        
        // Libérer l'AST
        free_ast(program_root);
    }
    
    return 0;
}