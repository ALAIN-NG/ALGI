// Implémentation du générateur de code assembleur x86 pour ALGI
// Inspiré du séminaire de traduction-compilation

#include "codegen_asm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

static char* asmgen_escape_for_nasm(const char* input);

// ===== FONCTIONS PRINCIPALES =====

void generate_asm(ASTNode* node, const char* filename) {
    generate_asm_with_symbols(node, filename, NULL);
}

void generate_asm_with_symbols(ASTNode* node, const char* filename, SymbolTable* symbols) {
    if (!node) {
        fprintf(stderr, "❌ Erreur: AST NULL\n");
        return;
    }
    
    FILE* output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "❌ Erreur: impossible d'écrire %s\n", filename);
        return;
    }
    
    AsmGen gen;
    asmgen_init(&gen, output, symbols);

    // Recense tous les littéraux de chaîne AVANT d'écrire la section .data
    // (celle-ci doit précéder le corps du programme dans le fichier de
    // sortie, mais les chaînes ne sont découvertes qu'en parcourant l'AST).
    asmgen_collect_strings(&gen, node);

    // En-tête du fichier assembleur
    fprintf(output, "; Code généré par le compilateur ALGI\n");
    fprintf(output, "; Fichier: %s\n", filename);
    fprintf(output, "; Date: %s", ctime(&(time_t){time(NULL)}));
    fprintf(output, "\n");
    fprintf(output, "; NOTE: ce backend assembleur est un générateur pédagogique\n");
    fprintf(output, "; simplifié. Il gère les entiers (arithmétique, variables,\n");
    fprintf(output, "; boucles, conditions) et l'affichage de littéraux de chaîne,\n");
    fprintf(output, "; mais PAS la concaténation de chaînes ('+' entre chaînes),\n");
    fprintf(output, "; les flottants, ni les listes/objets. Pour un support complet\n");
    fprintf(output, "; de ces fonctionnalités, utiliser le mode -c (génération C).\n");
    fprintf(output, "\n");

    fprintf(output, "BITS 32\n");
    fprintf(output, "section .data\n");
    fprintf(output, "  newline db 10, 0\n");
    fprintf(output, "  format_int db \"%%d\", 10, 0\n");
    fprintf(output, "  format_str db \"%%s\", 10, 0\n");
    for (int i = 0; i < gen.str_count; i++) {
        char* escaped = asmgen_escape_for_nasm(gen.str_contents[i]);
        fprintf(output, "  %s db \"%s\", 0\n", gen.str_labels[i], escaped);
        free(escaped);
    }
    fprintf(output, "\n");
    fprintf(output, "section .bss\n");
    fprintf(output, "  input_buffer resb 1024\n");
    fprintf(output, "\n");
    fprintf(output, "section .text\n");
    fprintf(output, "  global _start\n");
    fprintf(output, "\n");
    
    // Externes
    fprintf(output, "  extern printf\n");
    fprintf(output, "  extern scanf\n");
    fprintf(output, "  extern exit\n");
    fprintf(output, "\n");
    
    // Prologue
    asmgen_emit_prologue(&gen);
    
    // Générer le code
    asmgen_generate(&gen, node);
    
    // Épilogue
    asmgen_emit_epilogue(&gen);
    
    fclose(output);
}

// ===== FONCTIONS DE GÉNÉRATION =====

void asmgen_init(AsmGen* gen, FILE* output, SymbolTable* symbols) {
    gen->output = output;
    gen->indent_level = 0;
    gen->temp_counter = 0;
    gen->label_counter = 0;
    gen->symbols = symbols;
    gen->stack_size = 0;
    gen->has_main = 0;
    gen->current_function = NULL;
    gen->var_names = NULL;
    gen->var_offsets = NULL;
    gen->var_count = 0;
    gen->var_capacity = 0;
    gen->str_nodes = NULL;
    gen->str_labels = NULL;
    gen->str_contents = NULL;
    gen->str_count = 0;
    gen->str_capacity = 0;
}

