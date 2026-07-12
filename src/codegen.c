// Implémentation du générateur de code C pour ALGI

#include "codegen.h"
#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// ===== FONCTIONS PRINCIPALES =====

void generate_code(ASTNode* node, const char* filename) {
    generate_code_with_runtime(node, filename, "runtime.h");
}

void generate_code_with_runtime(ASTNode* node, const char* filename, const char* runtime_path) {
    if (!node) {
        fprintf(stderr, "❌ Erreur: AST NULL\n");
        return;
    }

    FILE* output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "❌ Erreur: impossible d'écrire %s\n", filename);
        return;
    }

    CodeGen gen;
    codegen_init(&gen, output, NULL);
    gen.runtime_path = runtime_path;

    // En-tête du fichier C
    time_t now = time(NULL);
    fprintf(output, "// Code généré par le compilateur ALGI\n");
    fprintf(output, "// Fichier: %s\n", filename);
    fprintf(output, "// Date: %s", ctime(&now));
    fprintf(output, "\n");

    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#include <stdlib.h>\n");
    fprintf(output, "#include <string.h>\n");
    fprintf(output, "#include <stdbool.h>\n");
    fprintf(output, "#include <math.h>\n");
    fprintf(output, "#include \"%s\"\n", runtime_path);
    fprintf(output, "\n");

    // ===== PASSE 1 : définitions globales (fonctions et classes) =====
    // Les fonctions et classes ALGI sont hissées au niveau global du
    // fichier C, AVANT main(), ce qui est du C standard portable (la
    // version précédente les générait imbriquées à l'intérieur de
    // main(), ce qui ne fonctionne qu'en s'appuyant sur une extension
    // GNU non standard).
    if (node->type == NODE_PROGRAM || node->type == NODE_BLOCK) {
        for (int i = 0; i < node->data.block.count; i++) {
            ASTNode* instr = node->data.block.instructions[i];
            if (!instr) continue;
            if (instr->type == NODE_FUNCTION) {
                codegen_generate_function(&gen, instr);
                fprintf(output, "\n");
            } else if (instr->type == NODE_CLASS) {
                codegen_generate_class(&gen, instr);
                fprintf(output, "\n");
            }
        }
    }

    // ===== PASSE 2 : point d'entrée =====
    fprintf(output, "int main(void) {\n");
    gen.indent_level++;

    if (node->type == NODE_PROGRAM || node->type == NODE_BLOCK) {
        for (int i = 0; i < node->data.block.count; i++) {
            ASTNode* instr = node->data.block.instructions[i];
            if (!instr) continue;
            if (instr->type == NODE_FUNCTION || instr->type == NODE_CLASS) {
                continue; // déjà générées ci-dessus
            }
            codegen_generate(&gen, instr);
        }
    }

    gen.indent_level--;
    fprintf(output, "  return 0;\n");
    fprintf(output, "}\n");

    fclose(output);
}

// ===== INITIALISATION / UTILITAIRES DE BASE =====

void codegen_init(CodeGen* gen, FILE* output, SymbolTable* symbols) {
    gen->output = output;
    gen->indent_level = 0;
    gen->temp_counter = 0;
    gen->symbols = symbols;
    gen->runtime_path = "runtime.h";
}

void codegen_emit(CodeGen* gen, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(gen->output, format, args);
    va_end(args);
}

void codegen_emit_indent(CodeGen* gen) {
    for (int i = 0; i < gen->indent_level; i++) {
        fprintf(gen->output, "  ");
    }
}

char* codegen_temp_name(CodeGen* gen) {
    char buf[32];
    snprintf(buf, sizeof(buf), "_algi_tmp_%d", gen->temp_counter++);
    return strdup(buf);
}

char* codegen_label_name(CodeGen* gen, const char* prefix) {
    char buf[64];
    snprintf(buf, sizeof(buf), "_%s_%d", prefix, gen->temp_counter++);
    return strdup(buf);
}

