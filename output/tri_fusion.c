// Code généré par le compilateur ALGI
// Fichier: examples/tri_fusion.c
// Date: Sun Jul 12 17:00:37 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* fusionner(AlgiValue* tab, AlgiValue* gauche, AlgiValue* milieu, AlgiValue* droit) {
  AlgiValue* i = gauche;
  AlgiValue* j = algi_add(milieu, algi_make_int(1L));
  AlgiValue* k = algi_make_int(0L);
  AlgiValue* capacite = algi_add(algi_sub(droit, gauche), algi_make_int(1L));
  AlgiList* _algi_tmp_0 = algi_list_create(20);
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  AlgiValue* temp = algi_make_list(_algi_tmp_0);
  while (algi_value_to_bool(algi_logical_and(algi_cmp_le(i, milieu), algi_cmp_le(j, droit)))) {
    if (algi_value_to_bool(algi_cmp_le(algi_index_get(tab, i), algi_index_get(tab, j)))) {
      algi_index_set(temp, k, algi_index_get(tab, i));
      i = algi_add(i, algi_make_int(1L));
    }
    else {
      algi_index_set(temp, k, algi_index_get(tab, j));
      j = algi_add(j, algi_make_int(1L));
    }
    k = algi_add(k, algi_make_int(1L));
  }
  while (algi_value_to_bool(algi_cmp_le(i, milieu))) {
    algi_index_set(temp, k, algi_index_get(tab, i));
    i = algi_add(i, algi_make_int(1L));
    k = algi_add(k, algi_make_int(1L));
  }
  while (algi_value_to_bool(algi_cmp_le(j, droit))) {
    algi_index_set(temp, k, algi_index_get(tab, j));
    j = algi_add(j, algi_make_int(1L));
    k = algi_add(k, algi_make_int(1L));
  }
  AlgiValue* idx = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(idx, capacite))) {
    algi_index_set(tab, algi_add(gauche, idx), algi_index_get(temp, idx));
    idx = algi_add(idx, algi_make_int(1L));
  }
  return algi_make_bool(true);
  return algi_make_null();
}

AlgiValue* tri_fusion_rec(AlgiValue* tab, AlgiValue* gauche, AlgiValue* droit) {
  AlgiValue* a = algi_make_int(0L);
  AlgiValue* b = algi_make_int(0L);
  if (algi_value_to_bool(algi_cmp_lt(gauche, droit))) {
    AlgiValue* milieu = algi_div(algi_add(gauche, droit), algi_make_int(2L));
    a = tri_fusion_rec(tab, gauche, milieu);
    AlgiValue* mil = algi_add(milieu, algi_make_int(1L));
    a = tri_fusion_rec(tab, mil, droit);
    b = fusionner(tab, gauche, milieu, droit);
  }
  return algi_make_bool(true);
  return algi_make_null();
}

AlgiValue* tri_fusion(AlgiValue* tab, AlgiValue* taille) {
  AlgiValue* a = tri_fusion_rec(tab, algi_make_int(0L), algi_sub(taille, algi_make_int(1L)));
  return algi_make_bool(true);
  return algi_make_null();
}

AlgiValue* afficher_tableau(AlgiValue* tab, AlgiValue* taille) {
  AlgiValue* idx = algi_make_int(0L);
  algi_print(algi_add(algi_make_string("Tableau: ["), algi_make_string("")));
  while (algi_value_to_bool(algi_cmp_lt(idx, taille))) {
    if (algi_value_to_bool(algi_cmp_lt(idx, algi_sub(taille, algi_make_int(1L))))) {
      algi_print(algi_add(algi_add(algi_index_get(tab, idx), algi_make_string(", ")), algi_make_string("")));
    }
    else {
      algi_print(algi_add(algi_index_get(tab, idx), algi_make_string("")));
    }
    idx = algi_add(idx, algi_make_int(1L));
  }
  algi_print(algi_make_string("]"));
  return algi_make_bool(true);
  return algi_make_null();
}

int main(void) {
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   TRI FUSION (MERGE SORT)"));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string(""));
  AlgiValue* taillemax = algi_make_int(20L);
  algi_print(algi_add(algi_add(algi_make_string("Combien de nombres voulez-vous trier ? (max "), taillemax), algi_make_string(")")));
  AlgiValue* taille = algi_make_int(0L);
  taille = algi_input_from_value(NULL);
  if (algi_value_to_bool(algi_cmp_gt(taille, taillemax))) {
    algi_print(algi_add(algi_make_string("Attention: la capacite maximale est de "), taillemax));
    taille = taillemax;
  }
  AlgiList* _algi_tmp_1 = algi_list_create(20);
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  algi_list_append(_algi_tmp_1, algi_make_int(0L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_1);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("Entrez les nombres a trier:"));
  AlgiValue* i = algi_make_int(0L);
  AlgiValue* val = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, taille))) {
    algi_print(algi_add(algi_add(algi_make_string("Nombre "), algi_add(i, algi_make_int(1L))), algi_make_string(":")));
    val = algi_input_from_value(NULL);
    algi_index_set(nombres, i, val);
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("--- Avant le tri ---"));
  AlgiValue* bn = afficher_tableau(nombres, taille);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("Tri en cours..."));
  AlgiValue* ad = tri_fusion(nombres, taille);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("--- Apres le tri (ordre croissant) ---"));
  AlgiValue* at = afficher_tableau(nombres, taille);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   TRI FUSION TERMINE"));
  algi_print(algi_make_string("=========================================="));
  return 0;
}