// Échappe une chaîne pour un littéral NASM 'db "...", 0' (guillemets doubles
// uniquement, NASM ne supporte pas les séquences d'échappement à l'intérieur
// d'une chaîne entre guillemets de la même façon que le C : on découpe donc
// tout guillemet interne en le sortant de la chaîne littérale).
static char* asmgen_escape_for_nasm(const char* input) {
    if (!input) return strdup("");
    size_t len = strlen(input);
    char* out = (char*)malloc(len * 6 + 1); // marge large (échappements)
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c == '"') {
            // Ferme la chaîne, insère un octet 34 (") littéral, rouvre
            const char* seq = "\", 34, \"";
            memcpy(out + j, seq, strlen(seq));
            j += strlen(seq);
        } else {
            out[j++] = (char)c;
        }
    }
    out[j] = '\0';
    return out;
}

// Parcourt récursivement l'AST pour recenser tous les littéraux de chaîne
// et leur attribuer une étiquette .data unique, AVANT que le corps du
// programme ne soit généré (nécessaire car la section .data est déjà
// écrite dans le fichier de sortie à ce moment-là).
void asmgen_collect_strings(AsmGen* gen, ASTNode* node) {
    if (!node) return;

    if (node->type == NODE_STRING) {
        if (gen->str_count >= gen->str_capacity) {
            gen->str_capacity = gen->str_capacity ? gen->str_capacity * 2 : 8;
            gen->str_nodes = (ASTNode**)realloc(gen->str_nodes, gen->str_capacity * sizeof(ASTNode*));
            gen->str_labels = (char**)realloc(gen->str_labels, gen->str_capacity * sizeof(char*));
            gen->str_contents = (char**)realloc(gen->str_contents, gen->str_capacity * sizeof(char*));
        }
        char label[32];
        snprintf(label, sizeof(label), "_str_lit_%d", gen->str_count);
        gen->str_nodes[gen->str_count] = node;
        gen->str_labels[gen->str_count] = strdup(label);
        gen->str_contents[gen->str_count] = strdup(node->data.string_value ? node->data.string_value : "");
        gen->str_count++;
    }

    asmgen_collect_strings(gen, node->left);
    asmgen_collect_strings(gen, node->right);
    asmgen_collect_strings(gen, node->extra);

    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = 0; i < node->data.block.count; i++) {
                asmgen_collect_strings(gen, node->data.block.instructions[i]);
            }
            break;
        case NODE_LIST:
            for (int i = 0; i < node->data.list.count; i++) {
                asmgen_collect_strings(gen, node->data.list.elements[i]);
            }
            break;
        case NODE_CALL:
        case NODE_NEW_INSTANCE:
            for (int i = 0; i < node->data.call.count; i++) {
                asmgen_collect_strings(gen, node->data.call.args[i]);
            }
            break;
        default:
            break;
    }
}

// Recherche l'étiquette .data associée à un nœud NODE_STRING donné
// (recherche par pointeur, chaque littéral de chaîne étant un nœud unique).
const char* asmgen_find_string_label(AsmGen* gen, ASTNode* node) {
    for (int i = 0; i < gen->str_count; i++) {
        if (gen->str_nodes[i] == node) {
            return gen->str_labels[i];
        }
    }
    return NULL;
}

// Déclare une nouvelle variable et lui attribue un offset unique sur la pile.
// Retourne l'offset (ebp-offset) attribué.
int asmgen_declare_var(AsmGen* gen, const char* name) {
    if (gen->var_count >= gen->var_capacity) {
        gen->var_capacity = gen->var_capacity ? gen->var_capacity * 2 : 8;
        gen->var_names = (char**)realloc(gen->var_names, gen->var_capacity * sizeof(char*));
        gen->var_offsets = (int*)realloc(gen->var_offsets, gen->var_capacity * sizeof(int));
    }
    gen->stack_size += 4;
    gen->var_names[gen->var_count] = strdup(name);
    gen->var_offsets[gen->var_count] = gen->stack_size;
    gen->var_count++;
    return gen->stack_size;
}

// Recherche l'offset d'une variable déjà déclarée. Retourne -1 si absente.
int asmgen_find_var_offset(AsmGen* gen, const char* name) {
    if (!name) return -1;
    for (int i = gen->var_count - 1; i >= 0; i--) {
        if (strcmp(gen->var_names[i], name) == 0) {
            return gen->var_offsets[i];
        }
    }
    return -1;
}

void asmgen_emit(AsmGen* gen, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(gen->output, format, args);
    va_end(args);
}

void asmgen_emit_indent(AsmGen* gen) {
    for (int i = 0; i < gen->indent_level; i++) {
        fprintf(gen->output, "  ");
    }
}