// Échappe une chaîne ALGI pour l'insérer telle quelle dans un littéral C
// ("...", entre guillemets). Sans cela, une chaîne contenant un guillemet,
// un antislash ou un retour à la ligne casserait la compilation du code
// C généré, voire changerait la sémantique du programme.
char* codegen_escape_string(const char* input) {
    if (!input) return strdup("");
    size_t len = strlen(input);
    // Pire cas: chaque caractère doit être échappé (2 caractères en sortie)
    char* out = (char*)malloc(len * 2 + 1);
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        switch (c) {
            case '"':  out[j++] = '\\'; out[j++] = '"';  break;
            case '\\': out[j++] = '\\'; out[j++] = '\\'; break;
            case '\n': out[j++] = '\\'; out[j++] = 'n';  break;
            case '\t': out[j++] = '\\'; out[j++] = 't';  break;
            case '\r': out[j++] = '\\'; out[j++] = 'r';  break;
            default:   out[j++] = (char)c; break;
        }
    }
    out[j] = '\0';
    return out;
}

// ===== DISPATCH GÉNÉRAL (instructions) =====

void codegen_generate(CodeGen* gen, ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            codegen_generate_program(gen, node);
            break;
        case NODE_DECLARATION:
            codegen_generate_declaration(gen, node);
            break;
        case NODE_ASSIGNMENT:
            codegen_generate_assignment(gen, node);
            break;
        case NODE_IF:
            codegen_generate_if(gen, node);
            break;
        case NODE_WHILE:
            codegen_generate_while(gen, node);
            break;
        case NODE_FOR:
            codegen_generate_for(gen, node);
            break;
        case NODE_FUNCTION:
            codegen_generate_function(gen, node);
            break;
        case NODE_CLASS:
            codegen_generate_class(gen, node);
            break;
        case NODE_RETURN:
            codegen_generate_return(gen, node);
            break;
        case NODE_BREAK:
            codegen_generate_break(gen, node);
            break;
        case NODE_PRINT:
            codegen_generate_print(gen, node);
            break;
        case NODE_INPUT:
            codegen_generate_input(gen, node);
            break;
        case NODE_TRY_CATCH:
            codegen_generate_try_catch(gen, node);
            break;
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_NUMBER:
        case NODE_STRING:
        case NODE_BOOLEAN:
        case NODE_NULL:
        case NODE_IDENTIFIER:
        case NODE_CALL:
        case NODE_LIST:
        case NODE_INDEX_ACCESS:
        case NODE_FIELD_ACCESS:
        case NODE_NEW_INSTANCE: {
            // Expression utilisée seule comme instruction (ex: appel de
            // fonction dont le résultat est ignoré).
            char* expr = codegen_gen_expr(gen, node);
            codegen_emit_indent(gen);
            codegen_emit(gen, "%s;\n", expr);
            free(expr);
            break;
        }
        default:
            codegen_emit_indent(gen);
            codegen_emit(gen, "// Nœud non supporté: %s\n", node_AST_type_to_string(node->type));
            break;
    }
}

// ===== INSTRUCTIONS =====

void codegen_generate_program(CodeGen* gen, ASTNode* node) {
    for (int i = 0; i < node->data.block.count; i++) {
        codegen_generate(gen, node->data.block.instructions[i]);
    }
}

void codegen_generate_declaration(CodeGen* gen, ASTNode* node) {
    char* expr = codegen_gen_expr(gen, node->left);
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiValue* %s = %s;\n", node->data.identifier_name, expr);
    free(expr);
}

void codegen_generate_assignment(CodeGen* gen, ASTNode* node) {
    char* expr = codegen_gen_expr(gen, node->left);

    if (node->data.identifier_name) {
        // Cas simple: identifiant = expression
        codegen_emit_indent(gen);
        codegen_emit(gen, "%s = %s;\n", node->data.identifier_name, expr);
    } else if (node->right) {
        // Cas d'une affectation sur une cible postfixe (liste[i] = ... ou
        // objet.champ = ...) : la cible est stockée dans node->right
        // (cf. correction du bug dans algi.y).
        ASTNode* target = node->right;
        if (target->type == NODE_INDEX_ACCESS) {
            char* list_expr = codegen_gen_expr(gen, target->left);
            char* index_expr = codegen_gen_expr(gen, target->right);
            codegen_emit_indent(gen);
            codegen_emit(gen, "algi_index_set(%s, %s, %s);\n", list_expr, index_expr, expr);
            free(list_expr);
            free(index_expr);
        } else if (target->type == NODE_FIELD_ACCESS) {
            char* obj_expr = codegen_gen_expr(gen, target->left);
            char* escaped_field = codegen_escape_string(target->data.identifier_name);
            codegen_emit_indent(gen);
            codegen_emit(gen, "algi_field_set_value(%s, \"%s\", %s);\n", obj_expr, escaped_field, expr);
            free(obj_expr);
            free(escaped_field);
        } else {
            codegen_emit_indent(gen);
            codegen_emit(gen, "// Cible d'affectation non supportée\n");
        }
    } else {
        codegen_emit_indent(gen);
        codegen_emit(gen, "// Affectation invalide (aucune cible)\n");
    }

    free(expr);
}

