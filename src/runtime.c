// Implémentation du runtime pour le code C généré par ALGI

#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <setjmp.h>

// ===== GESTION DES EXCEPTIONS (avec setjmp/longjmp) =====
typedef struct {
    jmp_buf buffer;
    char* error_message;
    int active;
} ExceptionFrame;

static ExceptionFrame exception_stack[32];
static int exception_stack_top = -1;

void algi_push_try() {
    exception_stack_top++;
    exception_stack[exception_stack_top].active = 1;
    exception_stack[exception_stack_top].error_message = NULL;
}

void algi_pop_try() {
    if (exception_stack_top >= 0) {
        exception_stack[exception_stack_top].active = 0;
        if (exception_stack[exception_stack_top].error_message) {
            free(exception_stack[exception_stack_top].error_message);
        }
        exception_stack_top--;
    }
}

int algi_setjmp_try() {
    if (exception_stack_top < 0) return 0;
    return setjmp(exception_stack[exception_stack_top].buffer);
}

void algi_throw(const char* error_message) {
    if (exception_stack_top >= 0 && exception_stack[exception_stack_top].active) {
        exception_stack[exception_stack_top].error_message = strdup(error_message);
        longjmp(exception_stack[exception_stack_top].buffer, 1);
    } else {
        fprintf(stderr, "❌ Erreur ALGI non rattrapée: %s\n", error_message);
        exit(1);
    }
}

char* algi_get_last_error() {
    if (exception_stack_top >= 0) {
        return exception_stack[exception_stack_top].error_message;
    }
    return NULL;
}

// ===== FONCTIONS DE CRÉATION DE VALEURS =====

AlgiValue* algi_make_int(long value) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_INT;
    v->value.int_value = value;
    return v;
}

AlgiValue* algi_make_float(double value) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_FLOAT;
    v->value.float_value = value;
    return v;
}

AlgiValue* algi_make_bool(bool value) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_BOOL;
    v->value.bool_value = value;
    return v;
}

AlgiValue* algi_make_string(const char* value) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_STRING;
    v->value.string_value = value ? strdup(value) : NULL;
    return v;
}

AlgiValue* algi_make_null() {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_NULL;
    v->value.ptr_value = NULL;
    return v;
}

AlgiValue* algi_make_list(AlgiList* list) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_LIST;
    v->value.ptr_value = list;
    return v;
}

AlgiValue* algi_make_object(AlgiObject* obj) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_OBJECT;
    v->value.ptr_value = obj;
    return v;
}

AlgiValue* algi_make_error(const char* message) {
    AlgiValue* v = (AlgiValue*)malloc(sizeof(AlgiValue));
    if (!v) return NULL;
    v->type = TYPE_ERROR;
    v->value.string_value = message ? strdup(message) : NULL;
    return v;
}

// ===== FONCTIONS DE CONVERSION =====

char* algi_value_to_string(AlgiValue* value) {
    if (!value) return strdup("n");
    
    char buffer[256];
    switch (value->type) {
        case TYPE_INT:
            sprintf(buffer, "%ld", value->value.int_value);
            return strdup(buffer);
        case TYPE_FLOAT:
            sprintf(buffer, "%g", value->value.float_value);
            return strdup(buffer);
        case TYPE_BOOL:
            return strdup(value->value.bool_value ? "nene" : "pok");
        case TYPE_STRING:
            return value->value.string_value ? strdup(value->value.string_value) : strdup("");
        case TYPE_NULL:
            return strdup("n");
        case TYPE_LIST:
            return strdup("bi[...]");
        case TYPE_OBJECT:
            return strdup("kwouo {...}");
        case TYPE_ERROR:
            return value->value.string_value ? strdup(value->value.string_value) : strdup("erreur");
        default:
            return strdup("(inconnu)");
    }
}

long algi_value_to_int(AlgiValue* value) {
    if (!value) return 0;
    switch (value->type) {
        case TYPE_INT: return value->value.int_value;
        case TYPE_FLOAT: return (long)value->value.float_value;
        case TYPE_BOOL: return value->value.bool_value ? 1 : 0;
        case TYPE_STRING: return value->value.string_value ? atol(value->value.string_value) : 0;
        default: return 0;
    }
}

double algi_value_to_float(AlgiValue* value) {
    if (!value) return 0.0;
    switch (value->type) {
        case TYPE_INT: return (double)value->value.int_value;
        case TYPE_FLOAT: return value->value.float_value;
        case TYPE_BOOL: return value->value.bool_value ? 1.0 : 0.0;
        case TYPE_STRING: return value->value.string_value ? atof(value->value.string_value) : 0.0;
        default: return 0.0;
    }
}

