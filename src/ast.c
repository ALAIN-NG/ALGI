// Implémentation de l'Arbre Syntaxique Abstrait (AST) pour ALGI

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>

// ===== FONCTIONS DE CRÉATION DE NŒUDS =====

ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "❌ Erreur: allocation mémoire échouée\n");
        exit(1);
    }
    memset(node, 0, sizeof(ASTNode));
    node->type = type;
    node->data_type = AST_TYPE_UNKNOWN;
    return node;
}

ASTNode* create_program() {
    ASTNode* node = create_node(NODE_PROGRAM);
    node->data.block.instructions = NULL;
    node->data.block.count = 0;
    return node;
}

ASTNode* create_block(ASTNode** instructions) {
    ASTNode* node = create_node(NODE_BLOCK);
    node->data.block.instructions = instructions;
    // Compter les instructions
    int count = 0;
    if (instructions) {
        while (instructions[count]) count++;
    }
    node->data.block.count = count;
    return node;
}

ASTNode* create_declaration(char* name, ASTNode* expr) {
    ASTNode* node = create_node(NODE_DECLARATION);
    node->data.identifier_name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* create_assignment(char* name, ASTNode* expr) {
    ASTNode* node = create_node(NODE_ASSIGNMENT);
    if (name) {
        node->data.identifier_name = strdup(name);
    }
    node->left = expr;
    return node;
}

ASTNode* create_if(ASTNode* condition, ASTNode* then_block, ASTNode* else_block) {
    ASTNode* node = create_node(NODE_IF);
    node->left = condition;
    node->right = then_block;
    node->extra = else_block; // Peut être NULL si pas de 'kommenu'
    return node;
}

ASTNode* create_while(ASTNode* condition, ASTNode* body) {
    ASTNode* node = create_node(NODE_WHILE);
    node->left = condition;
    node->right = body;
    return node;
}

ASTNode* create_for(char* var_name, ASTNode* iterable, ASTNode* body) {
    ASTNode* node = create_node(NODE_FOR);
    node->data.identifier_name = strdup(var_name);
    node->left = iterable;
    node->right = body;
    return node;
}

ASTNode* create_function(char* name, char** params, int param_count, ASTNode* body) {
    ASTNode* node = create_node(NODE_FUNCTION);
    node->data.function.name = strdup(name);
    node->data.function.names = params;
    node->data.function.count = param_count;
    node->right = body;
    return node;
}

ASTNode* create_class(char* name, ASTNode* body) {
    ASTNode* node = create_node(NODE_CLASS);
    node->data.identifier_name = strdup(name);
    node->right = body;
    return node;
}

ASTNode* create_return(ASTNode* expr) {
    ASTNode* node = create_node(NODE_RETURN);
    node->left = expr;
    return node;
}

ASTNode* create_break() {
    return create_node(NODE_BREAK);
}

ASTNode* create_new_instance(char* class_name, ASTNode** args) {
    ASTNode* node = create_node(NODE_NEW_INSTANCE);
    node->data.call.name = strdup(class_name);
    node->data.call.args = args;
    // Compter les arguments
    int count = 0;
    if (args) {
        while (args[count]) count++;
    }
    node->data.call.count = count;
    return node;
}

ASTNode* create_try_catch(ASTNode* try_block, char* catch_var, ASTNode* catch_block) {
    ASTNode* node = create_node(NODE_TRY_CATCH);
    node->left = try_block;
    node->data.identifier_name = strdup(catch_var);
    node->right = catch_block;
    return node;
}

ASTNode* create_print(ASTNode* expr) {
    ASTNode* node = create_node(NODE_PRINT);
    node->left = expr;
    return node;
}

ASTNode* create_input(ASTNode* expr) {
    ASTNode* node = create_node(NODE_INPUT);
    node->left = expr;
    return node;
}

ASTNode* create_binary_op(OperatorType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_node(NODE_BINARY_OP);
    node->data.operator_type = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_unary_op(OperatorType op, ASTNode* expr) {
    ASTNode* node = create_node(NODE_UNARY_OP);
    node->data.operator_type = op;
    node->left = expr;
    return node;
}

ASTNode* create_number(double value) {
    ASTNode* node = create_node(NODE_NUMBER);
    node->data.number_value = value;
    node->data_type = AST_TYPE_FLOAT;
    // Si c'est un entier, on le marque comme tel
    if (value == floor(value)) {
        node->data_type = AST_TYPE_INT;
    }
    return node;
}

ASTNode* create_string(char* value) {
    ASTNode* node = create_node(NODE_STRING);
    // Enlever les guillemets
    char* str = value;
    if (str[0] == '"') {
        str = strdup(value + 1);
        str[strlen(str) - 1] = '\0';
        node->data.string_value = str;
    } else {
        node->data.string_value = strdup(value);
    }
    node->data_type = AST_TYPE_STRING;
    return node;
}

ASTNode* create_boolean(int value) {
    ASTNode* node = create_node(NODE_BOOLEAN);
    node->data.bool_value = value;
    node->data_type = AST_TYPE_BOOL;
    return node;
}

ASTNode* create_null() {
    ASTNode* node = create_node(NODE_NULL);
    node->data_type = AST_TYPE_VOID;
    return node;
}

ASTNode* create_identifier(char* name) {
    ASTNode* node = create_node(NODE_IDENTIFIER);
    node->data.identifier_name = strdup(name);
    return node;
}

ASTNode* create_AST_type_annotation(DataType type) {
    ASTNode* node = create_node(NODE_AST_TYPE_ANNOTATION);
    node->data.AST_type_value = type;
    return node;
}

ASTNode* create_call(ASTNode* callee, ASTNode** args, int arg_count) {
    ASTNode* node = create_node(NODE_CALL);
    node->left = callee;
    if (args) {
        // BUG CORRIGÉ: la grammaire appelait toujours create_call(..., 0),
        // perdant systématiquement tous les arguments d'appel. On calcule
        // donc nous-mêmes le nombre réel d'arguments à partir du tableau
        // terminé par NULL (comme le fait déjà parameter_list/argument_list
        // côté grammaire), en ignorant le paramètre arg_count non fiable.
        int count = 0;
        while (args[count]) count++;
        node->data.call.args = args;
        node->data.call.count = count;
        (void)arg_count;
    } else {
        node->data.call.args = NULL;
        node->data.call.count = 0;
    }
    return node;
}

ASTNode* create_list(ASTNode** elements, int count) {
    ASTNode* node = create_node(NODE_LIST);
    // BUG CORRIGÉ: la grammaire appelait toujours create_list(..., 0),
    // ce qui vidait silencieusement toute liste littérale (bi[...]) de
    // son contenu lors de la génération de code. On calcule nous-mêmes
    // le nombre réel d'éléments à partir du tableau terminé par NULL.
    if (elements) {
        int real_count = 0;
        while (elements[real_count]) real_count++;
        node->data.list.elements = elements;
        node->data.list.count = real_count;
        (void)count;
    } else {
        node->data.list.elements = NULL;
        node->data.list.count = 0;
    }
    node->data_type = AST_TYPE_LIST;
    return node;
}

ASTNode* create_index_access(ASTNode* expr, ASTNode* index) {
    ASTNode* node = create_node(NODE_INDEX_ACCESS);
    node->left = expr;
    node->right = index;
    return node;
}

ASTNode* create_field_access(ASTNode* expr, char* field) {
    ASTNode* node = create_node(NODE_FIELD_ACCESS);
    node->left = expr;
    node->data.identifier_name = strdup(field);
    return node;
}

ASTNode* create_error(char* message) {
    ASTNode* node = create_node(NODE_ERROR);
    node->data.string_value = strdup(message);
    return node;
}

// ===== FONCTIONS DE GESTION DE L'AST =====

void add_instruction(ASTNode *program, ASTNode *instr)
{
    if (!program || !instr)
        return;

    if (program->type != NODE_PROGRAM &&
        program->type != NODE_BLOCK)
        return;

    int n = program->data.block.count;

    ASTNode **tmp = realloc(
        program->data.block.instructions,
        (n + 1) * sizeof(ASTNode *)
    );

    if (!tmp)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    program->data.block.instructions = tmp;
    program->data.block.instructions[n] = instr;
    program->data.block.count++;
}

void add_argument(ASTNode* call, ASTNode* arg) {
    if (!call || !arg) return;
    if (call->type != NODE_CALL) {
        fprintf(stderr, "⚠️ Attention: add_argument appelé sur un nœud non-appel\n");
        return;
    }
    call->data.call.count++;
    call->data.call.args = (ASTNode**)realloc(call->data.call.args,
        call->data.call.count * sizeof(ASTNode*));
    call->data.call.args[call->data.call.count - 1] = arg;
}

void add_list_element(ASTNode* list, ASTNode* element) {
    if (!list || !element) return;
    if (list->type != NODE_LIST) {
        fprintf(stderr, "⚠️ Attention: add_list_element appelé sur un nœud non-liste\n");
        return;
    }
    list->data.list.count++;
    list->data.list.elements = (ASTNode**)realloc(list->data.list.elements,
        list->data.list.count * sizeof(ASTNode*));
    list->data.list.elements[list->data.list.count - 1] = element;
}

void add_parameter(ASTNode* function, char* param) {
    if (!function || !param) return;
    if (function->type != NODE_FUNCTION) {
        fprintf(stderr, "⚠️ Attention: add_parameter appelé sur un nœud non-fonction\n");
        return;
    }
    function->data.function.count++;
    function->data.function.names = (char**)realloc(function->data.function.names,
        function->data.function.count * sizeof(char*));
    function->data.function.names[function->data.function.count - 1] = strdup(param);
}

void add_field(ASTNode* class_def, char* field_name, DataType type) {
    if (!class_def || !field_name) return;
    if (class_def->type != NODE_CLASS) {
        fprintf(stderr, "⚠️ Attention: add_field appelé sur un nœud non-classe\n");
        return;
    }
    // Les champs sont stockés dans les instructions du corps
    // On crée une déclaration pour chaque champ
    ASTNode* decl = create_declaration(field_name, NULL);
    decl->data_type = type;
    add_instruction(class_def->right, decl);
}

// ===== FONCTIONS D'UTILITÉ =====

const char* node_AST_type_to_string(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "Program";
        case NODE_BLOCK: return "Block";
        case NODE_DECLARATION: return "Declaration";
        case NODE_ASSIGNMENT: return "Assignment";
        case NODE_IF: return "If";
        case NODE_WHILE: return "While";
        case NODE_FOR: return "For";
        case NODE_FUNCTION: return "Function";
        case NODE_CLASS: return "Class";
        case NODE_RETURN: return "Return";
        case NODE_BREAK: return "Break";
        case NODE_NEW_INSTANCE: return "NewInstance";
        case NODE_TRY_CATCH: return "TryCatch";
        case NODE_PRINT: return "Print";
        case NODE_INPUT: return "Input";
        case NODE_BINARY_OP: return "BinaryOp";
        case NODE_UNARY_OP: return "UnaryOp";
        case NODE_NUMBER: return "Number";
        case NODE_STRING: return "String";
        case NODE_BOOLEAN: return "Boolean";
        case NODE_NULL: return "Null";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_AST_TYPE_ANNOTATION: return "TypeAnnotation";
        case NODE_CALL: return "Call";
        case NODE_LIST: return "List";
        case NODE_INDEX_ACCESS: return "IndexAccess";
        case NODE_FIELD_ACCESS: return "FieldAccess";
        case NODE_ERROR: return "Error";
        default: return "Unknown";
    }
}

const char* data_AST_type_to_string(DataType type) {
    switch (type) {
        case AST_TYPE_INT: return "int";
        case AST_TYPE_FLOAT: return "float";
        case AST_TYPE_STRING: return "string";
        case AST_TYPE_BOOL: return "bool";
        case AST_TYPE_LIST: return "list";
        case AST_TYPE_OBJECT: return "object";
        case AST_TYPE_VOID: return "void";
        default: return "unknown";
    }
}

const char* operator_to_string(OperatorType op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_EQ: return "==";
        case OP_NE: return "!=";
        case OP_LT: return "<";
        case OP_GT: return ">";
        case AST_OP_LE: return "<=";
        case AST_OP_GE: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_NOT: return "!";
        case OP_IS: return "is";
        case OP_IN: return "in";
        case OP_NEG: return "-";
        default: return "?";
    }
}