void codegen_generate_if(CodeGen* gen, ASTNode* node) {
    char* cond = codegen_gen_expr(gen, node->left);
    codegen_emit_indent(gen);
    codegen_emit(gen, "if (algi_value_to_bool(%s)) {\n", cond);
    free(cond);

    gen->indent_level++;
    codegen_generate(gen, node->right);
    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "}\n");

    // Bloc 'kommenu' (else), stocké dans node->extra (cf. correction du
    // bug dans ast.c/ast.h : ce bloc était auparavant totalement perdu).
    if (node->extra) {
        codegen_emit_indent(gen);
        codegen_emit(gen, "else {\n");
        gen->indent_level++;
        codegen_generate(gen, node->extra);
        gen->indent_level--;
        codegen_emit_indent(gen);
        codegen_emit(gen, "}\n");
    }
}

void codegen_generate_while(CodeGen* gen, ASTNode* node) {
    // La condition ne nécessite pas d'instructions préparatoires pour les
    // formes usuelles (identifiants, comparaisons...), donc elle peut être
    // insérée directement dans le test C sans perte de correction : elle
    // sera bien ré-évaluée à chaque itération.
    char* cond = codegen_gen_expr(gen, node->left);
    codegen_emit_indent(gen);
    codegen_emit(gen, "while (algi_value_to_bool(%s)) {\n", cond);
    free(cond);

    gen->indent_level++;
    codegen_generate(gen, node->right);
    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "}\n");
}

void codegen_generate_for(CodeGen* gen, ASTNode* node) {
    char* var_name = node->data.identifier_name;
    char* iterable_expr = codegen_gen_expr(gen, node->left);
    char* list_ptr = codegen_temp_name(gen);
    char* idx = codegen_temp_name(gen);

    codegen_emit_indent(gen);
    codegen_emit(gen, "// Boucle 'pou' sur %s\n", var_name);
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiValue* %s_src = %s;\n", list_ptr, iterable_expr);
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiList* %s = (AlgiList*)(%s_src->value.ptr_value);\n", list_ptr, list_ptr);
    codegen_emit_indent(gen);
    codegen_emit(gen, "for (int %s = 0; %s < algi_list_size(%s); %s++) {\n",
                 idx, idx, list_ptr, idx);
    gen->indent_level++;
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiValue* %s = algi_list_get(%s, %s);\n", var_name, list_ptr, idx);
    codegen_generate(gen, node->right);
    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "}\n");

    free(iterable_expr);
    free(list_ptr);
    free(idx);
}

void codegen_generate_function(CodeGen* gen, ASTNode* node) {
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiValue* %s(", node->data.function.name);
    int param_count = node->data.function.count;
    if (param_count == 0) {
        codegen_emit(gen, "void");
    } else {
        for (int i = 0; i < param_count; i++) {
            if (i > 0) codegen_emit(gen, ", ");
            codegen_emit(gen, "AlgiValue* %s", node->data.function.names[i]);
        }
    }
    codegen_emit(gen, ") {\n");
    gen->indent_level++;

    if (node->right) {
        codegen_generate(gen, node->right);
    }

    // Filet de sécurité: si le corps ne contient pas de 'tem' explicite sur
    // tous les chemins, on retourne NULL plutôt que de laisser un
    // comportement indéfini (une fonction C déclarée comme retournant
    // AlgiValue* qui "tombe" sans return est un comportement indéfini).
    codegen_emit_indent(gen);
    codegen_emit(gen, "return algi_make_null();\n");

    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "}\n");
}