bool algi_value_to_bool(AlgiValue* value) {
    if (!value) return false;
    switch (value->type) {
        case TYPE_BOOL: return value->value.bool_value;
        case TYPE_INT: return value->value.int_value != 0;
        case TYPE_FLOAT: return value->value.float_value != 0.0;
        case TYPE_STRING: return value->value.string_value && strlen(value->value.string_value) > 0;
        default: return false;
    }
}

AlgiType algi_value_type(AlgiValue* value) {
    return value ? value->type : TYPE_NULL;
}

void algi_value_free(AlgiValue* value) {
    if (!value) return;
    if (value->type == TYPE_STRING || value->type == TYPE_ERROR) {
        if (value->value.string_value) {
            free(value->value.string_value);
        }
    }
    free(value);
}

// ===== FONCTIONS NATIVES =====

void algi_print(AlgiValue* value) {
    if (!value) {
        printf("n\n");
        return;
    }
    char* str = algi_value_to_string(value);
    if (str) {
        printf("%s\n", str);
        free(str);
    }
}

AlgiValue* algi_input(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return algi_make_error("Erreur de lecture");
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    // BUG CORRIGE: l'entree utilisateur etait toujours renvoyee comme une
    // chaine (TYPE_STRING). Or l'operateur '+' (algi_add) concatene des
    // qu'un des deux operandes est une chaine : une calculatrice qui lit
    // deux nombres avec keksie() et fait ensuite `a + b` obtenait donc
    // "53" au lieu de 8 pour les entrees 5 et 3. On detecte maintenant si
    // le texte saisi est un entier ou un reel et on renvoie le type
    // numerique adequat, afin que +, -, *, / se comportent comme des
    // operations arithmetiques normales sur une saisie numerique. Si la
    // saisie n'est pas un nombre, on conserve le comportement d'origine
    // (chaine de caracteres), ce qui permet aussi de lire du texte (nom,
    // etc.) avec le meme mot-cle keksie.
    size_t len = strlen(buffer);
    if (len > 0) {
        char* endptr = NULL;

        long as_int = strtol(buffer, &endptr, 10);
        if (endptr && *endptr == '\0') {
            return algi_make_int(as_int);
        }

        double as_float = strtod(buffer, &endptr);
        if (endptr && *endptr == '\0') {
            return algi_make_float(as_float);
        }
    }

    return algi_make_string(buffer);
}

AlgiValue* algi_eval(const char* expr) {
    // Simplifié: retourne l'expression sous forme de chaîne
    return algi_make_string(expr);
}

AlgiValue* algi_sum(AlgiList* list) {
    if (!list) return algi_make_float(0.0);
    double sum = 0.0;
    for (int i = 0; i < list->length; i++) {
        sum += algi_value_to_float(list->items[i]);
    }
    return algi_make_float(sum);
}

AlgiValue* algi_subtract(AlgiValue* a, AlgiValue* b) {
    double val_a = algi_value_to_float(a);
    double val_b = algi_value_to_float(b);
    return algi_make_float(val_a - val_b);
}

AlgiValue* algi_equals(AlgiValue* a, AlgiValue* b) {
    if (!a && !b) return algi_make_bool(true);
    if (!a || !b) return algi_make_bool(false);
    
    if (a->type != b->type) return algi_make_bool(false);
    
    switch (a->type) {
        case TYPE_INT:
            return algi_make_bool(a->value.int_value == b->value.int_value);
        case TYPE_FLOAT:
            return algi_make_bool(a->value.float_value == b->value.float_value);
        case TYPE_BOOL:
            return algi_make_bool(a->value.bool_value == b->value.bool_value);
        case TYPE_STRING:
            if (!a->value.string_value && !b->value.string_value) return algi_make_bool(true);
            if (!a->value.string_value || !b->value.string_value) return algi_make_bool(false);
            return algi_make_bool(strcmp(a->value.string_value, b->value.string_value) == 0);
        default:
            return algi_make_bool(a->value.ptr_value == b->value.ptr_value);
    }
}

AlgiValue* algi_sort(AlgiList* list) {
    if (!list) return algi_make_list(NULL);
    
    // Tri à bulles simple
    for (int i = 0; i < list->length - 1; i++) {
        for (int j = 0; j < list->length - i - 1; j++) {
            AlgiValue* a = list->items[j];
            AlgiValue* b = list->items[j + 1];
            char* str_a = algi_value_to_string(a);
            char* str_b = algi_value_to_string(b);
            if (str_a && str_b && strcmp(str_a, str_b) > 0) {
                AlgiValue* temp = list->items[j];
                list->items[j] = list->items[j + 1];
                list->items[j + 1] = temp;
            }
            if (str_a) free(str_a);
            if (str_b) free(str_b);
        }
    }
    return algi_make_list(list);
}