// ===== FONCTIONS DE LIBÉRATION =====

void free_ast(ASTNode* node) {
    if (!node) return;

    // Libérer les enfants récursivement
    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);
    if (node->extra) free_ast(node->extra);

    // Libérer les données selon le type réel du nœud.
    // ATTENTION: "data" est une union - on ne peut lire QUE le membre
    // correspondant au type du nœud, sous peine de lire de la mémoire
    // invalide et de provoquer un crash lors du free().
    switch (node->type) {
        case NODE_STRING:
        case NODE_ERROR:
            free(node->data.string_value);
            break;

        case NODE_DECLARATION:
        case NODE_ASSIGNMENT:
        case NODE_FOR:
        case NODE_TRY_CATCH:
        case NODE_IDENTIFIER:
        case NODE_FIELD_ACCESS:
            free(node->data.identifier_name);
            break;

        case NODE_NEW_INSTANCE:
            free(node->data.call.name);
            if (node->data.call.args) {
                for (int i = 0; i < node->data.call.count; i++) {
                    free_ast(node->data.call.args[i]);
                }
                free(node->data.call.args);
            }
            break;

        case NODE_LIST:
            if (node->data.list.elements) {
                for (int i = 0; i < node->data.list.count; i++) {
                    if (node->data.list.elements[i]) {
                        free_ast(node->data.list.elements[i]);
                    }
                }
                free(node->data.list.elements);
            }
            break;

        case NODE_CALL:
            if (node->data.call.args) {
                for (int i = 0; i < node->data.call.count; i++) {
                    if (node->data.call.args[i]) {
                        free_ast(node->data.call.args[i]);
                    }
                }
                free(node->data.call.args);
            }
            break;

        case NODE_PROGRAM:
        case NODE_BLOCK:
            if (node->data.block.instructions) {
                for (int i = 0; i < node->data.block.count; i++) {
                    if (node->data.block.instructions[i]) {
                        free_ast(node->data.block.instructions[i]);
                    }
                }
                free(node->data.block.instructions);
            }
            break;

        case NODE_FUNCTION:
            free(node->data.function.name);
            if (node->data.function.names) {
                for (int i = 0; i < node->data.function.count; i++) {
                    if (node->data.function.names[i]) {
                        free(node->data.function.names[i]);
                    }
                }
                free(node->data.function.names);
            }
            break;

        case NODE_CLASS:
            free(node->data.identifier_name);
            break;

        default:
            // NODE_NUMBER, NODE_BOOLEAN, NODE_NULL, NODE_BINARY_OP,
            // NODE_UNARY_OP, NODE_AST_TYPE_ANNOTATION, NODE_IF, NODE_WHILE,
            // NODE_RETURN, NODE_BREAK, NODE_PRINT, NODE_INPUT,
            // NODE_INDEX_ACCESS: pas de données allouées dynamiquement
            // dans l'union à libérer ici.
            break;
    }

    free(node);
}