void codegen_generate_class(CodeGen* gen, ASTNode* node) {
    codegen_emit_indent(gen);
    codegen_emit(gen, "// Classe ALGI: %s\n", node->data.identifier_name);
    codegen_emit_indent(gen);
    codegen_emit(gen, "// (représentation simplifiée: les champs sont gérés dynamiquement\n");
    codegen_emit_indent(gen);
    codegen_emit(gen, "//  via AlgiObject / algi_object_* à l'exécution, cf. runtime.c)\n");
    // NOTE: le runtime ALGI représente les objets de façon dynamique
    // (AlgiObject avec un tableau de champs), il n'est donc pas nécessaire
    // ni cohérent de générer un "typedef struct" figé par classe: on se
    // contente de documenter la classe ici. La création d'instance se fait
    // via algi_make_object(algi_object_create("NomClasse")).
    (void)node;
}

void codegen_generate_return(CodeGen* gen, ASTNode* node) {
    if (node->left) {
        char* expr = codegen_gen_expr(gen, node->left);
        codegen_emit_indent(gen);
        codegen_emit(gen, "return %s;\n", expr);
        free(expr);
    } else {
        codegen_emit_indent(gen);
        codegen_emit(gen, "return algi_make_null();\n");
    }
}

void codegen_generate_break(CodeGen* gen, ASTNode* node) {
    (void)node;
    codegen_emit_indent(gen);
    codegen_emit(gen, "break;\n");
}

void codegen_generate_print(CodeGen* gen, ASTNode* node) {
    char* expr = codegen_gen_expr(gen, node->left);
    codegen_emit_indent(gen);
    codegen_emit(gen, "algi_print(%s);\n", expr);
    free(expr);
}

void codegen_generate_input(CodeGen* gen, ASTNode* node) {
    // BUG CORRIGE: l'ancienne implementation evaluait l'expression passee
    // a keksie(...) (par ex. la variable cible) pour s'en servir de
    // "prompt", appelait algi_input_from_value(), puis JETAIT le resultat
    // (l'appel etait fait comme une instruction seule, sans affectation).
    // Consequence: `keksie(x)` ne modifiait jamais x - il etait donc
    // impossible de recuperer une entree utilisateur avec ce mot-cle,
    // ce qui rend "lire" totalement inutilisable en pratique (ex: pour
    // une calculatrice).
    //
    // Nouveau comportement, conforme a l'esprit de "keksie = lire" :
    //   keksie(x)  ->  x = algi_input_from_value(NULL);
    // c'est-a-dire que la valeur lue au clavier est desormais stockee
    // dans la variable x (x doit avoir ete declaree au prealable avec
    // 'he'). Aucun prompt n'est affiche automatiquement: comme dans tous
    // les exemples du langage, on affiche le message d'invite avec
    // tehap(...) juste avant, ce qui reste coherent avec le style ALGI.
    if (node->left && node->left->type == NODE_IDENTIFIER) {
        codegen_emit_indent(gen);
        codegen_emit(gen, "%s = algi_input_from_value(NULL);\n",
                     node->left->data.identifier_name);
        return;
    }

    // Repli (compatibilite): si keksie() est appele sur autre chose qu'un
    // simple identifiant (ex: keksie("texte")), on garde l'ancien
    // comportement de base (affiche l'expression comme prompt et lit une
    // ligne, sans pouvoir la stocker nulle part) plutot que de produire
    // une erreur de compilation.
    char* expr = codegen_gen_expr(gen, node->left);
    codegen_emit_indent(gen);
    codegen_emit(gen, "algi_input_from_value(%s);\n", expr);
    free(expr);
}

void codegen_generate_try_catch(CodeGen* gen, ASTNode* node) {
    // Implémentation réelle du try/catch via setjmp/longjmp (le runtime
    // fournit déjà algi_push_try/algi_setjmp_try/algi_pop_try/algi_throw;
    // la version précédente ne générait qu'un commentaire, sans aucune
    // protection effective).
    codegen_emit_indent(gen);
    codegen_emit(gen, "algi_push_try();\n");
    codegen_emit_indent(gen);
    codegen_emit(gen, "if (algi_setjmp_try() == 0) {\n");
    gen->indent_level++;
    codegen_generate(gen, node->left);
    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "} else {\n");
    gen->indent_level++;
    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiValue* %s = algi_make_string(algi_get_last_error());\n",
                 node->data.identifier_name);
    codegen_generate(gen, node->right);
    gen->indent_level--;
    codegen_emit_indent(gen);
    codegen_emit(gen, "}\n");
    codegen_emit_indent(gen);
    codegen_emit(gen, "algi_pop_try();\n");
}

