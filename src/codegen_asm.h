// Générateur de code assembleur x86 pour ALGI
// Inspiré du séminaire de traduction-compilation

#ifndef CODEGEN_ASM_H
#define CODEGEN_ASM_H

#include "ast.h"
#include "symbol.h"
#include <stdio.h>

// ===== STRUCTURE DU GÉNÉRATEUR =====
typedef struct AsmGen {
    FILE* output;
    int indent_level;
    int temp_counter;
    int label_counter;
    SymbolTable* symbols;
    int stack_size;
    int has_main;
    char* current_function;

    // Table locale nom-de-variable -> offset sur la pile (ebp-offset).
    // Nécessaire car chaque variable doit avoir son propre emplacement
    // mémoire (auparavant, toutes les variables lisaient/écrivaient
    // à [ebp-stack_size], donc partageaient le même emplacement).
    char** var_names;
    int* var_offsets;
    int var_count;
    int var_capacity;

    // Table de correspondance (nœud NODE_STRING) -> étiquette .data.
    // Nécessaire car la section .data est écrite AVANT la génération du
    // corps du programme (une seule passe sur le fichier de sortie) alors
    // que les littéraux de chaîne ne sont découverts qu'en générant le
    // corps : on les collecte donc d'abord par un parcours dédié de l'AST.
    struct ASTNode** str_nodes;
    char** str_labels;
    char** str_contents;
    int str_count;
    int str_capacity;
} AsmGen;

// ===== GESTION DES CHAÎNES LITTÉRALES (étiquettes .data) =====
void asmgen_collect_strings(AsmGen* gen, ASTNode* node);
const char* asmgen_find_string_label(AsmGen* gen, ASTNode* node);

// ===== GESTION DES VARIABLES (offsets sur la pile) =====
int asmgen_declare_var(AsmGen* gen, const char* name);
int asmgen_find_var_offset(AsmGen* gen, const char* name);

// ===== FONCTIONS PRINCIPALES =====
void generate_asm(ASTNode* node, const char* filename);
void generate_asm_with_symbols(ASTNode* node, const char* filename, SymbolTable* symbols);

// ===== FONCTIONS DE GÉNÉRATION =====
void asmgen_init(AsmGen* gen, FILE* output, SymbolTable* symbols);
void asmgen_emit(AsmGen* gen, const char* format, ...);
void asmgen_emit_indent(AsmGen* gen);
void asmgen_generate(AsmGen* gen, ASTNode* node);

// ===== FONCTIONS SPÉCIFIQUES =====
void asmgen_generate_program(AsmGen* gen, ASTNode* node);
void asmgen_generate_declaration(AsmGen* gen, ASTNode* node);
void asmgen_generate_assignment(AsmGen* gen, ASTNode* node);
void asmgen_generate_if(AsmGen* gen, ASTNode* node);
void asmgen_generate_while(AsmGen* gen, ASTNode* node);
void asmgen_generate_for(AsmGen* gen, ASTNode* node);
void asmgen_generate_function(AsmGen* gen, ASTNode* node);
void asmgen_generate_class(AsmGen* gen, ASTNode* node);
void asmgen_generate_return(AsmGen* gen, ASTNode* node);
void asmgen_generate_print(AsmGen* gen, ASTNode* node);
void asmgen_generate_input(AsmGen* gen, ASTNode* node);
void asmgen_generate_expression(AsmGen* gen, ASTNode* node);

// ===== FONCTIONS D'UTILITÉ =====
char* asmgen_temp_name(AsmGen* gen);
char* asmgen_label_name(AsmGen* gen, const char* prefix);
void asmgen_push_value(AsmGen* gen, const char* value);
void asmgen_pop_value(AsmGen* gen, const char* dest);
void asmgen_emit_prologue(AsmGen* gen);
void asmgen_emit_epilogue(AsmGen* gen);

#endif // CODEGEN_ASM_H