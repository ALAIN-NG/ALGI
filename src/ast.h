// Définition de l'Arbre Syntaxique Abstrait (AST) pour ALGI

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// ===== TYPES DE NŒUDS AST =====
typedef enum {
    // Nœuds de base
    NODE_PROGRAM,
    NODE_BLOCK,
    
    // Déclarations et assignations
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    
    // Structures de contrôle
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    
    // Fonctions et classes
    NODE_FUNCTION,
    NODE_CLASS,
    NODE_RETURN,
    NODE_BREAK,
    NODE_NEW_INSTANCE,
    
    // Gestion d'erreurs
    NODE_TRY_CATCH,
    
    // Fonctions natives
    NODE_PRINT,
    NODE_INPUT,
    
    // Opérateurs
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    
    // Littéraux
    NODE_NUMBER,
    NODE_STRING,
    NODE_BOOLEAN,
    NODE_NULL,
    NODE_IDENTIFIER,
    NODE_AST_TYPE_ANNOTATION,
    
    // Appels et accès
    NODE_CALL,
    NODE_LIST,
    NODE_INDEX_ACCESS,
    NODE_FIELD_ACCESS,
    
    // Spécial
    NODE_ERROR
} NodeType;

// ===== OPÉRATEURS =====
typedef enum {
    OP_ADD = '+',
    OP_SUB = '-',
    OP_MUL = '*',
    OP_DIV = '/',
    OP_MOD = '%',
    OP_EQ = '=',
    OP_NE = '!',
    OP_LT = '<',
    OP_GT = '>',
    AST_OP_LE = 'l',
    AST_OP_GE = 'g',
    OP_AND = '&',
    OP_OR = '|',
    OP_NOT = '~',
    OP_IS = 'i',
    OP_IN = 'n',
    OP_NEG = 1000000
} OperatorType;

// ===== TYPES DE SYMBOLES =====
typedef enum {
    AST_TYPE_UNKNOWN = 0,
    AST_TYPE_INT = 1,
    AST_TYPE_FLOAT = 2,
    AST_TYPE_STRING = 3,
    AST_TYPE_BOOL = 4,
    AST_TYPE_LIST = 5,
    AST_TYPE_OBJECT = 6,
    AST_TYPE_VOID = 7
} DataType;

// ===== STRUCTURE D'UN NŒUD AST =====
typedef struct ASTNode {
    NodeType type;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* extra; // 3e enfant : utilisé pour le bloc 'else' (kommenu) de NODE_IF
    DataType data_type;
    int line;
    int column;
    
    union {
        double number_value;
        char *string_value;
        int bool_value;
        char *identifier_name;
        OperatorType operator_type;

        struct {
            struct ASTNode **elements;
            int count;
        } list;

        struct {
            // 'name' est utilisé uniquement par NODE_NEW_INSTANCE (nom de
            // la classe instanciée). Pour NODE_CALL, le nom de la fonction
            // appelée est stocké via node->left (pas ici) et 'name' reste
            // inutilisé (NULL).
            // BUG CORRIGÉ: avant l'ajout de ce champ, NODE_NEW_INSTANCE
            // écrivait le nom de la classe dans node->data.identifier_name
            // PUIS écrasait cette même mémoire (union!) en écrivant
            // node->data.call.args juste après, perdant le nom de la
            // classe instanciée.
            char *name;
            struct ASTNode **args;
            int count;
        } call;

        struct {
            // BUG CORRIGÉ: avant l'ajout de ce champ 'name', NODE_FUNCTION
            // écrivait le nom de la fonction dans node->data.identifier_name
            // PUIS écrasait cette même mémoire (union!) en écrivant
            // node->data.function.names juste après, perdant totalement
            // le nom de la fonction (et provoquant des lectures de
            // mémoire invalides / noms de fonctions corrompus).
            char *name;
            char **names;
            int count;
        } function;

        struct {
            struct ASTNode **instructions;
            int count;
        } block;

        DataType AST_type_value;
    } data;
} ASTNode;

// ===== FONCTIONS DE CRÉATION DE NŒUDS =====
ASTNode* create_node(NodeType type);
ASTNode* create_program();
ASTNode* create_block(ASTNode** instructions);
ASTNode* create_declaration(char* name, ASTNode* expr);
ASTNode* create_assignment(char* name, ASTNode* expr);
ASTNode* create_if(ASTNode* condition, ASTNode* then_block, ASTNode* else_block);
ASTNode* create_while(ASTNode* condition, ASTNode* body);
ASTNode* create_for(char* var_name, ASTNode* iterable, ASTNode* body);
ASTNode* create_function(char* name, char** params, int param_count, ASTNode* body);
ASTNode* create_class(char* name, ASTNode* body);
ASTNode* create_return(ASTNode* expr);
ASTNode* create_break();
ASTNode* create_new_instance(char* class_name, ASTNode** args);
ASTNode* create_try_catch(ASTNode* try_block, char* catch_var, ASTNode* catch_block);
ASTNode* create_print(ASTNode* expr);
ASTNode* create_input(ASTNode* expr);
ASTNode* create_binary_op(OperatorType op, ASTNode* left, ASTNode* right);
ASTNode* create_unary_op(OperatorType op, ASTNode* expr);
ASTNode* create_number(double value);
ASTNode* create_string(char* value);
ASTNode* create_boolean(int value);
ASTNode* create_null();
ASTNode* create_identifier(char* name);
ASTNode* create_AST_type_annotation(DataType type);
ASTNode* create_call(ASTNode* callee, ASTNode** args, int arg_count);
ASTNode* create_list(ASTNode** elements, int count);
ASTNode* create_index_access(ASTNode* expr, ASTNode* index);
ASTNode* create_field_access(ASTNode* expr, char* field);
ASTNode* create_error(char* message);

// ===== FONCTIONS DE GESTION DE L'AST =====
void add_instruction(ASTNode* program, ASTNode* instr);
void add_argument(ASTNode* call, ASTNode* arg);
void add_list_element(ASTNode* list, ASTNode* element);
void add_parameter(ASTNode* function, char* param);
void add_field(ASTNode* class_def, char* field_name, DataType type);

// ===== FONCTIONS D'UTILITÉ =====
const char* node_AST_type_to_string(NodeType type);
const char* data_AST_type_to_string(DataType type);
const char* operator_to_string(OperatorType op);

// ===== FONCTIONS DE LIBÉRATION =====
void free_ast(ASTNode* node);

// ===== FONCTIONS D'AFFICHAGE =====
void print_ast(ASTNode* node, int indent);

#endif // AST_H