// ===== EXPRESSIONS =====
// Chaque fonction ci-dessous renvoie une chaîne allouée (à libérer par
// l'appelant) contenant une expression C de type AlgiValue*. Les nœuds qui
// nécessitent des instructions intermédiaires (listes) les émettent
// directement dans gen->output AVANT de renvoyer leur texte, ce qui
// garantit un ordre correct dans le fichier de sortie.

char* codegen_gen_expr(CodeGen* gen, ASTNode* node) {
    if (!node) {
        return strdup("algi_make_null()");
    }

    char buf[512];

    switch (node->type) {
        case NODE_NUMBER:
            if (node->data_type == AST_TYPE_INT) {
                snprintf(buf, sizeof(buf), "algi_make_int(%ldL)", (long)node->data.number_value);
            } else {
                snprintf(buf, sizeof(buf), "algi_make_float(%g)", node->data.number_value);
            }
            return strdup(buf);

        case NODE_STRING: {
            char* escaped = codegen_escape_string(node->data.string_value);
            char* result = (char*)malloc(strlen(escaped) + 32);
            sprintf(result, "algi_make_string(\"%s\")", escaped);
            free(escaped);
            return result;
        }

        case NODE_BOOLEAN:
            return strdup(node->data.bool_value ? "algi_make_bool(true)" : "algi_make_bool(false)");

        case NODE_NULL:
            return strdup("algi_make_null()");

        case NODE_IDENTIFIER:
            // L'identifiant désigne déjà une variable C de type AlgiValue*
            // (ou, dans un appel de fonction, le nom de la fonction).
            return strdup(node->data.identifier_name);

        case NODE_BINARY_OP:
            return codegen_gen_binary_op(gen, node);

        case NODE_UNARY_OP:
            return codegen_gen_unary_op(gen, node);

        case NODE_CALL:
            return codegen_gen_call(gen, node);

        case NODE_LIST:
            return codegen_gen_list(gen, node);

        case NODE_INDEX_ACCESS:
            return codegen_gen_index_access(gen, node);

        case NODE_FIELD_ACCESS:
            return codegen_gen_field_access(gen, node);

        case NODE_NEW_INSTANCE:
            return codegen_gen_new_instance(gen, node);

        default:
            return strdup("algi_make_null() /* expression non supportée */");
    }
}

char* codegen_gen_binary_op(CodeGen* gen, ASTNode* node) {
    char* left = codegen_gen_expr(gen, node->left);
    char* right = codegen_gen_expr(gen, node->right);

    const char* func;
    switch (node->data.operator_type) {
        case OP_ADD:    func = "algi_add";          break;
        case OP_SUB:    func = "algi_sub";          break;
        case OP_MUL:    func = "algi_mul";          break;
        case OP_DIV:    func = "algi_div";          break;
        case OP_MOD:    func = "algi_mod";          break;
        case OP_EQ:     func = "algi_equals";       break;
        case OP_NE:     func = "algi_not_equals";   break;
        case OP_LT:     func = "algi_cmp_lt";       break;
        case OP_GT:     func = "algi_cmp_gt";       break;
        case AST_OP_LE: func = "algi_cmp_le";       break;
        case AST_OP_GE: func = "algi_cmp_ge";       break;
        case OP_AND:    func = "algi_logical_and";  break;
        case OP_OR:     func = "algi_logical_or";   break;
        case OP_IS:     func = "algi_is_type";      break;
        case OP_IN:     func = "algi_in_list";      break;
        default:        func = "algi_add";          break;
    }

    size_t size = strlen(func) + strlen(left) + strlen(right) + 16;
    char* result = (char*)malloc(size);
    snprintf(result, size, "%s(%s, %s)", func, left, right);

    free(left);
    free(right);
    return result;
}

