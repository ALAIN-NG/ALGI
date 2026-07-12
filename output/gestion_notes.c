// Code généré par le compilateur ALGI
// Fichier: examples/gestion_notes.c
// Date: Sun Jul 12 14:45:00 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* moyenne(AlgiValue* n1, AlgiValue* n2, AlgiValue* n3) {
  return algi_div(algi_add(algi_add(n1, n2), n3), algi_make_int(3L));
  return algi_make_null();
}

AlgiValue* mention(AlgiValue* m) {
  if (algi_value_to_bool(algi_cmp_ge(m, algi_make_int(16L)))) {
    return algi_make_string("Tres Bien");
  }
  if (algi_value_to_bool(algi_cmp_ge(m, algi_make_int(14L)))) {
    return algi_make_string("Bien");
  }
  if (algi_value_to_bool(algi_cmp_ge(m, algi_make_int(12L)))) {
    return algi_make_string("Assez Bien");
  }
  if (algi_value_to_bool(algi_cmp_ge(m, algi_make_int(10L)))) {
    return algi_make_string("Passable");
  }
  return algi_make_string("Ajourne");
  return algi_make_null();
}

AlgiValue* est_admis(AlgiValue* m) {
  if (algi_value_to_bool(algi_cmp_ge(m, algi_make_int(10L)))) {
    return algi_make_bool(true);
  }
  return algi_make_bool(false);
  return algi_make_null();
}

int main(void) {
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   GESTION DES NOTES DES ETUDIANTS"));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string(""));
  AlgiValue* capacite_max = algi_make_int(30L);
  AlgiValue* nb_etudiants = algi_make_int(0L);
  algi_print(algi_add(algi_add(algi_make_string("Combien d'etudiants voulez-vous saisir ? (max "), capacite_max), algi_make_string(")")));
  nb_etudiants = algi_input_from_value(NULL);
  if (algi_value_to_bool(algi_cmp_gt(nb_etudiants, capacite_max))) {
    algi_print(algi_add(algi_add(algi_make_string("Attention: la capacite maximale est de "), capacite_max), algi_make_string(" etudiants")));
  }
  AlgiList* _algi_tmp_0 = algi_list_create(30);
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  algi_list_append(_algi_tmp_0, algi_make_string(""));
  AlgiValue* noms = algi_make_list(_algi_tmp_0);
  AlgiList* _algi_tmp_1 = algi_list_create(30);
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
  AlgiValue* moyennes = algi_make_list(_algi_tmp_1);
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, nb_etudiants))) {
    algi_print(algi_make_string(""));
    algi_print(algi_add(algi_add(algi_make_string("--- Etudiant "), algi_add(i, algi_make_int(1L))), algi_make_string(" ---")));
    AlgiValue* nom = algi_make_string("");
    algi_print(algi_make_string("Nom de l'etudiant:"));
    nom = algi_input_from_value(NULL);
    AlgiValue* note1 = algi_make_int(0L);
    algi_print(algi_make_string("Note 1 (/20):"));
    note1 = algi_input_from_value(NULL);
    AlgiValue* note2 = algi_make_int(0L);
    algi_print(algi_make_string("Note 2 (/20):"));
    note2 = algi_input_from_value(NULL);
    AlgiValue* note3 = algi_make_int(0L);
    algi_print(algi_make_string("Note 3 (/20):"));
    note3 = algi_input_from_value(NULL);
    AlgiValue* m = moyenne(note1, note2, note3);
    algi_index_set(noms, i, nom);
    algi_index_set(moyennes, i, m);
    algi_print(algi_add(algi_add(algi_add(algi_add(algi_add(algi_make_string("Moyenne de "), nom), algi_make_string(" : ")), m), algi_make_string(" - ")), mention(m)));
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   RECAPITULATIF DE LA CLASSE"));
  algi_print(algi_make_string("=========================================="));
  AlgiValue* somme = algi_make_int(0L);
  AlgiValue* nb_admis = algi_make_int(0L);
  AlgiValue* j = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(j, nb_etudiants))) {
    algi_print(algi_add(algi_add(algi_add(algi_add(algi_add(algi_index_get(noms, j), algi_make_string(" : ")), algi_index_get(moyennes, j)), algi_make_string("/20 (")), mention(algi_index_get(moyennes, j))), algi_make_string(")")));
    somme = algi_add(somme, algi_index_get(moyennes, j));
    if (algi_value_to_bool(est_admis(algi_index_get(moyennes, j)))) {
      nb_admis = algi_add(nb_admis, algi_make_int(1L));
    }
    j = algi_add(j, algi_make_int(1L));
  }
  AlgiValue* moyenne_classe = algi_div(somme, nb_etudiants);
  algi_print(algi_make_string(""));
  algi_print(algi_add(algi_make_string("Moyenne generale de la classe : "), moyenne_classe));
  algi_print(algi_add(algi_add(algi_add(algi_make_string("Etudiants admis : "), nb_admis), algi_make_string(" / ")), nb_etudiants));
  algi_print(algi_add(algi_make_string("Etudiants ajournes : "), algi_sub(nb_etudiants, nb_admis)));
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   CLASSEMENT"));
  algi_print(algi_make_string("=========================================="));
  AlgiValue* a = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(a, algi_sub(nb_etudiants, algi_make_int(1L))))) {
    AlgiValue* max_index = a;
    AlgiValue* b = algi_add(a, algi_make_int(1L));
    while (algi_value_to_bool(algi_cmp_lt(b, nb_etudiants))) {
      if (algi_value_to_bool(algi_cmp_gt(algi_index_get(moyennes, b), algi_index_get(moyennes, max_index)))) {
        max_index = b;
      }
      b = algi_add(b, algi_make_int(1L));
    }
    if (algi_value_to_bool(algi_not_equals(max_index, a))) {
      AlgiValue* temp_m = algi_index_get(moyennes, a);
      algi_index_set(moyennes, a, algi_index_get(moyennes, max_index));
      algi_index_set(moyennes, max_index, temp_m);
      AlgiValue* temp_n = algi_index_get(noms, a);
      algi_index_set(noms, a, algi_index_get(noms, max_index));
      algi_index_set(noms, max_index, temp_n);
    }
    a = algi_add(a, algi_make_int(1L));
  }
  AlgiValue* rang = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(rang, nb_etudiants))) {
    algi_print(algi_add(algi_add(algi_add(algi_add(algi_add(algi_add(rang, algi_make_int(1L)), algi_make_string(". ")), algi_index_get(noms, rang)), algi_make_string(" - ")), algi_index_get(moyennes, rang)), algi_make_string("/20")));
    rang = algi_add(rang, algi_make_int(1L));
  }
  algi_print(algi_make_string(""));
  algi_print(algi_make_string("=========================================="));
  algi_print(algi_make_string("   FIN DU PROGRAMME"));
  algi_print(algi_make_string("=========================================="));
  return 0;
}
