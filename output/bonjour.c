// Code généré par le compilateur ALGI
// Fichier: examples/bonjour.c
// Date: Sat Jul 11 12:16:59 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

int main(void) {
  algi_print(algi_make_string("Bonjour le monde!"));
  algi_print(algi_make_string("Bienvenue dans ALGI, le langage Bamiléké!"));
  AlgiValue* nom = algi_make_string("");
  algi_print(algi_make_string("Entrez votre nom"));
  nom = algi_input_from_value(NULL);
  algi_print(algi_add(algi_make_string("Bonjour "), nom));
  return 0;
}