void asmgen_emit_prologue(AsmGen* gen) {
    fprintf(gen->output, "_start:\n");
    gen->indent_level++;
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov ebp, esp\n");
    // NOTE: le nombre de variables locales n'est connu qu'après génération
    // du corps du programme (approche un seul passage). On réserve donc un
    // espace de pile généreux et fixe pour éviter tout débordement, plutôt
    // que de recalculer précisément la taille nécessaire (ce qui exigerait
    // une double passe sur l'AST).
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "sub esp, 4096\n");  // Espace pour les variables locales
    gen->stack_size = 0;
}

void asmgen_emit_epilogue(AsmGen* gen) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov esp, ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "pop ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov eax, 0\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "ret\n");
}

void asmgen_generate(AsmGen* gen, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            asmgen_generate_program(gen, node);
            break;
        case NODE_DECLARATION:
            asmgen_generate_declaration(gen, node);
            break;
        case NODE_ASSIGNMENT:
            asmgen_generate_assignment(gen, node);
            break;
        case NODE_IF:
            asmgen_generate_if(gen, node);
            break;
        case NODE_WHILE:
            asmgen_generate_while(gen, node);
            break;
        case NODE_FOR:
            asmgen_generate_for(gen, node);
            break;
        case NODE_FUNCTION:
            asmgen_generate_function(gen, node);
            break;
        case NODE_CLASS:
            asmgen_generate_class(gen, node);
            break;
        case NODE_RETURN:
            asmgen_generate_return(gen, node);
            break;
        case NODE_PRINT:
            asmgen_generate_print(gen, node);
            break;
        case NODE_INPUT:
            asmgen_generate_input(gen, node);
            break;
        case NODE_NUMBER:
        case NODE_STRING:
        case NODE_BOOLEAN:
        case NODE_NULL:
        case NODE_IDENTIFIER:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            asmgen_generate_expression(gen, node);
            break;
        case NODE_CALL:
            // Appel de fonction
            asmgen_generate_expression(gen, node);
            break;
        default:
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "; Nœud non supporté: %s\n", node_AST_type_to_string(node->type));
            break;
    }
}

// ===== FONCTIONS SPÉCIFIQUES =====

void asmgen_generate_program(AsmGen* gen, ASTNode* node) {
    for (int i = 0; i < node->data.block.count; i++) {
        asmgen_generate(gen, node->data.block.instructions[i]);
    }
}

void asmgen_generate_declaration(AsmGen* gen, ASTNode* node) {
    // Déclaration de variable: chaque variable reçoit son propre offset
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; Declaration: %s\n", node->data.identifier_name);

    // Évaluer l'expression d'initialisation (résultat dans eax)
    asmgen_generate_expression(gen, node->left);

    int offset = asmgen_declare_var(gen, node->data.identifier_name);

    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov [ebp-%d], eax\n", offset);
}

void asmgen_generate_assignment(AsmGen* gen, ASTNode* node) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; Assignment: %s\n",
        node->data.identifier_name ? node->data.identifier_name : "(postfix)");

    // Évaluer l'expression de droite
    asmgen_generate_expression(gen, node->left);

    if (!node->data.identifier_name) {
        // Assignation sur un accès postfixe (ex: liste[i] = ...), non géré
        // par ce backend assembleur simplifié.
        asmgen_emit_indent(gen);
        asmgen_emit(gen, "; Assignation postfixe non supportée en assembleur\n");
        return;
    }

    int offset = asmgen_find_var_offset(gen, node->data.identifier_name);
    if (offset == -1) {
        // Variable jamais déclarée explicitement (ex: assignée sans 'he')
        // -> on lui attribue un nouvel emplacement à la volée.
        offset = asmgen_declare_var(gen, node->data.identifier_name);
    }

    // Stocker le résultat dans la variable
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov [ebp-%d], eax\n", offset);
}

void asmgen_generate_if(AsmGen* gen, ASTNode* node) {
    char* else_label = asmgen_label_name(gen, "else");
    char* end_label = asmgen_label_name(gen, "endif");
    
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; If statement\n");
    
    // Évaluer la condition
    asmgen_generate_expression(gen, node->left);
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "cmp eax, 0\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "je %s\n", else_label);
    
    // Then block
    if (node->right) {
        asmgen_generate(gen, node->right);
    }
    
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "jmp %s\n", end_label);
    
    // Else block (stocké dans node->extra, cf. 'kommenu')
    asmgen_emit(gen, "%s:\n", else_label);
    if (node->extra) {
        asmgen_generate(gen, node->extra);
    }
    
    asmgen_emit(gen, "%s:\n", end_label);
    free(else_label);
    free(end_label);
}

