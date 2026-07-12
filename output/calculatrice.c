// Code généré par le compilateur ALGI
// Fichier: examples/calculatrice.c
// Date: Sun Jul 12 14:28:07 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* operer(AlgiValue* a, AlgiValue* b, AlgiValue* op) {
  if (algi_value_to_bool(algi_equals(op, algi_make_string("+")))) {
    return algi_add(a, b);
  }
  if (algi_value_to_bool(algi_equals(op, algi_make_string("-")))) {
    return algi_sub(a, b);
  }
  if (algi_value_to_bool(algi_equals(op, algi_make_string("*")))) {
    return algi_mul(a, b);
  }
  if (algi_value_to_bool(algi_equals(op, algi_make_string("/")))) {
    if (algi_value_to_bool(algi_equals(b, algi_make_int(0L)))) {
      algi_print(algi_make_string("Erreur: division par zero impossible"));
      return algi_make_null();
    }
    return algi_div(a, b);
  }
  if (algi_value_to_bool(algi_equals(op, algi_make_string("%")))) {
    if (algi_value_to_bool(algi_equals(b, algi_make_int(0L)))) {
      algi_print(algi_make_string("Erreur: modulo par zero impossible"));
      return algi_make_null();
    }
    return algi_mod(a, b);
  }
  algi_print(algi_add(algi_make_string("Operation inconnue: "), op));
  return algi_make_null();
  return algi_make_null();
}

int main(void) {
  AlgiValue* continuer = algi_make_bool(true);
  AlgiValue* a = algi_make_int(0L);
  AlgiValue* b = algi_make_int(0L);
  AlgiValue* op = algi_make_string("");
  AlgiValue* resultat = algi_make_int(0L);
  AlgiValue* encore = algi_make_string("");
  algi_print(algi_make_string("=== Calculatrice ALGI ==="));
  while (algi_value_to_bool(continuer)) {
    algi_print(algi_make_string("------------------------"));
    algi_print(algi_make_string("Premier nombre :"));
    a = algi_input_from_value(NULL);
    algi_print(algi_make_string("Operation (+ - * / %) :"));
    op = algi_input_from_value(NULL);
    algi_print(algi_make_string("Second nombre :"));
    b = algi_input_from_value(NULL);
    resultat = operer(a, b, op);
    algi_print(algi_add(algi_add(algi_add(algi_add(algi_add(algi_add(a, algi_make_string(" ")), op), algi_make_string(" ")), b), algi_make_string(" = ")), resultat));
    algi_print(algi_make_string("Encore un calcul ? (o/n)"));
    encore = algi_input_from_value(NULL);
    if (algi_value_to_bool(algi_equals(encore, algi_make_string("n")))) {
      continuer = algi_make_bool(false);
    }
  }
  algi_print(algi_make_string("=== Fin de la calculatrice ==="));
  return 0;
}
