// Code généré par le compilateur ALGI
// Fichier: examples/alain.c
// Date: Sun Jul 12 14:41:26 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* factorielle(AlgiValue* nb) {
  if (algi_value_to_bool(algi_cmp_le(nb, algi_make_int(1L)))) {
    return algi_make_int(1L);
  }
  else {
    return algi_mul(nb, factorielle(algi_sub(nb, algi_make_int(1L))));
  }
  return algi_make_null();
}

AlgiValue* fibonacci(AlgiValue* nb) {
  if (algi_value_to_bool(algi_cmp_le(nb, algi_make_int(1L)))) {
    return nb;
  }
  else {
    return algi_add(fibonacci(algi_sub(nb, algi_make_int(1L))), fibonacci(algi_sub(nb, algi_make_int(2L))));
  }
  return algi_make_null();
}

int main(void) {
  algi_print(algi_make_string("========================================"));
  algi_print(algi_make_string("  PROGRAMME DE DÉMONSTRATION ALGI"));
  algi_print(algi_make_string("========================================"));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 1. Variables et types ==="));
  AlgiValue* nom = algi_make_string("");
  algi_print(algi_make_string("Entrez votre nom : "));
  nom = algi_input_from_value(NULL);
  AlgiValue* age = algi_make_int(0L);
  algi_print(algi_make_string("Entrez votre age : "));
  age = algi_input_from_value(NULL);
  AlgiValue* taille = algi_make_int(0L);
  algi_print(algi_make_string("Entrez votre taille (en mètres) : "));
  taille = algi_input_from_value(NULL);
  algi_print(algi_make_string(""));
  AlgiValue* est_majeur = algi_make_bool(true);
  algi_print(algi_add(algi_make_string("Nom: "), nom));
  algi_print(algi_add(algi_make_string("Âge: "), age));
  algi_print(algi_add(algi_make_string("Taille: "), taille));
  algi_print(algi_add(algi_make_string("Majeur: "), est_majeur));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 2. Calculs arithmétiques ==="));
  AlgiValue* a = algi_make_int(0L);
  AlgiValue* b = algi_make_int(0L);
  algi_print(algi_make_string("Entrez le premier nombre : "));
  a = algi_input_from_value(NULL);
  algi_print(algi_make_string("Entrez le second nombre : "));
  b = algi_input_from_value(NULL);
  algi_print(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" + ")), b), algi_make_string(" = ")), algi_add(a, b)));
  algi_print(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" - ")), b), algi_make_string(" = ")), algi_sub(a, b)));
  algi_print(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" * ")), b), algi_make_string(" = ")), algi_mul(a, b)));
  algi_print(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" / ")), b), algi_make_string(" = ")), algi_div(a, b)));
  algi_print(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" % ")), b), algi_make_string(" = ")), algi_mod(a, b)));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 3. Structures conditionnelles ==="));
  AlgiValue* note = algi_make_int(82L);
  if (algi_value_to_bool(algi_cmp_ge(note, algi_make_int(90L)))) {
    algi_print(algi_make_string("Excellent! "));
  }
  if (algi_value_to_bool(algi_logical_and(algi_cmp_ge(note, algi_make_int(80L)), algi_cmp_lt(note, algi_make_int(90L))))) {
    algi_print(algi_make_string("Très bien! "));
  }
  if (algi_value_to_bool(algi_logical_and(algi_cmp_ge(note, algi_make_int(70L)), algi_cmp_lt(note, algi_make_int(80L))))) {
    algi_print(algi_make_string("Bien! "));
  }
  if (algi_value_to_bool(algi_logical_and(algi_cmp_ge(note, algi_make_int(60L)), algi_cmp_lt(note, algi_make_int(70L))))) {
    algi_print(algi_make_string("Passable! "));
  }
  if (algi_value_to_bool(algi_cmp_lt(note, algi_make_int(60L)))) {
    algi_print(algi_make_string("À améliorer! "));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 4. Boucles ==="));
  algi_print(algi_make_string("Boucle 'w' (while):"));
  AlgiValue* i = algi_make_int(1L);
  while (algi_value_to_bool(algi_cmp_le(i, algi_make_int(5L)))) {
    algi_print(algi_add(algi_make_string("  i = "), i));
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_make_string("Boucle 'pou' (for):"));
  AlgiList* _algi_tmp_0 = algi_list_create(5);
  algi_list_append(_algi_tmp_0, algi_make_int(10L));
  algi_list_append(_algi_tmp_0, algi_make_int(20L));
  algi_list_append(_algi_tmp_0, algi_make_int(30L));
  algi_list_append(_algi_tmp_0, algi_make_int(40L));
  algi_list_append(_algi_tmp_0, algi_make_int(50L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_0);
  // Boucle 'pou' sur val
  AlgiValue* _algi_tmp_1_src = nombres;
  AlgiList* _algi_tmp_1 = (AlgiList*)(_algi_tmp_1_src->value.ptr_value);
  for (int _algi_tmp_2 = 0; _algi_tmp_2 < algi_list_size(_algi_tmp_1); _algi_tmp_2++) {
    AlgiValue* val = algi_list_get(_algi_tmp_1, _algi_tmp_2);
    algi_print(algi_add(algi_make_string("  val = "), val));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 5. Fonctions ==="));
  AlgiValue* ab = algi_make_int(0L);
  algi_print(algi_make_string("Entrez un nombre pour calculer sa factorielle : "));
  ab = algi_input_from_value(NULL);
  algi_print(algi_add(algi_add(algi_add(algi_make_string("Factorielle de "), ab), algi_make_string(" = ")), factorielle(ab)));
  algi_print(algi_make_string("Entrez un nombre pour calculer le terme correspondant de la suite de Fibonacci : "));
  ab = algi_input_from_value(NULL);
  algi_print(algi_add(algi_add(algi_add(algi_make_string("Fibonacci("), ab), algi_make_string(") = ")), fibonacci(ab)));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 6. Manipulation de listes ==="));
  AlgiList* _algi_tmp_3 = algi_list_create(3);
  algi_list_append(_algi_tmp_3, algi_make_string("pomme"));
  algi_list_append(_algi_tmp_3, algi_make_string("banane"));
  algi_list_append(_algi_tmp_3, algi_make_string("orange"));
  AlgiValue* fruits = algi_make_list(_algi_tmp_3);
  algi_print(algi_add(algi_make_string("Liste initiale: "), fruits));
  algi_print(algi_make_string("Parcours de la liste:"));
  // Boucle 'pou' sur fruit
  AlgiValue* _algi_tmp_4_src = fruits;
  AlgiList* _algi_tmp_4 = (AlgiList*)(_algi_tmp_4_src->value.ptr_value);
  for (int _algi_tmp_5 = 0; _algi_tmp_5 < algi_list_size(_algi_tmp_4); _algi_tmp_5++) {
    AlgiValue* fruit = algi_list_get(_algi_tmp_4, _algi_tmp_5);
    algi_print(algi_add(algi_make_string("  - "), fruit));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 7. Gestion d'erreurs ==="));
  algi_push_try();
  if (algi_setjmp_try() == 0) {
    algi_print(algi_make_string("hi"));
    AlgiValue* division = algi_div(algi_make_int(10L), algi_make_int(2L));
    algi_print(algi_add(algi_make_string("Résultat: "), division));
  } else {
    AlgiValue* erreur = algi_make_string(algi_get_last_error());
    algi_print(algi_add(algi_make_string("Erreur: "), erreur));
  }
  algi_pop_try();
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 8. Opérateurs logiques ==="));
  AlgiValue* x = algi_make_int(5L);
  AlgiValue* y = algi_make_int(10L);
  AlgiValue* z = algi_make_int(7L);
  algi_print(algi_add(algi_make_string("x < y et y > z: "), algi_logical_and(algi_cmp_lt(x, y), algi_cmp_gt(y, z))));
  algi_print(algi_add(algi_make_string("x > y ou z > y: "), algi_logical_or(algi_cmp_gt(x, y), algi_cmp_gt(z, y))));
  algi_print(algi_add(algi_make_string("poke (x est y): "), algi_logical_not(algi_is_type(x, y))));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=== 9. Opérateur 'est' ==="));
  AlgiValue* valeur = algi_make_int(42L);
  algi_print(algi_add(algi_make_string("valeur est 42? "), algi_is_type(valeur, algi_make_int(42L))));
  algi_print(algi_add(algi_make_string("valeur est 50? "), algi_is_type(valeur, algi_make_int(50L))));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("========================================"));
  algi_print(algi_make_string("  FIN DU PROGRAMME"));
  algi_print(algi_make_string("  Auteur: Alain"));
  algi_print(algi_make_string("  Merci d'avoir testé ALGI! 🎉"));
  algi_print(algi_make_string("========================================"));
  return 0;
}