// ===== FONCTIONS D'AFFICHAGE =====

void print_ast(ASTNode* node, int indent) {
    if (!node) {
        for (int i = 0; i < indent; i++) printf("  ");
        printf("(null)\n");
        return;
    }
    
    for (int i = 0; i < indent; i++) printf("  ");
    printf("%s", node_AST_type_to_string(node->type));
    
    // Afficher les détails selon le type
    switch (node->type) {
        case NODE_NUMBER:
            printf(": %g", node->data.number_value);
            break;
        case NODE_STRING:
            printf(": \"%s\"", node->data.string_value);
            break;
        case NODE_BOOLEAN:
            printf(": %s", node->data.bool_value ? "true" : "false");
            break;
        case NODE_IDENTIFIER:
        case NODE_DECLARATION:
        case NODE_ASSIGNMENT:
        case NODE_CLASS:
        case NODE_FIELD_ACCESS:
            printf(": %s", node->data.identifier_name);
            break;
        case NODE_FUNCTION:
            printf(": %s", node->data.function.name);
            break;
        case NODE_NEW_INSTANCE:
            printf(": %s", node->data.call.name);
            break;
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            printf(": %s", operator_to_string(node->data.operator_type));
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
            printf(" (%d instructions)", node->data.block.count);
            break;
        case NODE_LIST:
            printf(" (%d éléments)", node->data.list.count);
            break;
        case NODE_CALL:
            printf(" (%d arguments)", node->data.call.count);
            break;
        default:
            break;
    }
    printf("\n");
    
    // Afficher les enfants
    if (node->type == NODE_PROGRAM || node->type == NODE_BLOCK) {
        for (int i = 0; i < node->data.block.count; i++) {
            print_ast(node->data.block.instructions[i], indent + 1);
        }
    } else if (node->type == NODE_LIST) {
        for (int i = 0; i < node->data.list.count; i++) {
            print_ast(node->data.list.elements[i], indent + 1);
        }
    } else if (node->type == NODE_CALL) {
        print_ast(node->left, indent + 1);
        for (int i = 0; i < node->data.call.count; i++) {
            print_ast(node->data.call.args[i], indent + 1);
        }
    } else {
        if (node->left) print_ast(node->left, indent + 1);
        if (node->right) print_ast(node->right, indent + 1);
        if (node->extra) print_ast(node->extra, indent + 1);
    }
}