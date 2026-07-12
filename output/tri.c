// Code généré par le compilateur ALGI
// Fichier: examples/tri.c
// Date: Sun Jul 12 14:23:54 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* tri_selection(AlgiValue* liste, AlgiValue* nd) {
  AlgiValue* nb = liste;
  AlgiValue* taille = nb;
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, algi_sub(nd, algi_make_int(1L))))) {
    AlgiValue* min_index = i;
    AlgiValue* j = algi_add(i, algi_make_int(1L));
    while (algi_value_to_bool(algi_cmp_lt(j, nd))) {
      if (algi_value_to_bool(algi_cmp_lt(algi_index_get(nb, j), algi_index_get(nb, min_index)))) {
        min_index = j;
      }
      j = algi_add(j, algi_make_int(1L));
    }
    if (algi_value_to_bool(algi_not_equals(min_index, i))) {
      AlgiValue* temp = algi_index_get(nb, i);
      algi_index_set(nb, i, algi_index_get(nb, min_index));
      algi_index_set(nb, min_index, temp);
    }
    i = algi_add(i, algi_make_int(1L));
  }
  return nb;
  return algi_make_null();
}

AlgiValue* afficher_liste(AlgiValue* liste, AlgiValue* nd) {
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, nd))) {
    algi_print(algi_index_get(liste, i));
    i = algi_add(i, algi_make_int(1L));
  }
  return algi_make_int(0L);
  return algi_make_null();
}

int main(void) {
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   TRI PAR SÉLECTION (SELECTION SORT)"));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string(""));
  AlgiValue* nb = algi_make_int(0L);
  algi_print(algi_make_string("Entrez le nombre d'éléments à trier:"));
  nb = algi_input_from_value(NULL);
  if (algi_value_to_bool(algi_cmp_le(nb, algi_make_int(0L)))) {
    algi_print(algi_make_string("❌ Erreur: Le nombre d'éléments doit être positif!"));
  }
  if (algi_value_to_bool(algi_cmp_gt(nb, algi_make_int(100L)))) {
    algi_print(algi_add(algi_add(algi_make_string("⚠️ Attention: Vous allez entrer "), nb), algi_make_string(" nombres")));
    algi_print(algi_make_string("   (limite recommandée: 100)"));
  }
  AlgiList* _algi_tmp_0 = algi_list_create(27);
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
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  algi_list_append(_algi_tmp_0, algi_make_int(0L));
  AlgiValue* nombres = algi_make_list(_algi_tmp_0);
  algi_print(algi_make_string(""));
  algi_print(algi_add(algi_add(algi_make_string("Entrez "), nb), algi_make_string(" nombres:")));
  AlgiValue* i = algi_make_int(0L);
  AlgiValue* valeur = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_le(i, nb))) {
    algi_print(algi_add(algi_add(algi_make_string("Nombre "), i), algi_make_string(":")));
    valeur = algi_input_from_value(NULL);
    algi_index_set(nombres, i, valeur);
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_index_get(nombres, algi_make_int(0L)));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("📋 Liste non triée:"));
  AlgiValue* sd = afficher_liste(nombres, nb);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("🔄 Tri en cours..."));
  AlgiValue* liste_triee = tri_selection(nombres, nb);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("✅ Liste triée (ordre croissant):"));
  sd = afficher_liste(liste_triee, nb);
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("📊 Statistiques:"));
  algi_print(algi_add(algi_make_string("   Nombre d'éléments: "), nb));
  algi_print(algi_add(algi_make_string("   Premier élément: "), algi_index_get(liste_triee, algi_make_int(0L))));
  algi_print(algi_add(algi_make_string("   Dernier élément: "), algi_index_get(liste_triee, algi_sub(nb, algi_make_int(1L)))));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   FIN DU PROGRAMME"));
  algi_print(algi_make_string("=========================================="));
  return 0;
}
