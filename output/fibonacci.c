// Code généré par le compilateur ALGI
// Fichier: examples/fibonacci.c
// Date: Sat Jul 11 12:27:42 2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "runtime.h"

AlgiValue* fibonacci(AlgiValue* nbr) {
  if (algi_value_to_bool(algi_cmp_le(nbr, algi_make_int(1L)))) {
    return nbr;
  }
  else {
    return algi_add(fibonacci(algi_sub(nbr, algi_make_int(1L))), fibonacci(algi_sub(nbr, algi_make_int(2L))));
  }
  return algi_make_null();
}

int main(void) {
  algi_print(algi_make_string("=== SUITE DE FIBONACCI ==="));
  AlgiValue* nb = algi_make_int(0L);
  algi_print(algi_make_string("Entrez le nombre de terme : "));
  nb = algi_input_from_value(NULL);
  AlgiValue* i = algi_make_int(0L);
  while (algi_value_to_bool(algi_cmp_lt(i, nb))) {
    AlgiValue* val = fibonacci(i);
    algi_print(algi_add(algi_add(algi_add(algi_make_string("F("), i), algi_make_string(") = ")), val));
    i = algi_add(i, algi_make_int(1L));
  }
  algi_print(algi_make_string("=== FIN ==="));
  return 0;
}
