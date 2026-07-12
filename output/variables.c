// Code généré par le compilateur ALGI
// Fichier: examples/variables.c
// Date: Sun Jul 12 20:24:53 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

int main(void) {
  AlgiValue* age = algi_make_int(0L);
  AlgiValue* taille = algi_make_int(0L);
  AlgiValue* nom = algi_make_string("");
  AlgiValue* poids = algi_make_int(0L);
  algi_print(algi_make_string("Entrez votre nom : "));
  nom = algi_input_from_value(NULL);
  algi_print(algi_make_string("Entrez votre age : "));
  age = algi_input_from_value(NULL);
  algi_print(algi_make_string("Entrez votre taille (en mètres) : "));
  taille = algi_input_from_value(NULL);
  algi_print(algi_make_string("Entrez votre poids (en kg) : "));
  poids = algi_input_from_value(NULL);
  algi_print(algi_make_string("=== Informations personnelles ==="));
  algi_print(algi_add(algi_make_string("Nom: "), nom));
  algi_print(algi_add(algi_make_string("Age: "), age));
  algi_print(algi_add(algi_make_string("Taille: "), taille));
  algi_print(algi_add(algi_make_string("Poids: "), poids));
  AlgiValue* est_majeur = algi_cmp_ge(age, algi_make_int(18L));
  algi_print(algi_add(algi_make_string("Majeur? "), est_majeur));
  AlgiValue* imc = algi_div(poids, algi_mul(taille, taille));
  algi_print(algi_add(algi_make_string("IMC: "), imc));
  return 0;
}