AlgiValue* algi_typeof(AlgiValue* value) {
    if (!value) return algi_make_string("n");
    const char* type_name = "";
    switch (value->type) {
        case TYPE_INT: type_name = "saa"; break;
        case TYPE_FLOAT: type_name = "reyel"; break;
        case TYPE_BOOL: type_name = "pok/nene"; break;
        case TYPE_STRING: type_name = "djanghom"; break;
        case TYPE_NULL: type_name = "n"; break;
        case TYPE_LIST: type_name = "bi"; break;
        case TYPE_OBJECT: type_name = "kwouo"; break;
        default: type_name = "inconnu"; break;
    }
    return algi_make_string(type_name);
}

AlgiValue* algi_open(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        return algi_make_error("Impossible d'ouvrir le fichier");
    }
    fclose(file);
    return algi_make_int(0);
}

void algi_exit(AlgiValue* code) {
    int exit_code = code ? algi_value_to_int(code) : 0;
    exit(exit_code);
}

AlgiValue* algi_concat(AlgiValue* a, AlgiValue* b) {
    char* str_a = algi_value_to_string(a);
    char* str_b = algi_value_to_string(b);
    
    if (!str_a && !str_b) return algi_make_string("");
    if (!str_a) return algi_make_string(str_b);
    if (!str_b) return algi_make_string(str_a);
    
    char* result = (char*)malloc(strlen(str_a) + strlen(str_b) + 1);
    if (!result) {
        free(str_a);
        free(str_b);
        return algi_make_error("Erreur d'allocation mémoire");
    }
    strcpy(result, str_a);
    strcat(result, str_b);
    
    free(str_a);
    free(str_b);
    return algi_make_string(result);
}

// ===== FONCTIONS POUR LES LISTES =====

AlgiList* algi_list_create(int initial_capacity) {
    AlgiList* list = (AlgiList*)malloc(sizeof(AlgiList));
    if (!list) return NULL;
    list->capacity = initial_capacity > 0 ? initial_capacity : 10;
    list->length = 0;
    list->items = (AlgiValue**)malloc(list->capacity * sizeof(AlgiValue*));
    if (!list->items) {
        free(list);
        return NULL;
    }
    return list;
}

void algi_list_append(AlgiList* list, AlgiValue* item) {
    if (!list) return;
    if (list->length >= list->capacity) {
        list->capacity *= 2;
        list->items = (AlgiValue**)realloc(list->items, list->capacity * sizeof(AlgiValue*));
        if (!list->items) return;
    }
    list->items[list->length++] = item;
}

AlgiValue* algi_list_get(AlgiList* list, int index) {
    if (!list || index < 0 || index >= list->length) {
        algi_throw("Index hors limites");
        return algi_make_null();
    }
    return list->items[index];
}

void algi_list_set(AlgiList* list, int index, AlgiValue* value) {
    if (!list || index < 0 || index >= list->length) {
        algi_throw("Index hors limites");
        return;
    }
    list->items[index] = value;
}

int algi_list_size(AlgiList* list) {
    return list ? list->length : 0;
}

void algi_list_free(AlgiList* list) {
    if (!list) return;
    for (int i = 0; i < list->length; i++) {
        algi_value_free(list->items[i]);
    }
    free(list->items);
    free(list);
}

AlgiList* algi_list_copy(AlgiList* list) {
    if (!list) return NULL;
    AlgiList* copy = algi_list_create(list->capacity);
    if (!copy) return NULL;
    for (int i = 0; i < list->length; i++) {
        algi_list_append(copy, list->items[i]);
    }
    return copy;
}

// ===== FONCTIONS POUR LES OBJETS =====

AlgiObject* algi_object_create(const char* class_name) {
    AlgiObject* obj = (AlgiObject*)malloc(sizeof(AlgiObject));
    if (!obj) return NULL;
    obj->class_name = class_name ? strdup(class_name) : NULL;
    obj->field_count = 0;
    obj->capacity = 10;
    obj->fields = (AlgiValue**)malloc(obj->capacity * sizeof(AlgiValue*));
    if (!obj->fields) {
        if (obj->class_name) free(obj->class_name);
        free(obj);
        return NULL;
    }
    return obj;
}

