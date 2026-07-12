; Code généré par le compilateur ALGI
; Fichier: examples/variables.algi.asm
; Date: Sun Jul 12 20:38:13 2026

; NOTE: ce backend assembleur est un générateur pédagogique
; simplifié. Il gère les entiers (arithmétique, variables,
; boucles, conditions) et l'affichage de littéraux de chaîne,
; mais PAS la concaténation de chaînes ('+' entre chaînes),
; les flottants, ni les listes/objets. Pour un support complet
; de ces fonctionnalités, utiliser le mode -c (génération C).

BITS 32
section .data
  newline db 10, 0
  format_int db "%d", 10, 0
  format_str db "%s", 10, 0
  _str_lit_0 db "", 0
  _str_lit_1 db "Entrez votre nom : ", 0
  _str_lit_2 db "Entrez votre age : ", 0
  _str_lit_3 db "Entrez votre taille (en mètres) : ", 0
  _str_lit_4 db "Entrez votre poids (en kg) : ", 0
  _str_lit_5 db "=== Informations personnelles ===", 0
  _str_lit_6 db "Nom: ", 0
  _str_lit_7 db "Age: ", 0
  _str_lit_8 db "Taille: ", 0
  _str_lit_9 db "Poids: ", 0
  _str_lit_10 db "Majeur? ", 0
  _str_lit_11 db "IMC: ", 0

section .bss
  input_buffer resb 1024

section .text
  global _start

  extern printf
  extern scanf
  extern exit

_start:
  push ebp
  mov ebp, esp
  sub esp, 4096
  ; Declaration: age
  mov eax, 0
  mov [ebp-4], eax
  ; Declaration: taille
  mov eax, 0
  mov [ebp-8], eax
  ; Declaration: nom
  lea eax, [_str_lit_0]
  mov [ebp-12], eax
  ; Declaration: poids
  mov eax, 0
  mov [ebp-16], eax
  lea eax, [_str_lit_1]
  push eax
  push format_str
  call printf
  add esp, 8
  push input_buffer
  push format_str
  call scanf
  add esp, 8
  lea eax, [_str_lit_2]
  push eax
  push format_str
  call printf
  add esp, 8
  push input_buffer
  push format_str
  call scanf
  add esp, 8
  lea eax, [_str_lit_3]
  push eax
  push format_str
  call printf
  add esp, 8
  push input_buffer
  push format_str
  call scanf
  add esp, 8
  lea eax, [_str_lit_4]
  push eax
  push format_str
  call printf
  add esp, 8
  push input_buffer
  push format_str
  call scanf
  add esp, 8
  lea eax, [_str_lit_5]
  push eax
  push format_str
  call printf
  add esp, 8
  lea eax, [_str_lit_6]
  push eax
  mov eax, [ebp-12]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  lea eax, [_str_lit_7]
  push eax
  mov eax, [ebp-4]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  lea eax, [_str_lit_8]
  push eax
  mov eax, [ebp-8]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  lea eax, [_str_lit_9]
  push eax
  mov eax, [ebp-16]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  ; Declaration: est_majeur
  mov eax, [ebp-4]
  push eax
  mov eax, 18
  pop ebx
  cmp eax, ebx
  setge al
  movzx eax, al
  mov [ebp-20], eax
  lea eax, [_str_lit_10]
  push eax
  mov eax, [ebp-20]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  ; Declaration: imc
  mov eax, [ebp-16]
  push eax
  mov eax, [ebp-8]
  push eax
  mov eax, [ebp-8]
  pop ebx
  imul eax, ebx
  pop ebx
  cdq
  idiv ebx
  mov [ebp-24], eax
  lea eax, [_str_lit_11]
  push eax
  mov eax, [ebp-24]
  pop ebx
  add eax, ebx
  push eax
  push format_int
  call printf
  add esp, 8
  mov esp, ebp
  pop ebp
  mov eax, 0
  ret