void asmgen_generate_while(AsmGen* gen, ASTNode* node) {
    char* loop_label = asmgen_label_name(gen, "while");
    char* end_label = asmgen_label_name(gen, "wend");
    
    asmgen_emit(gen, "%s:\n", loop_label);
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; While loop\n");
    
    // Évaluer la condition
    asmgen_generate_expression(gen, node->left);
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "cmp eax, 0\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "je %s\n", end_label);
    
    // Corps de la boucle
    if (node->right) {
        asmgen_generate(gen, node->right);
    }
    
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "jmp %s\n", loop_label);
    asmgen_emit(gen, "%s:\n", end_label);
    free(loop_label);
    free(end_label);
}

void asmgen_generate_for(AsmGen* gen, ASTNode* node) {
    char* loop_label = asmgen_label_name(gen, "for");
    char* end_label = asmgen_label_name(gen, "endfor");

    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; For loop: %s\n", node->data.identifier_name);

    // La variable de boucle reçoit son propre emplacement sur la pile
    int offset = asmgen_find_var_offset(gen, node->data.identifier_name);
    if (offset == -1) {
        offset = asmgen_declare_var(gen, node->data.identifier_name);
    }

    // Initialisation
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov dword [ebp-%d], 0\n", offset);

    asmgen_emit(gen, "%s:\n", loop_label);

    // Condition
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov eax, [ebp-%d]\n", offset);
    asmgen_emit_indent(gen);
    // Ici on devrait comparer avec la taille de l'itérable
    asmgen_emit(gen, "cmp eax, _size\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "jge %s\n", end_label);

    // Corps
    if (node->right) {
        asmgen_generate(gen, node->right);
    }

    // Incrémentation
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov eax, [ebp-%d]\n", offset);
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "add eax, 1\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov [ebp-%d], eax\n", offset);

    asmgen_emit_indent(gen);
    asmgen_emit(gen, "jmp %s\n", loop_label);
    asmgen_emit(gen, "%s:\n", end_label);
    free(loop_label);
    free(end_label);
}

void asmgen_generate_function(AsmGen* gen, ASTNode* node) {
    asmgen_emit(gen, "%s:\n", node->data.function.name);
    gen->indent_level++;
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov ebp, esp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "sub esp, 8\n");
    
    if (node->right) {
        asmgen_generate(gen, node->right);
    }
    
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov esp, ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "pop ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "ret\n");
    gen->indent_level--;
}

void asmgen_generate_class(AsmGen* gen, ASTNode* node) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "; Class: %s (non implémenté en assembleur)\n", node->data.identifier_name);
}

void asmgen_generate_return(AsmGen* gen, ASTNode* node) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "mov esp, ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "pop ebp\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "ret\n");
}

void asmgen_generate_print(AsmGen* gen, ASTNode* node) {
    // Évaluer l'expression à afficher (résultat dans eax)
    asmgen_generate_expression(gen, node->left);
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push eax\n");
    asmgen_emit_indent(gen);
    // NOTE (limitation connue de ce backend simplifié): le format d'affichage
    // est choisi de façon STATIQUE, uniquement lorsque l'expression affichée
    // est directement un littéral de chaîne. Une variable contenant une
    // chaîne (ex: he nom = "Alain" ; tehap(nom)) sera affichée avec
    // format_int car ce backend ne fait pas de suivi de type par variable
    // (contrairement au mode -c, entièrement dynamique via AlgiValue).
    if (node->left && node->left->type == NODE_STRING) {
        asmgen_emit(gen, "push format_str\n");
    } else {
        asmgen_emit(gen, "push format_int\n");
    }
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "call printf\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "add esp, 8\n");
}

void asmgen_generate_input(AsmGen* gen, ASTNode* node) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push input_buffer\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push format_str\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "call scanf\n");
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "add esp, 8\n");
}

// ===== GÉNÉRATION D'EXPRESSIONS =====