void algi_object_add_field(AlgiObject* obj, const char* field_name, AlgiValue* value) {
    if (!obj) return;
    if (obj->field_count >= obj->capacity) {
        obj->capacity *= 2;
        obj->fields = (AlgiValue**)realloc(obj->fields, obj->capacity * sizeof(AlgiValue*));
        if (!obj->fields) return;
    }
    obj->fields[obj->field_count++] = value;
}

AlgiValue* algi_object_get_field(AlgiObject* obj, const char* field_name) {
    if (!obj || obj->field_count == 0) {
        return algi_make_null();
    }
    return obj->fields[0];  // Simplifié: retourne le premier champ
}

void algi_object_set_field(AlgiObject* obj, const char* field_name, AlgiValue* value) {
    if (!obj || obj->field_count == 0) return;
    obj->fields[0] = value;  // Simplifié: modifie le premier champ
}

// ===== GESTION DES ERREURS =====

void algi_try(void* (*func)(void*), void* data) {
    algi_push_try();
    if (algi_setjmp_try() == 0) {
        func(data);
    }
    algi_pop_try();
}

// ===== OPÉRATIONS ARITHMÉTIQUES / LOGIQUES SUR AlgiValue =====
// Ajoutées pour permettre au générateur de code C de rester cohérent avec
// le système de valeurs dynamiques AlgiValue (auparavant, le générateur de
// code émettait des types C bruts qui ne correspondaient à aucune fonction
// de ce runtime, rendant le code C généré non compilable ou incorrect).

AlgiValue* algi_add(AlgiValue* a, AlgiValue* b) {
    // Concaténation si l'un des deux opérandes est une chaîne
    if ((a && a->type == TYPE_STRING) || (b && b->type == TYPE_STRING)) {
        return algi_concat(a, b);
    }
    if ((a && a->type == TYPE_FLOAT) || (b && b->type == TYPE_FLOAT)) {
        return algi_make_float(algi_value_to_float(a) + algi_value_to_float(b));
    }
    return algi_make_int(algi_value_to_int(a) + algi_value_to_int(b));
}

AlgiValue* algi_sub(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_FLOAT) || (b && b->type == TYPE_FLOAT)) {
        return algi_make_float(algi_value_to_float(a) - algi_value_to_float(b));
    }
    return algi_make_int(algi_value_to_int(a) - algi_value_to_int(b));
}

AlgiValue* algi_mul(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_FLOAT) || (b && b->type == TYPE_FLOAT)) {
        return algi_make_float(algi_value_to_float(a) * algi_value_to_float(b));
    }
    return algi_make_int(algi_value_to_int(a) * algi_value_to_int(b));
}

AlgiValue* algi_div(AlgiValue* a, AlgiValue* b) {
    double denom = algi_value_to_float(b);
    if (denom == 0.0) {
        algi_throw("Division par zéro");
        return algi_make_null();
    }
    if ((a && a->type == TYPE_FLOAT) || (b && b->type == TYPE_FLOAT)) {
        return algi_make_float(algi_value_to_float(a) / denom);
    }
    long ib = algi_value_to_int(b);
    if (ib == 0) {
        algi_throw("Division par zéro");
        return algi_make_null();
    }
    return algi_make_int(algi_value_to_int(a) / ib);
}

AlgiValue* algi_mod(AlgiValue* a, AlgiValue* b) {
    long ib = algi_value_to_int(b);
    if (ib == 0) {
        algi_throw("Modulo par zéro");
        return algi_make_null();
    }
    return algi_make_int(algi_value_to_int(a) % ib);
}

AlgiValue* algi_not_equals(AlgiValue* a, AlgiValue* b) {
    AlgiValue* eq = algi_equals(a, b);
    bool result = !algi_value_to_bool(eq);
    algi_value_free(eq);
    return algi_make_bool(result);
}

AlgiValue* algi_cmp_lt(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_STRING) && (b && b->type == TYPE_STRING)) {
        return algi_make_bool(strcmp(a->value.string_value, b->value.string_value) < 0);
    }
    return algi_make_bool(algi_value_to_float(a) < algi_value_to_float(b));
}

AlgiValue* algi_cmp_gt(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_STRING) && (b && b->type == TYPE_STRING)) {
        return algi_make_bool(strcmp(a->value.string_value, b->value.string_value) > 0);
    }
    return algi_make_bool(algi_value_to_float(a) > algi_value_to_float(b));
}

AlgiValue* algi_cmp_le(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_STRING) && (b && b->type == TYPE_STRING)) {
        return algi_make_bool(strcmp(a->value.string_value, b->value.string_value) <= 0);
    }
    return algi_make_bool(algi_value_to_float(a) <= algi_value_to_float(b));
}

