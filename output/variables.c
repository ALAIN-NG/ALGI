// Code généré par le compilateur ALGI
// Fichier: examples/variables.c
// Date: Sat Jul 11 13:09:29 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

int main(void) {
  AlgiValue* age = algi_make_int(25L);
  AlgiValue* taille = algi_make_float(1.75);
  AlgiValue* nom = algi_make_string("Alain");
  algi_print(algi_make_string("=== Informations personnelles ==="));
  algi_print(algi_add(algi_make_string("Nom: "), nom));
  algi_print(algi_add(algi_make_string("Age: "), age));
  algi_print(algi_add(algi_make_string("Taille: "), taille));
  AlgiValue* est_majeur = algi_cmp_ge(age, algi_make_int(18L));
  algi_print(algi_add(algi_make_string("Majeur? "), est_majeur));
  AlgiValue* imc = algi_mul(taille, taille);
  algi_print(algi_add(algi_make_string("Taille au carre: "), imc));
  return 0;
}
