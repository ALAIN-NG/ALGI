# ALGI - Langage de Programmation Bamiléké

## 📖 Description

ALGI est un langage de programmation dont les mots-clés sont inspirés du lexique de la langue Bamiléké (région de l'Ouest-Cameroun). Ce projet a été réalisé dans le cadre du Master 1 en Sécurité Informatique à l'Université de Yaoundé I.

## 🎯 Objectifs

- Valoriser et documenter un corpus lexical d'une langue camerounaise
- Démonstration de la faisabilité d'un langage formel construit sur une langue africaine
- Application des techniques de conception de langages (analyse lexicale, syntaxique, génération de code)

## 🏗️ Architecture

Le compilateur ALGI est construit avec les outils classiques de compilation :
Source ALGI (.algi)
↓
Analyseur Lexical (flex)
↓
Analyseur Syntaxique (bison)
↓
Générateur de Code (C / Assembleur x86)
↓
Exécutable

text

## 📦 Structure du Projet
algi-compiler/
├── src/
│ ├── algi.l # Analyseur lexical (flex)
│ ├── algi.y # Analyseur syntaxique (bison)
│ ├── ast.h/c # Arbre Syntaxique Abstrait
│ ├── symbol.h/c # Table des symboles
│ ├── codegen.h/c # Générateur de code C
│ ├── codegen_asm.h/c # Générateur de code assembleur
│ └── runtime.h/c # Runtime
├── examples/ # Exemples ALGI
├── tests/ # Tests
├── output/ # Fichiers générés
├── Makefile
└── README.md

text

## 🚀 Installation

### Prérequis

- GCC (ou Clang)
- Flex
- Bison
- Make

### Compilation

```bash
git clone <repository>
cd algi-compiler
make
📝 Utilisation
Compiler un programme ALGI vers C
bash
./bin/algi -c examples/test.algi
gcc -I./src -o test examples/test.c src/runtime.c -lm
./test
Compiler un programme ALGI vers Assembleur x86
bash
# Nécessite nasm et un environnement 32 bits (sur Debian/Ubuntu 64 bits :
# sudo apt install nasm gcc-multilib libc6-dev-i386)
./bin/algi -a examples/test.algi
nasm -f elf32 -o test.o examples/test.algi.asm
gcc -m32 -o test test.o -lc
./test
Note: le mode assembleur (-a) est un générateur pédagogique simplifié.
Il gère correctement les entiers (arithmétique, variables, boucles,
conditions) et l'affichage de littéraux de chaîne, mais PAS la
concaténation de chaînes ('+' entre chaînes), les flottants, ni les
listes/objets. Pour un programme ALGI complet et fonctionnel, utiliser
le mode -c (génération C), entièrement supporté.
📚 Mots-Clés ALGI
Mot ALGI	Sens	Mot ALGI	Sens
ke	si	kommenu	sinon
w	tant que	pou	pour
mi	fin	he	déclaration
fonksion	fonction	kwouo	classe
feh	nouveau	tem	retour
peu	casser	tela	essayer
tatem	attraper	tehap	afficher
keksie	lire	sanwe	évaluer
waan	somme	tie	soustraire
denden	égal	nwagniehwe	trier
ge	typeof	ganfa	ouvrir
stegnie	exit	nene	vrai
pok	faux	n	nul
e	et	ou	ou
poke	non	est	est
dans	dans	comme	comme
bi	liste	saa	entier
reyel	réel	djanghom	chaîne
🧪 Exemple
python
// Programme ALGI
he age = 18
he nom = "Alain"
he est_majeur = (age >= 18)

tehap(nom)
tehap(age)
tehap(est_majeur)

ke (est_majeur)
    tehap("Tu es majeur!")
kommenu
    tehap("Tu es mineur!")
mi
📄 Licence
Ce projet est développé dans un cadre académique.

👤 Auteur
Alain - Master 1 Sécurité Informatique - Université de Yaoundé I

text

---

## Résumé des fichiers créés

| # | Fichier | Description |
|---|---------|-------------|
| 1 | `src/algi.l` | Analyseur lexical Flex |
| 2 | `src/algi.y` | Analyseur syntaxique Bison |
| 3 | `src/ast.h` | Définition de l'AST |
| 4 | `src/ast.c` | Implémentation de l'AST |
| 5 | `src/symbol.h` | Table des symboles |
| 6 | `src/symbol.c` | Implémentation de la table |
| 7 | `src/codegen.h` | Générateur de code C |
| 8 | `src/codegen.c` | Implémentation du générateur C |
| 9 | `src/codegen_asm.h` | Générateur de code assembleur |
| 10 | `src/codegen_asm.c` | Implémentation du générateur ASM |
| 11 | `src/runtime.h` | Runtime pour le code C |
| 12 | `src/runtime.c` | Implémentation du runtime |
| 13 | `Makefile` | Compilation du projet |
| 14 | `examples/test.algi` | Exemple de test |
| 15 | `examples/test.c` | Code C généré |
| 16 | `README.md` | Documentation |

## Compilation et test

```bash
# Créer la structure
mkdir -p src examples output

# Copier tous les fichiers

# Compiler
make

# Tester
make test

# Compiler vers C
make example-c

# Compiler vers Assembleur
make example-asm