char* codegen_gen_unary_op(CodeGen* gen, ASTNode* node) {
    char* operand = codegen_gen_expr(gen, node->left);
    const char* func = (node->data.operator_type == OP_NOT) ? "algi_logical_not" : "algi_negate";

    size_t size = strlen(func) + strlen(operand) + 8;
    char* result = (char*)malloc(size);
    snprintf(result, size, "%s(%s)", func, operand);

    free(operand);
    return result;
}

char* codegen_gen_call(CodeGen* gen, ASTNode* node) {
    char* callee = codegen_gen_expr(gen, node->left);

    int argc = node->data.call.count;
    char** arg_strs = NULL;
    if (argc > 0) {
        arg_strs = (char**)malloc(argc * sizeof(char*));
        for (int i = 0; i < argc; i++) {
            arg_strs[i] = codegen_gen_expr(gen, node->data.call.args[i]);
        }
    }

    size_t total = strlen(callee) + 8;
    for (int i = 0; i < argc; i++) total += strlen(arg_strs[i]) + 2;

    char* result = (char*)malloc(total);
    strcpy(result, callee);
    strcat(result, "(");
    for (int i = 0; i < argc; i++) {
        if (i > 0) strcat(result, ", ");
        strcat(result, arg_strs[i]);
    }
    strcat(result, ")");

    free(callee);
    for (int i = 0; i < argc; i++) free(arg_strs[i]);
    if (arg_strs) free(arg_strs);

    return result;
}

char* codegen_gen_list(CodeGen* gen, ASTNode* node) {
    // Émet les instructions de construction de la liste directement dans
    // gen->output (elles précèdent donc bien, dans le fichier généré,
    // l'instruction englobante qui utilisera cette liste), puis renvoie
    // une expression qui enveloppe le résultat dans un AlgiValue*.
    char* list_name = codegen_temp_name(gen);

    codegen_emit_indent(gen);
    codegen_emit(gen, "AlgiList* %s = algi_list_create(%d);\n",
                 list_name, node->data.list.count > 0 ? node->data.list.count : 4);

    for (int i = 0; i < node->data.list.count; i++) {
        char* elem_expr = codegen_gen_expr(gen, node->data.list.elements[i]);
        codegen_emit_indent(gen);
        codegen_emit(gen, "algi_list_append(%s, %s);\n", list_name, elem_expr);
        free(elem_expr);
    }

    size_t size = strlen(list_name) + 32;
    char* result = (char*)malloc(size);
    snprintf(result, size, "algi_make_list(%s)", list_name);
    free(list_name);
    return result;
}

char* codegen_gen_index_access(CodeGen* gen, ASTNode* node) {
    char* list_expr = codegen_gen_expr(gen, node->left);
    char* index_expr = codegen_gen_expr(gen, node->right);

    size_t size = strlen(list_expr) + strlen(index_expr) + 32;
    char* result = (char*)malloc(size);
    snprintf(result, size, "algi_index_get(%s, %s)", list_expr, index_expr);

    free(list_expr);
    free(index_expr);
    return result;
}

char* codegen_gen_field_access(CodeGen* gen, ASTNode* node) {
    char* obj_expr = codegen_gen_expr(gen, node->left);
    char* escaped_field = codegen_escape_string(node->data.identifier_name);

    size_t size = strlen(obj_expr) + strlen(escaped_field) + 32;
    char* result = (char*)malloc(size);
    snprintf(result, size, "algi_field_get_value(%s, \"%s\")", obj_expr, escaped_field);

    free(obj_expr);
    free(escaped_field);
    return result;
}

char* codegen_gen_new_instance(CodeGen* gen, ASTNode* node) {
    (void)gen;
    // NOTE: le runtime ALGI ne lie pas les arguments du constructeur à des
    // champs (limitation assumée du runtime existant, en dehors du
    // périmètre de cette correction). L'instance est créée avec sa classe
    // et ses champs pourront être ajoutés dynamiquement via
    // algi_field_set_value.
    char* escaped_class = codegen_escape_string(node->data.call.name);
    size_t size = strlen(escaped_class) + 48;
    char* result = (char*)malloc(size);
    snprintf(result, size, "algi_make_object(algi_object_create(\"%s\"))", escaped_class);
    free(escaped_class);
    return result;
}
