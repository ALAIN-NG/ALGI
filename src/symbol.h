// Table des symboles pour ALGI

#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== TYPES DE SYMBOLES =====
typedef enum {
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_CLASS,
    SYM_PARAMETER,
    SYM_FIELD,
    SYM_METHOD
} SymbolKind;

// ===== STRUCTURE D'UN SYMBOLE =====
typedef struct Symbol {
    char* name;
    SymbolKind kind;
    DataType data_type;
    int scope_level;
    int is_mutable;
    struct Symbol* next;
} Symbol;

// ===== STRUCTURE DE LA TABLE DES SYMBOLES =====
typedef struct SymbolTable {
    Symbol** symbols;
    int count;
    int capacity;
    int scope_level;
    struct SymbolTable* parent;
} SymbolTable;

// ===== FONCTIONS DE CRÉATION =====
SymbolTable* create_symbol_table();
SymbolTable* create_child_scope(SymbolTable* parent);
void destroy_symbol_table(SymbolTable* table);

// ===== FONCTIONS DE GESTION =====
Symbol* add_symbol(SymbolTable* table, const char* name, SymbolKind kind, DataType type);
Symbol* find_symbol(SymbolTable* table, const char* name);
Symbol* find_symbol_in_scope(SymbolTable* table, const char* name);
Symbol* get_symbol(SymbolTable* table, const char* name);

// ===== FONCTIONS D'UTILITÉ =====
void enter_scope(SymbolTable** table);
void exit_scope(SymbolTable** table);
int get_current_scope_level(SymbolTable* table);

// ===== FONCTIONS D'AFFICHAGE =====
void print_symbol_table(SymbolTable* table);
void print_symbol(Symbol* symbol);

#endif // SYMBOL_H