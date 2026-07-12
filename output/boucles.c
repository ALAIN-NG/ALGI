// Code généré par le compilateur ALGI
// Fichier: examples/boucles.c
// Date: Sat Jul 11 13:04:30 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

int main(void) {
  algi_print(algi_make_string("=== Boucle WHILE ==="));
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, algi_make_int(5L)))) {
    algi_print(algi_add(algi_make_string("i = "), i));
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_make_string("=== Boucle FOR avec liste ==="));
  AlgiList* _algi_tmp_0 = algi_list_create(10);
  algi_list_append(_algi_tmp_0, algi_make_int(1L));
  algi_list_append(_algi_tmp_0, algi_make_int(2L));
  algi_list_append(_algi_tmp_0, algi_make_int(3L));
  algi_list_append(_algi_tmp_0, algi_make_int(4L));
  algi_list_append(_algi_tmp_0, algi_make_int(5L));
  algi_list_append(_algi_tmp_0, algi_make_int(6L));
  algi_list_append(_algi_tmp_0, algi_make_int(7L));
  algi_list_append(_algi_tmp_0, algi_make_int(8L));
  algi_list_append(_algi_tmp_0, algi_make_int(9L));
  algi_list_append(_algi_tmp_0, algi_make_int(10L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_0);
  algi_print(algi_index_get(nombres, algi_make_int(2L)));
  AlgiValue* somme = algi_make_int(0L);
  // Boucle 'pou' sur nb
  AlgiValue* _algi_tmp_1_src = nombres;
  AlgiList* _algi_tmp_1 = (AlgiList*)(_algi_tmp_1_src->value.ptr_value);
  for (int _algi_tmp_2 = 0; _algi_tmp_2 < algi_list_size(_algi_tmp_1); _algi_tmp_2++) {
    AlgiValue* nb = algi_list_get(_algi_tmp_1, _algi_tmp_2);
    somme = algi_add(somme, nb);
    algi_print(algi_add(algi_add(algi_add(algi_make_string("Nombre: "), nb), algi_make_string(", Somme partielle: ")), somme));
  }
  algi_print(algi_add(algi_make_string("Somme totale: "), somme));
  algi_print(algi_make_string("=== Boucle FOR sur caracteres ==="));
  AlgiValue* mot = algi_make_string("ALGI");
  algi_print(mot);
  // Boucle 'pou' sur c
  AlgiValue* _algi_tmp_3_src = mot;
  AlgiList* _algi_tmp_3 = (AlgiList*)(_algi_tmp_3_src->value.ptr_value);
  for (int _algi_tmp_4 = 0; _algi_tmp_4 < algi_list_size(_algi_tmp_3); _algi_tmp_4++) {
    AlgiValue* c = algi_list_get(_algi_tmp_3, _algi_tmp_4);
    algi_print(algi_add(algi_make_string("Caractere: "), c));
  }
  return 0;
}
