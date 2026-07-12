// Code généré par le compilateur ALGI
// Fichier: examples/test.c
// Date: Sat Jul 11 12:46:09 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* carre(AlgiValue* x) {
  return algi_mul(x, x);
  return algi_make_null();
}

int main(void) {
  AlgiValue* age = algi_make_int(0L);
  AlgiValue* nom = algi_make_string("");
  algi_print(algi_make_string("Entrez votre nom :"));
  nom = algi_input_from_value(NULL);
  algi_print(algi_make_string("Entrez votre age :"));
  age = algi_input_from_value(NULL);
  AlgiValue* est_majeur = algi_cmp_ge(age, algi_make_int(18L));
  algi_print(algi_make_string("=== TEST ALGI ==="));
  algi_print(nom);
  algi_print(age);
  algi_print(est_majeur);
  if (algi_value_to_bool(est_majeur)) {
    algi_print(algi_make_string("Tu es majeur!"));
  }
  else {
    algi_print(algi_make_string("Tu es mineur!"));
  }
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, algi_make_int(5L)))) {
    algi_print(algi_add(algi_make_string("i = "), i));
    i = algi_add(i, algi_make_int(1L));
  }
  AlgiList* _algi_tmp_0 = algi_list_create(5);
  algi_list_append(_algi_tmp_0, algi_make_int(10L));
  algi_list_append(_algi_tmp_0, algi_make_int(20L));
  algi_list_append(_algi_tmp_0, algi_make_int(30L));
  algi_list_append(_algi_tmp_0, algi_make_int(40L));
  algi_list_append(_algi_tmp_0, algi_make_int(50L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_0);
  // Boucle 'pou' sur nb
  AlgiValue* _algi_tmp_1_src = nombres;
  AlgiList* _algi_tmp_1 = (AlgiList*)(_algi_tmp_1_src->value.ptr_value);
  for (int _algi_tmp_2 = 0; _algi_tmp_2 < algi_list_size(_algi_tmp_1); _algi_tmp_2++) {
    AlgiValue* nb = algi_list_get(_algi_tmp_1, _algi_tmp_2);
    algi_print(algi_add(algi_make_string("Nombre: "), nb));
  }
  AlgiValue* resultat = carre(algi_make_float(7.8));
  algi_print(algi_add(algi_make_string("7.8 au carre = "), resultat));
  algi_print(algi_make_string("=== FIN ==="));
  return 0;
}