AlgiValue* algi_cmp_ge(AlgiValue* a, AlgiValue* b) {
    if ((a && a->type == TYPE_STRING) && (b && b->type == TYPE_STRING)) {
        return algi_make_bool(strcmp(a->value.string_value, b->value.string_value) >= 0);
    }
    return algi_make_bool(algi_value_to_float(a) >= algi_value_to_float(b));
}

AlgiValue* algi_logical_and(AlgiValue* a, AlgiValue* b) {
    return algi_make_bool(algi_value_to_bool(a) && algi_value_to_bool(b));
}

AlgiValue* algi_logical_or(AlgiValue* a, AlgiValue* b) {
    return algi_make_bool(algi_value_to_bool(a) || algi_value_to_bool(b));
}

AlgiValue* algi_is_type(AlgiValue* a, AlgiValue* b) {
    // BUG CORRIGÉ: cette fonction ne comparait auparavant que le TYPE
    // dynamique des deux valeurs (ex: TYPE_INT == TYPE_INT), ce qui rend
    // "i est 5" vrai dès que 'i' est un entier, quelle que soit sa valeur
    // réelle ! C'est un piège dangereux (une condition censée tester une
    // valeur précise devient presque toujours vraie). On aligne donc "est"
    // sur une véritable égalité de valeur (type ET contenu), cohérente
    // avec l'usage naturel attendu ("si i est 5").
    return algi_equals(a, b);
}

AlgiValue* algi_in_list(AlgiValue* a, AlgiValue* b) {
    // 'dans': vrai si 'a' est un élément de la liste 'b'
    if (!b || b->type != TYPE_LIST || !b->value.ptr_value) {
        return algi_make_bool(false);
    }
    AlgiList* list = (AlgiList*)b->value.ptr_value;
    for (int i = 0; i < list->length; i++) {
        AlgiValue* eq = algi_equals(a, list->items[i]);
        bool matched = algi_value_to_bool(eq);
        algi_value_free(eq);
        if (matched) return algi_make_bool(true);
    }
    return algi_make_bool(false);
}

AlgiValue* algi_negate(AlgiValue* a) {
    if (a && a->type == TYPE_FLOAT) {
        return algi_make_float(-algi_value_to_float(a));
    }
    return algi_make_int(-algi_value_to_int(a));
}

AlgiValue* algi_logical_not(AlgiValue* a) {
    return algi_make_bool(!algi_value_to_bool(a));
}

// ===== ACCÈS INDEXÉ / CHAMPS SUR AlgiValue =====

AlgiValue* algi_index_get(AlgiValue* list_value, AlgiValue* index_value) {
    if (!list_value || list_value->type != TYPE_LIST || !list_value->value.ptr_value) {
        algi_throw("Accès indexé sur une valeur non-liste");
        return algi_make_null();
    }
    int index = (int)algi_value_to_int(index_value);
    return algi_list_get((AlgiList*)list_value->value.ptr_value, index);
}

void algi_index_set(AlgiValue* list_value, AlgiValue* index_value, AlgiValue* new_value) {
    if (!list_value || list_value->type != TYPE_LIST || !list_value->value.ptr_value) {
        algi_throw("Affectation indexée sur une valeur non-liste");
        return;
    }
    int index = (int)algi_value_to_int(index_value);
    algi_list_set((AlgiList*)list_value->value.ptr_value, index, new_value);
}

AlgiValue* algi_field_get_value(AlgiValue* object_value, const char* field_name) {
    if (!object_value || object_value->type != TYPE_OBJECT || !object_value->value.ptr_value) {
        algi_throw("Accès à un champ sur une valeur non-objet");
        return algi_make_null();
    }
    return algi_object_get_field((AlgiObject*)object_value->value.ptr_value, field_name);
}

void algi_field_set_value(AlgiValue* object_value, const char* field_name, AlgiValue* new_value) {
    if (!object_value || object_value->type != TYPE_OBJECT || !object_value->value.ptr_value) {
        algi_throw("Affectation de champ sur une valeur non-objet");
        return;
    }
    algi_object_set_field((AlgiObject*)object_value->value.ptr_value, field_name, new_value);
}

// ===== ENTRÉE UTILISATEUR =====

AlgiValue* algi_input_from_value(AlgiValue* prompt_value) {
    char* prompt_str = prompt_value ? algi_value_to_string(prompt_value) : NULL;
    AlgiValue* result = algi_input(prompt_str);
    if (prompt_str) free(prompt_str);
    return result;
}