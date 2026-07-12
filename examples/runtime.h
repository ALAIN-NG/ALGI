// Runtime pour le code C généré par ALGI

#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>

// ===== TYPES DE VALEURS ALGI =====
typedef enum {
    TYPE_INT = 1,
    TYPE_FLOAT = 2,
    TYPE_STRING = 3,
    TYPE_BOOL = 4,
    TYPE_LIST = 5,
    TYPE_OBJECT = 6,
    TYPE_VOID = 7,
    TYPE_NULL = 8,
    TYPE_ERROR = 9
} AlgiType;

// ===== STRUCTURE D'UNE VALEUR ALGI =====
typedef struct {
    AlgiType type;
    union {
        long int_value;
        double float_value;
        bool bool_value;
        char* string_value;
        void* ptr_value;
    } value;
} AlgiValue;

// ===== STRUCTURE D'UNE LISTE =====
typedef struct {
    AlgiValue** items;
    int length;
    int capacity;
} AlgiList;

// ===== STRUCTURE D'UN OBJET =====
typedef struct {
    char* class_name;
    AlgiValue** fields;
    int field_count;
    int capacity;
} AlgiObject;

// ===== FONCTIONS DE CRÉATION DE VALEURS =====
AlgiValue* algi_make_int(long value);
AlgiValue* algi_make_float(double value);
AlgiValue* algi_make_bool(bool value);
AlgiValue* algi_make_string(const char* value);
AlgiValue* algi_make_null();
AlgiValue* algi_make_list(AlgiList* list);
AlgiValue* algi_make_object(AlgiObject* obj);
AlgiValue* algi_make_error(const char* message);

// ===== FONCTIONS DE CONVERSION =====
char* algi_value_to_string(AlgiValue* value);
long algi_value_to_int(AlgiValue* value);
double algi_value_to_float(AlgiValue* value);
bool algi_value_to_bool(AlgiValue* value);
AlgiType algi_value_type(AlgiValue* value);
void algi_value_free(AlgiValue* value);

// ===== FONCTIONS NATIVES =====
void algi_print(AlgiValue* value);
AlgiValue* algi_input(const char* prompt);
AlgiValue* algi_eval(const char* expr);
AlgiValue* algi_sum(AlgiList* list);
AlgiValue* algi_subtract(AlgiValue* a, AlgiValue* b);
AlgiValue* algi_equals(AlgiValue* a, AlgiValue* b);
AlgiValue* algi_sort(AlgiList* list);
AlgiValue* algi_typeof(AlgiValue* value);
AlgiValue* algi_open(const char* path);
void algi_exit(AlgiValue* code);
AlgiValue* algi_concat(AlgiValue* a, AlgiValue* b);

// ===== FONCTIONS POUR LES LISTES =====
AlgiList* algi_list_create(int initial_capacity);
void algi_list_append(AlgiList* list, AlgiValue* item);
AlgiValue* algi_list_get(AlgiList* list, int index);
void algi_list_set(AlgiList* list, int index, AlgiValue* value);
int algi_list_size(AlgiList* list);
void algi_list_free(AlgiList* list);
AlgiList* algi_list_copy(AlgiList* list);

// ===== FONCTIONS POUR LES OBJETS =====
AlgiObject* algi_object_create(const char* class_name);
void algi_object_add_field(AlgiObject* obj, const char* field_name, AlgiValue* value);
AlgiValue* algi_object_get_field(AlgiObject* obj, const char* field_name);
void algi_object_set_field(AlgiObject* obj, const char* field_name, AlgiValue* value);

// ===== GESTION DES ERREURS =====
void algi_throw(const char* error_message);
void algi_try(void* (*func)(void*), void* data);
void algi_catch(void (*handler)(const char*));

// ===== GESTION DES EXCEPTIONS (utilisées par le code généré pour tela/tatem) =====
// NOTE: ces fonctions étaient implémentées dans runtime.c mais jamais déclarées
// ici, ce qui provoquait des déclarations implicites (et donc des bugs graves,
// notamment sur char* algi_get_last_error() qui aurait été tronqué en int).
void algi_push_try(void);
void algi_pop_try(void);
int algi_setjmp_try(void);
char* algi_get_last_error(void);

// ===== OPÉRATIONS ARITHMÉTIQUES / LOGIQUES SUR AlgiValue =====
// Utilisées par le code C généré pour les opérateurs binaires et unaires ALGI.
AlgiValue* algi_add(AlgiValue* a, AlgiValue* b);         // +  (concatène si l'un des deux est une chaîne)
AlgiValue* algi_sub(AlgiValue* a, AlgiValue* b);         // -
AlgiValue* algi_mul(AlgiValue* a, AlgiValue* b);         // *
AlgiValue* algi_div(AlgiValue* a, AlgiValue* b);         // /
AlgiValue* algi_mod(AlgiValue* a, AlgiValue* b);         // %
AlgiValue* algi_not_equals(AlgiValue* a, AlgiValue* b);  // !=
AlgiValue* algi_cmp_lt(AlgiValue* a, AlgiValue* b);      // <
AlgiValue* algi_cmp_gt(AlgiValue* a, AlgiValue* b);      // >
AlgiValue* algi_cmp_le(AlgiValue* a, AlgiValue* b);      // <=
AlgiValue* algi_cmp_ge(AlgiValue* a, AlgiValue* b);      // >=
AlgiValue* algi_logical_and(AlgiValue* a, AlgiValue* b); // e
AlgiValue* algi_logical_or(AlgiValue* a, AlgiValue* b);  // ou
AlgiValue* algi_is_type(AlgiValue* a, AlgiValue* b);     // est
AlgiValue* algi_in_list(AlgiValue* a, AlgiValue* b);     // dans
AlgiValue* algi_negate(AlgiValue* a);                    // - unaire
AlgiValue* algi_logical_not(AlgiValue* a);               // poke / !

// ===== ACCÈS INDEXÉ / CHAMPS SUR AlgiValue (listes et objets) =====
AlgiValue* algi_index_get(AlgiValue* list_value, AlgiValue* index_value);
void algi_index_set(AlgiValue* list_value, AlgiValue* index_value, AlgiValue* new_value);
AlgiValue* algi_field_get_value(AlgiValue* object_value, const char* field_name);
void algi_field_set_value(AlgiValue* object_value, const char* field_name, AlgiValue* new_value);

// ===== ENTRÉE UTILISATEUR =====
AlgiValue* algi_input_from_value(AlgiValue* prompt_value);

#endif // RUNTIME_H