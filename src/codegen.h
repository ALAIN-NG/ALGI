// Générateur de code C pour ALGI
//
// Le code C généré s'appuie entièrement sur le système de valeurs
// dynamiques AlgiValue* défini dans runtime.h : chaque expression ALGI
// est traduite en une expression C de type AlgiValue*.

#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symbol.h"

// ===== STRUCTURE DU GÉNÉRATEUR =====
typedef struct CodeGen {
    FILE* output;
    int indent_level;
    int temp_counter;
    SymbolTable* symbols;
    const char* runtime_path;
} CodeGen;

// ===== FONCTIONS PRINCIPALES =====
void generate_code(ASTNode* node, const char* filename);
void generate_code_with_runtime(ASTNode* node, const char* filename, const char* runtime_path);

// ===== FONCTIONS DE GÉNÉRATION =====
void codegen_init(CodeGen* gen, FILE* output, SymbolTable* symbols);
void codegen_emit(CodeGen* gen, const char* format, ...);
void codegen_emit_indent(CodeGen* gen);
void codegen_generate(CodeGen* gen, ASTNode* node);

// ===== FONCTIONS SPÉCIFIQUES (instructions) =====
void codegen_generate_program(CodeGen* gen, ASTNode* node);
void codegen_generate_declaration(CodeGen* gen, ASTNode* node);
void codegen_generate_assignment(CodeGen* gen, ASTNode* node);
void codegen_generate_if(CodeGen* gen, ASTNode* node);
void codegen_generate_while(CodeGen* gen, ASTNode* node);
void codegen_generate_for(CodeGen* gen, ASTNode* node);
void codegen_generate_function(CodeGen* gen, ASTNode* node);
void codegen_generate_class(CodeGen* gen, ASTNode* node);
void codegen_generate_return(CodeGen* gen, ASTNode* node);
void codegen_generate_break(CodeGen* gen, ASTNode* node);
void codegen_generate_print(CodeGen* gen, ASTNode* node);
void codegen_generate_input(CodeGen* gen, ASTNode* node);
void codegen_generate_try_catch(CodeGen* gen, ASTNode* node);

// ===== GÉNÉRATION D'EXPRESSIONS (retournent une chaîne allouée à libérer) =====
// Toute expression ALGI est traduite en une expression C de type AlgiValue*.
// Certaines expressions (ex: littéraux de liste) ont besoin d'émettre des
// instructions préparatoires AVANT l'expression englobante : ces fonctions
// écrivent ces instructions directement dans gen->output (dans le bon
// ordre, avant de renvoyer le texte de l'expression finale), ce qui évite
// l'ancien bug où du code "instruction" se retrouvait imbriqué au milieu
// d'une expression déjà partiellement émise.
char* codegen_gen_expr(CodeGen* gen, ASTNode* node);
char* codegen_gen_binary_op(CodeGen* gen, ASTNode* node);
char* codegen_gen_unary_op(CodeGen* gen, ASTNode* node);
char* codegen_gen_call(CodeGen* gen, ASTNode* node);
char* codegen_gen_list(CodeGen* gen, ASTNode* node);
char* codegen_gen_index_access(CodeGen* gen, ASTNode* node);
char* codegen_gen_field_access(CodeGen* gen, ASTNode* node);
char* codegen_gen_new_instance(CodeGen* gen, ASTNode* node);

// ===== FONCTIONS D'UTILITÉ =====
char* codegen_escape_string(const char* input);
char* codegen_temp_name(CodeGen* gen);
char* codegen_label_name(CodeGen* gen, const char* prefix);

#endif // CODEGEN_H
