// Implémentation de la table des symboles pour ALGI

#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== FONCTIONS DE CRÉATION =====

SymbolTable* create_symbol_table() {
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!table) {
        fprintf(stderr, "❌ Erreur: allocation mémoire échouée\n");
        exit(1);
    }
    table->symbols = NULL;
    table->count = 0;
    table->capacity = 0;
    table->scope_level = 0;
    table->parent = NULL;
    return table;
}

SymbolTable* create_child_scope(SymbolTable* parent) {
    SymbolTable* table = create_symbol_table();
    table->parent = parent;
    table->scope_level = parent ? parent->scope_level + 1 : 1;
    return table;
}

void destroy_symbol_table(SymbolTable* table) {
    if (!table) return;
    
    // Libérer tous les symboles
    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i]) {
            if (table->symbols[i]->name) {
                free(table->symbols[i]->name);
            }
            free(table->symbols[i]);
        }
    }
    if (table->symbols) {
        free(table->symbols);
    }
    free(table);
}

// ===== FONCTIONS DE GESTION =====

Symbol* add_symbol(SymbolTable* table, const char* name, SymbolKind kind, DataType type) {
    if (!table || !name) return NULL;
    
    // Vérifier si le symbole existe déjà dans la portée courante
    Symbol* existing = find_symbol_in_scope(table, name);
    if (existing) {
        fprintf(stderr, "⚠️ Erreur: symbole '%s' déjà déclaré dans cette portée\n", name);
        return NULL;
    }
    
    // Agrandir la table si nécessaire
    if (table->count >= table->capacity) {
        table->capacity = table->capacity ? table->capacity * 2 : 16;
        table->symbols = (Symbol**)realloc(table->symbols, table->capacity * sizeof(Symbol*));
        if (!table->symbols) {
            fprintf(stderr, "❌ Erreur: réallocation mémoire échouée\n");
            exit(1);
        }
    }
    
    // Créer le symbole
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    if (!symbol) {
        fprintf(stderr, "❌ Erreur: allocation mémoire échouée\n");
        exit(1);
    }
    symbol->name = strdup(name);
    symbol->kind = kind;
    symbol->data_type = type;
    symbol->scope_level = table->scope_level;
    symbol->is_mutable = (kind == SYM_VARIABLE || kind == SYM_PARAMETER);
    symbol->next = NULL;
    
    table->symbols[table->count++] = symbol;
    return symbol;
}

Symbol* find_symbol(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    // Chercher dans la portée courante
    Symbol* symbol = find_symbol_in_scope(table, name);
    if (symbol) return symbol;
    
    // Chercher dans les portées parentes
    if (table->parent) {
        return find_symbol(table->parent, name);
    }
    
    return NULL;
}

Symbol* find_symbol_in_scope(SymbolTable* table, const char* name) {
    if (!table || !name) return NULL;
    
    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i] && strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i];
        }
    }
    return NULL;
}

Symbol* get_symbol(SymbolTable* table, const char* name) {
    return find_symbol(table, name);
}

// ===== FONCTIONS D'UTILITÉ =====

void enter_scope(SymbolTable** table) {
    if (!table || !*table) return;
    SymbolTable* new_scope = create_child_scope(*table);
    *table = new_scope;
}

void exit_scope(SymbolTable** table) {
    if (!table || !*table) return;
    SymbolTable* parent = (*table)->parent;
    if (parent) {
        destroy_symbol_table(*table);
        *table = parent;
    } else {
        fprintf(stderr, "⚠️ Attention: tentative de sortir de la portée globale\n");
    }
}

int get_current_scope_level(SymbolTable* table) {
    return table ? table->scope_level : -1;
}

// ===== FONCTIONS D'AFFICHAGE =====

void print_symbol(Symbol* symbol) {
    if (!symbol) {
        printf("(null)\n");
        return;
    }
    const char* kind_str = "";
    switch (symbol->kind) {
        case SYM_VARIABLE: kind_str = "variable"; break;
        case SYM_FUNCTION: kind_str = "function"; break;
        case SYM_CLASS: kind_str = "class"; break;
        case SYM_PARAMETER: kind_str = "parameter"; break;
        case SYM_FIELD: kind_str = "field"; break;
        case SYM_METHOD: kind_str = "method"; break;
    }
    printf("  %s: %s (type: %s, scope: %d, mutable: %s)\n",
           symbol->name,
           kind_str,
           data_AST_type_to_string(symbol->data_type),
           symbol->scope_level,
           symbol->is_mutable ? "oui" : "non");
}

void print_symbol_table(SymbolTable* table) {
    if (!table) {
        printf("Table des symboles vide\n");
        return;
    }
    
    printf("Table des symboles (scope %d):\n", table->scope_level);
    for (int i = 0; i < table->count; i++) {
        print_symbol(table->symbols[i]);
    }
    
    if (table->parent) {
        printf("Parent (scope %d):\n", table->parent->scope_level);
        print_symbol_table(table->parent);
    }
}