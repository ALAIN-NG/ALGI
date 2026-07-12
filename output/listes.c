// Code généré par le compilateur ALGI
// Fichier: examples/listes.c
// Date: Sat Jul 11 13:15:11 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

int main(void) {
  algi_print(algi_make_string("=== Création de listes ==="));
  AlgiList* _algi_tmp_0 = algi_list_create(4);
  algi_list_append(_algi_tmp_0, algi_make_string("pomme"));
  algi_list_append(_algi_tmp_0, algi_make_string("banane"));
  algi_list_append(_algi_tmp_0, algi_make_string("orange"));
  algi_list_append(_algi_tmp_0, algi_make_string("mangue"));
  AlgiValue* fruits = algi_make_list(_algi_tmp_0);
  algi_print(algi_add(algi_make_string("Fruits: "), fruits));
  AlgiList* _algi_tmp_1 = algi_list_create(5);
  algi_list_append(_algi_tmp_1, algi_make_int(10L));
  algi_list_append(_algi_tmp_1, algi_make_int(20L));
  algi_list_append(_algi_tmp_1, algi_make_int(30L));
  algi_list_append(_algi_tmp_1, algi_make_int(40L));
  algi_list_append(_algi_tmp_1, algi_make_int(50L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_1);
  algi_print(algi_add(algi_make_string("Nombres: "), nombres));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== Accès aux éléments ==="));
  algi_print(algi_add(algi_make_string("Premier fruit: "), algi_index_get(fruits, algi_make_int(0L))));
  algi_print(algi_add(algi_make_string("Deuxième fruit: "), algi_index_get(fruits, algi_make_int(1L))));
  algi_print(algi_add(algi_make_string("Dernier nombre: "), algi_index_get(nombres, algi_make_int(4L))));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== Modification ==="));
  algi_index_set(fruits, algi_make_int(1L), algi_make_string("ananas"));
  algi_print(algi_add(algi_make_string("Fruits modifiés: "), fruits));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== Ajout d'éléments ==="));
  AlgiList* _algi_tmp_2 = algi_list_create(4);
  AlgiValue* scores = algi_make_list(_algi_tmp_2);
  algi_print(algi_add(algi_make_string("Scores: "), scores));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== Parcours ==="));
  // Boucle 'pou' sur fruit
  AlgiValue* _algi_tmp_3_src = fruits;
  AlgiList* _algi_tmp_3 = (AlgiList*)(_algi_tmp_3_src->value.ptr_value);
  for (int _algi_tmp_4 = 0; _algi_tmp_4 < algi_list_size(_algi_tmp_3); _algi_tmp_4++) {
    AlgiValue* fruit = algi_list_get(_algi_tmp_3, _algi_tmp_4);
    algi_print(algi_add(algi_make_string("J'aime les "), fruit));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== Opérations ==="));
  AlgiList* _algi_tmp_5 = algi_list_create(6);
  algi_list_append(_algi_tmp_5, algi_make_int(5L));
  algi_list_append(_algi_tmp_5, algi_make_int(3L));
  algi_list_append(_algi_tmp_5, algi_make_int(8L));
  algi_list_append(_algi_tmp_5, algi_make_int(1L));
  algi_list_append(_algi_tmp_5, algi_make_int(9L));
  algi_list_append(_algi_tmp_5, algi_make_int(2L));
  AlgiValue* nombres2 = algi_make_list(_algi_tmp_5);
  algi_print(algi_add(algi_make_string("Avant tri: "), nombres2));
  return 0;
}