void asmgen_generate_expression(AsmGen* gen, ASTNode* node) {
    if (!node) {
        asmgen_emit_indent(gen);
        asmgen_emit(gen, "mov eax, 0\n");
        return;
    }
    
    switch (node->type) {
        case NODE_NUMBER:
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "mov eax, %d\n", (int)node->data.number_value);
            break;
            
        case NODE_STRING: {
            // BUG CORRIGÉ: l'ancien code émettait "mov eax, <texte brut>",
            // ce qui n'est PAS un opérande NASM valide (une chaîne littérale
            // doit être placée en section .data et référencée par son
            // étiquette). On utilise ici l'étiquette collectée au
            // préalable par asmgen_collect_strings.
            const char* label = asmgen_find_string_label(gen, node);
            asmgen_emit_indent(gen);
            if (label) {
                asmgen_emit(gen, "lea eax, [%s]\n", label);
            } else {
                asmgen_emit(gen, "; Chaîne non recensée, valeur nulle utilisée\n");
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "mov eax, 0\n");
            }
            break;
        }
            
        case NODE_BOOLEAN:
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "mov eax, %d\n", node->data.bool_value ? 1 : 0);
            break;
            
        case NODE_NULL:
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "mov eax, 0\n");
            break;
            
        case NODE_IDENTIFIER: {
            int offset = asmgen_find_var_offset(gen, node->data.identifier_name);
            asmgen_emit_indent(gen);
            if (offset == -1) {
                asmgen_emit(gen, "; Variable inconnue: %s\n", node->data.identifier_name);
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "mov eax, 0\n");
            } else {
                asmgen_emit(gen, "mov eax, [ebp-%d]\n", offset);
            }
            break;
        }
            
        case NODE_BINARY_OP:
            // Évaluer l'opérande gauche
            asmgen_generate_expression(gen, node->left);
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "push eax\n");
            
            // Évaluer l'opérande droite
            asmgen_generate_expression(gen, node->right);
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "pop ebx\n");
            
            // Effectuer l'opération
            switch (node->data.operator_type) {
                case OP_ADD:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "add eax, ebx\n");
                    break;
                case OP_SUB:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "sub eax, ebx\n");
                    break;
                case OP_MUL:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "imul eax, ebx\n");
                    break;
                case OP_DIV:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cdq\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "idiv ebx\n");
                    break;
                case OP_EQ:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "sete al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case OP_NE:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "setne al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case OP_LT:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "setl al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case OP_GT:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "setg al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case AST_OP_LE:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "setle al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case AST_OP_GE:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "cmp eax, ebx\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "setge al\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "movzx eax, al\n");
                    break;
                case OP_AND:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "and eax, ebx\n");
                    break;
                case OP_OR:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "or eax, ebx\n");
                    break;
                default:
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "; Opérateur non supporté\n");
                    asmgen_emit_indent(gen);
                    asmgen_emit(gen, "mov eax, 0\n");
                    break;
            }
            break;
            
        case NODE_UNARY_OP:
            asmgen_generate_expression(gen, node->left);
            if (node->data.operator_type == OP_NEG) {
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "neg eax\n");
            } else if (node->data.operator_type == OP_NOT) {
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "cmp eax, 0\n");
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "sete al\n");
                asmgen_emit_indent(gen);
                asmgen_emit(gen, "movzx eax, al\n");
            }
            break;
            
        default:
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "; Expression non supportée: %s\n", node_AST_type_to_string(node->type));
            asmgen_emit_indent(gen);
            asmgen_emit(gen, "mov eax, 0\n");
            break;
    }
}

// ===== FONCTIONS D'UTILITÉ =====

char* asmgen_temp_name(AsmGen* gen) {
    static char name[32];
    sprintf(name, "_temp_%d", gen->temp_counter++);
    return strdup(name);
}

char* asmgen_label_name(AsmGen* gen, const char* prefix) {
    static char name[64];
    sprintf(name, "_%s_%d", prefix, gen->label_counter++);
    return strdup(name);
}

void asmgen_push_value(AsmGen* gen, const char* value) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "push %s\n", value);
    gen->stack_size += 4;
}

void asmgen_pop_value(AsmGen* gen, const char* dest) {
    asmgen_emit_indent(gen);
    asmgen_emit(gen, "pop %s\n", dest);
    gen->stack_size -= 4;
}