# ALGI — Un langage de programmation inspiré du lexique Bamiléké

> Compilateur source-à-source (et générateur assembleur pédagogique) pour un langage de programmation impératif dont les mots-clés sont empruntés au lexique de la langue Bamiléké (région de l'Ouest-Cameroun).

[![Langage](https://img.shields.io/badge/langage-C-blue.svg)](#)
[![Build](https://img.shields.io/badge/build-flex%20%2B%20bison%20%2B%20gcc-informational.svg)](#)
[![Licence](https://img.shields.io/badge/licence-académique-lightgrey.svg)](#licence)
[![Statut](https://img.shields.io/badge/statut-projet%20universitaire-success.svg)](#)

**Dépôt :** [`ALAIN-NG/ALGI`](https://github.com/ALAIN-NG/ALGI)
**Auteur :** Alain — Master 1 Sécurité Informatique, Université de Yaoundé I

---

## Table des matières

1. [Présentation générale](#1-présentation-générale)
2. [Motivation et objectifs scientifiques](#2-motivation-et-objectifs-scientifiques)
3. [Architecture du compilateur](#3-architecture-du-compilateur)
4. [Pipeline de compilation en détail](#4-pipeline-de-compilation-en-détail)
5. [Structure du dépôt](#5-structure-du-dépôt)
6. [Spécification du langage ALGI](#6-spécification-du-langage-algi)
7. [Table des mots-clés](#7-table-des-mots-clés)
8. [Grammaire (BNF simplifiée)](#8-grammaire-bnf-simplifiée)
9. [Système de types et modèle mémoire](#9-système-de-types-et-modèle-mémoire)
10. [Installation](#10-installation)
11. [Utilisation](#11-utilisation)
12. [Exemples commentés](#12-exemples-commentés)
13. [Limitations connues](#13-limitations-connues)
14. [Historique des correctifs majeurs](#14-historique-des-correctifs-majeurs)
15. [Outillage éditeur (extension VS Code)](#15-outillage-éditeur-extension-vs-code)
16. [Tests](#16-tests)
17. [Pistes d'évolution](#17-pistes-dévolution)
18. [Licence](#18-licence)
19. [Auteur et remerciements](#19-auteur-et-remerciements)

---

## 1. Présentation générale

**ALGI** est un compilateur pour un langage de programmation impératif, à typage dynamique, dont l'ensemble des mots réservés (structures de contrôle, déclarations, fonctions natives, opérateurs logiques, types primitifs) est directement puisé dans le lexique de la langue **Bamiléké**. Le projet ne se limite pas à une curiosité linguistique : il implémente une chaîne de compilation complète et fonctionnelle, construite avec les outils académiques de référence de la théorie des langages — **Flex** pour l'analyse lexicale et **Bison** pour l'analyse syntaxique — puis deux back-ends de génération de code : un générateur de **code C** (portable, complet) et un générateur d'**assembleur x86** (à visée pédagogique, sous-ensemble du langage).

Le compilateur ALGI accepte des fichiers source `.algi` et produit, au choix :

- un fichier **C** exploitant un runtime dynamique dédié (`runtime.c` / `runtime.h`), lui-même compilable par GCC vers un exécutable natif ;
- un fichier **assembleur x86 (syntaxe NASM, ELF 32 bits)** pour un sous-ensemble du langage limité aux entiers, à l'arithmétique, aux boucles, aux conditions et à l'affichage de littéraux de chaîne.

Ce projet a été réalisé dans le cadre du **Master 1 en Sécurité Informatique** à l'**Université de Yaoundé I**, et constitue une application concrète des techniques de conception de compilateurs (analyse lexicale, analyse syntaxique descendante/ascendante, construction d'AST, table des symboles, génération de code).

---

## 2. Motivation et objectifs scientifiques

Le projet ALGI répond à trois objectifs complémentaires :

| Objectif | Description |
|---|---|
| **Valorisation linguistique** | Documenter et mettre en usage un corpus lexical d'une langue camerounaise (Bamiléké) en le transposant dans un contexte informatique, contribuant modestement à la préservation numérique de langues africaines sous-représentées dans l'outillage logiciel. |
| **Faisabilité technique** | Démontrer qu'un langage de programmation formel, complet (structures de contrôle, fonctions, classes rudimentaires, listes, gestion d'erreurs) peut être construit intégralement sur un vocabulaire non-anglophone et non-latin d'origine, sans compromis sur l'expressivité du langage. |
| **Pédagogie en compilation** | Mettre en pratique l'ensemble de la chaîne de compilation enseignée en cursus d'informatique : lexer (Flex), parser LALR (Bison), AST, table des symboles à portée hiérarchique, génération de code C et génération d'assembleur x86 bas niveau. |

---

## 3. Architecture du compilateur

Le compilateur suit une architecture **modulaire classique en pipeline**, chaque étage produisant une représentation intermédiaire consommée par l'étage suivant :

```
                 ┌────────────────────┐
Fichier source   │   Analyseur        │   Flux de
 (.algi)  ─────► │   lexical (Flex)   │ ─► tokens
                 │   src/algi.l       │
                 └─────────┬──────────┘
                           │
                           ▼
                 ┌────────────────────┐
                 │   Analyseur        │
                 │   syntaxique       │ ──► Arbre syntaxique
                 │   (Bison, LALR(1)) │      abstrait (AST)
                 │   src/algi.y       │
                 └─────────┬──────────┘
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
   ┌───────────────────┐       ┌─────────────────────┐
   │ Table des symboles│       │   AST (ast.c/.h)    │
   │  symbol.c / .h    │◄──────┤  nœuds typés,       │
   │portée hiérarchique│       │  union discriminée  │
   └───────────────────┘       └──────────┬──────────┘
                                          │
                       ┌──────────────────┴───────────────────┐
                       ▼                                      ▼
            ┌─────────────────────┐               ┌─────────────────────────┐
            │  Générateur de      │               │  Générateur assembleur  │
            │  code C             │               │  x86 (NASM, ELF32)      │
            │  codegen.c/.h       │               │  codegen_asm.c/.h       │
            └──────────┬──────────┘               └────────────┬────────────┘
                       │                                       │
                       ▼                                       ▼
              Fichier .c généré                         Fichier .asm généré
             (s'appuie sur runtime.c/.h)               (arithmétique entière,
                       │                                boucles, conditions,
                       ▼                                chaînes littérales)
                 gcc + runtime.c
                       │
                       ▼
                Exécutable natif
```

### 3.1. Modules du compilateur

| Module | Fichiers | Rôle |
|---|---|---|
| **Lexer** | `src/algi.l` | Reconnaissance des unités lexicales (mots-clés Bamiléké, littéraux, opérateurs, commentaires) et production de tokens Bison via `flex`. |
| **Parser** | `src/algi.y` | Grammaire LALR(1) définissant la syntaxe du langage ; construit l'AST au fil de la réduction des règles ; gère la précédence des opérateurs et la détection d'erreurs syntaxiques. |
| **AST** | `src/ast.c`, `src/ast.h` | Définition des types de nœuds (`NodeType`), des opérateurs (`OperatorType`), des types de données (`DataType`) et des fonctions de construction/libération/affichage de l'arbre. |
| **Table des symboles** | `src/symbol.c`, `src/symbol.h` | Table à portées imbriquées (`SymbolTable` chaînée par `parent`), permettant la résolution de noms de variables, fonctions et classes, et la détection de variables non déclarées. |
| **Générateur C** | `src/codegen.c`, `src/codegen.h` | Traduction de chaque nœud AST en instructions C exploitant le runtime dynamique `AlgiValue*`. Émet un fichier `.c` autonome, prêt à être compilé par GCC. |
| **Générateur ASM** | `src/codegen_asm.c`, `src/codegen_asm.h` | Traduction d'un sous-ensemble du langage (entiers, arithmétique, boucles, conditions, affichage de chaînes littérales) en assembleur x86 32 bits, syntaxe NASM. |
| **Runtime** | `src/runtime.c`, `src/runtime.h` | Bibliothèque C liée au code généré, fournissant le système de valeurs dynamiquement typées `AlgiValue`, les listes (`AlgiList`), les objets (`AlgiObject`), les opérateurs arithmétiques/logiques polymorphes, et les fonctions natives (`tehap`, `keksie`, `nwagniehwe`, etc.). |

### 3.2. Le système de valeurs dynamiques `AlgiValue`

Le cœur du back-end C repose sur une structure de valeur unique, taggée par type, qui permet au code C généré de manipuler des données ALGI sans connaître leur type à la compilation :

```c
typedef enum {
    TYPE_INT = 1, TYPE_FLOAT = 2, TYPE_STRING = 3, TYPE_BOOL = 4,
    TYPE_LIST = 5, TYPE_OBJECT = 6, TYPE_VOID = 7, TYPE_NULL = 8, TYPE_ERROR = 9
} AlgiType;

typedef struct {
    AlgiType type;
    union {
        long int_value;
        double float_value;
        bool bool_value;
        char* string_value;
        void* ptr_value;
    } value;
} AlgiValue;
```

Toute expression ALGI (littéral, appel, opération binaire, accès indexé...) est ainsi traduite par `codegen_gen_expr()` en une expression C évaluant à un `AlgiValue*`. Les opérateurs binaires ALGI sont résolus dynamiquement à l'exécution par des fonctions polymorphes du runtime (`algi_add`, `algi_sub`, `algi_cmp_lt`, `algi_logical_and`, etc.), qui appliquent les règles de coercition adaptées (par exemple, l'opérateur `+` concatène automatiquement des chaînes si l'un des deux opérandes est de type `TYPE_STRING`, comme l'illustrent les exemples `gestion_notes.algi` et `tri_fusion.algi`).

---

## 4. Pipeline de compilation en détail

1. **Lecture du fichier source** (`main()` dans `src/algi.y`) : ouverture du fichier `.algi` passé en argument, redirection du flux vers `yyin`.
2. **Analyse lexicale** (`yylex()`, généré depuis `src/algi.l`) : découpage du texte source en tokens (mots-clés, identifiants, nombres, chaînes, opérateurs), avec suivi du numéro de ligne (`%option yylineno`) pour un rapport d'erreurs précis.
3. **Analyse syntaxique** (`yyparse()`, généré depuis `src/algi.y`) : reconnaissance de la grammaire LALR(1) et construction incrémentale de l'AST via les actions sémantiques Bison ; alimentation en parallèle de la table des symboles globale (déclarations de variables, fonctions, classes).
4. **Vérifications sémantiques légères** : détection des variables utilisées avant déclaration (message d'erreur avec numéro de ligne), report d'erreurs de syntaxe (`has_error`), abandon de la compilation en cas d'erreur bloquante.
5. **Génération de code** :
   - **Mode C (`-c`, par défaut)** : `generate_code()` parcourt l'AST et écrit un fichier `.c` autonome, `#include`-ant `runtime.h`, prêt pour `gcc ... src/runtime.c`.
   - **Mode assembleur (`-a`)** : `generate_asm()` parcourt l'AST et produit un fichier `.asm` (syntaxe NASM, cible ELF 32 bits) pour le sous-ensemble entier du langage.
6. **Compilation native** : le fichier `.c` généré est compilé par GCC avec le runtime (`gcc -I./src -o prog prog.c src/runtime.c -lm`) ; le fichier `.asm` est assemblé par NASM (`nasm -f elf32`) puis lié par GCC en mode 32 bits (`gcc -m32`).
7. **Exécution** : l'exécutable natif produit s'exécute indépendamment du compilateur ALGI.

---

## 5. Structure du dépôt

```
ALGI/
├── src/                        # Cœur du compilateur
│   ├── algi.l                  # Analyseur lexical (spécification Flex)
│   ├── algi.y                  # Analyseur syntaxique + main() (spécification Bison)
│   ├── ast.h / ast.c           # Arbre syntaxique abstrait
│   ├── symbol.h / symbol.c     # Table des symboles à portées imbriquées
│   ├── codegen.h / codegen.c   # Générateur de code C (back-end principal)
│   ├── codegen_asm.h / codegen_asm.c   # Générateur assembleur x86 (back-end pédagogique)
│   └── runtime.h / runtime.c   # Runtime C : AlgiValue, AlgiList, AlgiObject, fonctions natives
├── examples/                   # Programmes ALGI de démonstration (.algi)
│   ├── bonjour.algi            # Hello World
│   ├── variables.algi          # Déclarations et types
│   ├── boucles.algi            # Boucles w / pou
│   ├── calculatrice.algi       # Opérations arithmétiques interactives
│   ├── fibonacci.algi          # Récursivité
│   ├── tri.algi                # Tri par sélection
│   ├── tri_fusion.algi         # Tri fusion (merge sort) récursif
│   ├── listes.algi             # Manipulation de listes (bi[...])
│   ├── gestion_notes.algi      # Programme complet : gestion de notes d'étudiants
│   ├── alain.algi              # Exemple libre
│   └── test.algi               # Programme de test par défaut (make test)
├── tests/
│   └── test_lexer.algi         # Test ciblé de l'analyseur lexical
├── output/                     # Fichiers générés (.c, .asm, exécutables) — reconstruit à la compilation
├── build/                      # Artefacts intermédiaires Bison/Flex (.tab.c/.h, lex.yy.c, .o) — reconstruit
├── bin/                        # Exécutable du compilateur (bin/algi) — reconstruit
├── docu/
│   └── presentation.tex/.pdf   # Support de présentation LaTeX/Beamer du projet
├── vscode-algi/                # Extension Visual Studio Code (coloration syntaxique ALGI)
│   ├── package.json
│   ├── syntaxes/algi.tmLanguage.json
│   ├── language-configuration.json
│   ├── icons/                  # Thème d'icônes de fichiers .algi
│   └── install.sh              # Script d'installation de l'extension
├── algi                        # Script shell « riche » : build auto + compilation + exécution (+GDB)
├── algi-compile                # Script shell simplifié : compilation + exécution
├── test-all                    # Script d'exécution de la suite de tests
├── Makefile                    # Orchestration de la compilation (flex, bison, gcc)
└── README.md
```

---

## 6. Spécification du langage ALGI

ALGI est un langage **impératif**, à **typage dynamique**, avec une syntaxe volontairement dépourvue d'accolades obligatoires : chaque bloc structurel (condition, boucle, fonction, classe, gestion d'erreurs) s'ouvre par un mot-clé dédié et se referme systématiquement par le mot-clé de fin **`mi`** — un choix stylistique proche des langages de la famille ALGOL/Pascal, mais tirant son vocabulaire du Bamiléké.

### 6.1. Caractéristiques principales

- **Déclarations typées ou inférées** : `he x = 5` (inférence) ou `he x : saa = 5` (annotation de type explicite).
- **Structures de contrôle** : conditionnelle `ke ... kommenu ... mi`, boucle conditionnelle `w ... mi`, boucle `pou ... dans ... mi` (for-each sur liste).
- **Fonctions** avec paramètres et corps de bloc : `fonksion nom(params) ... tem valeur mi`.
- **Classes rudimentaires** : `kwouo Nom ... mi`, instanciation via `feh Nom(args)`.
- **Gestion d'erreurs** : bloc `tela ... tatem (var) ... mi` (try/catch).
- **Listes hétérogènes** : littéraux `bi[...]`, accès indexé `liste[i]`, affectation indexée.
- **Fonctions natives intégrées** : affichage (`tehap`), lecture clavier (`keksie`), tri (`nwagniehwe`), typeof (`ge`), sortie du programme (`stegnie`), etc.
- **Opérateurs logiques et de comparaison** en toutes lettres (`e`, `ou`, `poke`, `est`, `dans`) en complément des opérateurs symboliques usuels (`==`, `!=`, `<=`, `>=`).
- **Commentaires** de ligne (`// ...`) et de bloc (`/* ... */`).
- **Concaténation implicite** : l'opérateur `+` concatène automatiquement chaînes et valeurs numériques converties, comme démontré dans `gestion_notes.algi` (`"Moyenne de " + nom + " : " + m`).

### 6.2. Exemple minimal

```algi
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
```

---

## 7. Table des mots-clés

### 7.1. Structures de contrôle et déclarations

| Mot ALGI | Signification | Mot ALGI | Signification |
|---|---|---|---|
| `ke` | si (if) | `kommenu` | sinon (else) |
| `w` | tant que (while) | `pou` | pour (for) |
| `mi` | fin (end) | `he` | déclaration (let) |
| `fonksion` | fonction | `kwouo` | classe |
| `feh` | nouveau (new) | `tem` | retour (return) |
| `peu` | casser (break) | `tela` | essayer (try) |
| `tatem` | attraper (catch) | | |

### 7.2. Fonctions natives

| Mot ALGI | Signification | Mot ALGI | Signification |
|---|---|---|---|
| `tehap` | afficher (print) | `keksie` | lire (input) |
| `sanwe` | évaluer (eval) | `waan` | somme (sum) |
| `tie` | soustraire (subtract) | `denden` | égal (equals) |
| `nwagniehwe` | trier (sort) | `ge` | typeof |
| `ganfa` | ouvrir (open) | `stegnie` | exit |

### 7.3. Littéraux et types

| Mot ALGI | Signification | Mot ALGI | Signification |
|---|---|---|---|
| `nene` | vrai (true) | `pok` | faux (false) |
| `n` | nul (null) | `bi` | liste (list) |
| `saa` | entier (int) | `reyel` / `nka'tche` | réel (float) |
| `djanghom` | chaîne (string) | | |

### 7.4. Opérateurs logiques et spéciaux

| Mot ALGI | Signification | Mot ALGI | Signification |
|---|---|---|---|
| `e` | et (and) | `ou` | ou (or) |
| `poke` | non (not) | `est` | est (is) |
| `dans` | dans (in) | `comme` | comme (as) |

### 7.5. Opérateurs symboliques

`==` `!=` `<=` `>=` `<` `>` `=` `+` `-` `*` `/` `%` `^` `( ) { } [ ] ; , . :`

---

## 8. Grammaire (BNF simplifiée)

La grammaire complète, formelle, est définie dans `src/algi.y` (spécification Bison, résolue en LALR(1)). Extrait simplifié :

```
program        ::= instructions

instruction    ::= declaration | assignment | if_statement | while_loop
                 | for_loop | function_def | class_def | return_statement
                 | break_statement | print_statement | input_statement
                 | try_catch | expression ';' | ';'

declaration    ::= 'he' IDENTIFIER '=' expression
                 | 'he' IDENTIFIER ':' type_annotation '=' expression

if_statement   ::= 'ke' '(' expression ')' block 'mi'
                 | 'ke' '(' expression ')' block 'kommenu' block 'mi'

while_loop     ::= 'w' '(' expression ')' block 'mi'

for_loop       ::= 'pou' '(' IDENTIFIER 'dans' expression ')' block 'mi'

function_def   ::= 'fonksion' IDENTIFIER '(' parameter_list ')' block 'mi'

class_def      ::= 'kwouo' IDENTIFIER block 'mi'

try_catch      ::= 'tela' block 'tatem' '(' IDENTIFIER ')' block 'mi'

block          ::= '{' instructions '}' | instructions

expression     ::= assignment | logical_or
logical_or     ::= logical_and ( 'ou' logical_and )*
logical_and    ::= equality ( 'e' equality )*
equality       ::= comparison ( ('==' | '!=') comparison )*
comparison     ::= addition ( ('<' | '>' | '<=' | '>=' | 'est' | 'dans') addition )*
addition       ::= multiplication ( ('+' | '-') multiplication )*
multiplication ::= unary ( ('*' | '/' | '%') unary )*
unary          ::= postfix | '-' unary | 'poke' unary
postfix        ::= primary ( '(' args ')' | '[' expression ']' | '.' IDENTIFIER )*
primary        ::= NUMBER | STRING | 'nene' | 'pok' | 'n' | IDENTIFIER
                 | '(' expression ')' | list_literal | new_instance
```

**Précédence des opérateurs** (de la plus faible à la plus forte) : affectation `=` → `ou` → `e` → `==`/`!=` → comparaisons/`est`/`dans` → `+`/`-` → `*`/`/`/`%` → `poke` (négation logique) → moins unaire.

---

## 9. Système de types et modèle mémoire

### 9.1. Types de données

| Type ALGI | Constante interne (`DataType` / `AlgiType`) | Description |
|---|---|---|
| `saa` | `AST_TYPE_INT` / `TYPE_INT` | Entier (représenté en `long` à l'exécution) |
| `reyel` / `nka'tche` | `AST_TYPE_FLOAT` / `TYPE_FLOAT` | Nombre à virgule flottante (`double`) |
| `djanghom` | `AST_TYPE_STRING` / `TYPE_STRING` | Chaîne de caractères |
| — | `AST_TYPE_BOOL` / `TYPE_BOOL` | Booléen (`nene` / `pok`) |
| `bi` | `AST_TYPE_LIST` / `TYPE_LIST` | Liste dynamique hétérogène |
| — | `AST_TYPE_OBJECT` / `TYPE_OBJECT` | Instance de classe |
| — | `AST_TYPE_VOID` / `TYPE_VOID` | Absence de valeur |
| `n` | — / `TYPE_NULL` | Valeur nulle |

### 9.2. Portée des symboles

La table des symboles (`symbol.c`) implémente une **chaîne de portées** (`SymbolTable.parent`), permettant en principe l'imbrication de portées lexicales (fonctions, blocs). Chaque symbole enregistre son espèce (`SymbolKind` : variable, fonction, classe, paramètre, champ, méthode), son type de données et son niveau de portée.

### 9.3. Modèle mémoire du runtime généré

Le code C généré manipule exclusivement des pointeurs `AlgiValue*` alloués dynamiquement. Les listes (`AlgiList`) et objets (`AlgiObject`) sont eux-mêmes alloués sur le tas et redimensionnés dynamiquement (`capacity` / `length`). Le runtime fournit des fonctions de libération dédiées (`algi_value_free`, `algi_list_free`) — la gestion de la durée de vie des valeurs reste toutefois à la charge du code généré (pas de ramasse-miettes).

---

## 10. Installation

### 10.1. Prérequis

| Outil | Rôle | Paquet Debian/Ubuntu |
|---|---|---|
| GCC (ou Clang) | Compilation du compilateur et des programmes générés | `gcc` |
| Flex | Génération de l'analyseur lexical | `flex` |
| Bison | Génération de l'analyseur syntaxique | `bison` |
| Make | Orchestration de la compilation | `make` |
| NASM *(optionnel)* | Assemblage du mode `-a` (backend x86) | `nasm` |
| gcc-multilib / libc6-dev-i386 *(optionnel)* | Édition de liens 32 bits pour le mode `-a` | `gcc-multilib libc6-dev-i386` |

Installation des prérequis sur Debian/Ubuntu :

```bash
sudo apt update
sudo apt install gcc flex bison make
# Pour le backend assembleur (optionnel) :
sudo apt install nasm gcc-multilib libc6-dev-i386
```

### 10.2. Compilation du compilateur

```bash
git clone https://github.com/ALAIN-NG/ALGI.git
cd ALGI
make
```

La cible `make all` (par défaut) :
1. crée les répertoires `build/`, `bin/`, `output/` ;
2. invoque `flex` sur `src/algi.l` puis `bison` sur `src/algi.y` ;
3. compile chaque module (`ast.c`, `symbol.c`, `codegen.c`, `codegen_asm.c`, `runtime.c`) en objets ;
4. édite les liens en un exécutable unique `bin/algi` ;
5. rend exécutables les scripts `algi`, `algi-compile`, `test-all`.

### 10.3. Installation système (optionnelle)

```bash
sudo make install     # copie bin/algi vers /usr/local/bin/algi
sudo make uninstall   # supprime /usr/local/bin/algi
```

---

## 11. Utilisation

### 11.1. Compilation directe via `bin/algi`

**Vers du code C (mode par défaut, entièrement supporté) :**

```bash
./bin/algi -c examples/test.algi
gcc -I./src -o test examples/test.c src/runtime.c -lm
./test
```

**Vers de l'assembleur x86 (mode pédagogique) :**

```bash
./bin/algi -a examples/test.algi
nasm -f elf32 -o test.o examples/test.algi.asm
gcc -m32 -o test test.o -lc
./test
```

> **Note.** Le mode assembleur (`-a`) est un générateur simplifié à but pédagogique. Il gère correctement les entiers (arithmétique, variables, boucles, conditions) et l'affichage de littéraux de chaîne, mais **ne prend pas en charge** la concaténation de chaînes (`+` entre chaînes), les flottants, ni les listes/objets. Pour un programme ALGI complet, utiliser le mode `-c` (génération C).

### 11.2. Scripts d'automatisation

Le dépôt fournit deux scripts shell qui encapsulent la construction du compilateur, la génération de code, la compilation native et l'exécution :

**`./algi` — script complet (build automatique, mode GDB, verbeux) :**

```bash
./algi -c examples/fibonacci.algi     # compiler vers C et exécuter
./algi -a examples/fibonacci.algi     # compiler vers ASM et exécuter
./algi -g examples/fibonacci.algi     # compiler vers C et exécuter sous GDB
./algi -v -c examples/fibonacci.algi  # mode verbeux (affiche les commandes)
./algi --clean                        # nettoyer les fichiers générés
```

**`./algi-compile` — script simplifié :**

```bash
./algi-compile c examples/test.algi   # compiler vers C
./algi-compile a examples/test.algi   # compiler vers assembleur
./algi-compile r test                 # exécuter un exécutable déjà produit
./algi-compile clean                  # nettoyer
```

### 11.3. Cibles Makefile utiles

```bash
make run-c            # compile examples/test.algi vers C et exécute
make run-asm           # compile examples/test.algi vers ASM et exécute
make run-gdb           # exécute sous GDB
make test               # exécute make sur test.algi
make test-all           # lance la suite de tests via ./test-all
make test-lexer         # teste l'analyseur lexical isolément (tests/test_lexer.algi)
make clean               # nettoie build/ et bin/
make clean-all           # nettoyage complet (y compris output/)
make analyze             # analyse statique via cppcheck (si installé)
make format              # formatage via clang-format (si installé)
make help                # affiche l'aide complète du Makefile
```

---

## 12. Exemples commentés

Le dossier `examples/` contient des programmes de complexité croissante, illustrant l'ensemble des fonctionnalités du langage :

| Fichier | Fonctionnalités illustrées |
|---|---|
| `bonjour.algi` | Programme minimal : affichage d'une chaîne littérale. |
| `variables.algi` | Déclarations, types inférés/annotés, affectations. |
| `boucles.algi` | Boucle `w` (while) et boucle `pou ... dans` (for-each). |
| `calculatrice.algi` | Entrées utilisateur (`keksie`), opérateurs arithmétiques, branchement conditionnel. |
| `fibonacci.algi` | Fonction récursive (`fonksion`), retour de valeur (`tem`). |
| `tri.algi` | Tri par sélection sur une liste saisie par l'utilisateur. |
| `tri_fusion.algi` | Tri fusion (merge sort) récursif complet, avec sous-listes temporaires (`bi[...]`) et fusion in-place. |
| `listes.algi` | Création, accès indexé, modification et parcours (`pou ... dans`) de listes hétérogènes ; tri natif (`nwagniehwe`). |
| `gestion_notes.algi` | Programme applicatif complet : saisie d'étudiants, calcul de moyennes, attribution de mentions, classement par tri par sélection — démontre la composition de fonctions, boucles, listes et concaténation de chaînes/nombres. |
| `alain.algi` | Exemple libre de l'auteur. |
| `test.algi` | Programme de référence utilisé par les cibles `make test` / `make run-c`. |

**Extrait — extrait de `gestion_notes.algi` (calcul de mention) :**

```algi
fonksion mention(m)
    ke (m >= 16)
        tem "Tres Bien"
    mi
    ke (m >= 14)
        tem "Bien"
    mi
    ke (m >= 10)
        tem "Passable"
    mi
    tem "Ajourne"
mi
```

---

## 13. Limitations connues

- **Backend assembleur restreint** : le générateur x86 (`-a`) ne supporte que les entiers, l'arithmétique de base, les boucles/conditions et l'affichage de littéraux de chaîne fixes. Les flottants, la concaténation de chaînes et les listes/objets ne sont pris en charge qu'en mode C (`-c`).
- **Cible 32 bits** : le mode assembleur requiert un environnement d'assemblage/édition de liens ELF 32 bits (`gcc-multilib`, `libc6-dev-i386` sur systèmes 64 bits), NASM installé.
- **Absence de ramasse-miettes** : le runtime C généré alloue dynamiquement chaque `AlgiValue`/`AlgiList`/`AlgiObject` sans collecteur automatique ; la gestion fine de la mémoire (au-delà de la durée de vie du processus) n'est pas garantie pour des programmes de longue durée ou à forte volumétrie.
- **Classes rudimentaires** : le support des classes (`kwouo`) et de l'instanciation (`feh`) reste minimal comparé à un modèle objet complet (pas d'héritage, de polymorphisme ni de visibilité des membres).
- **Diagnostic d'erreurs** : les messages d'erreur syntaxique/sémantique restent sommaires (ligne d'erreur, message générique), sans suggestions de correction ni récupération d'erreurs multiples dans une même passe.

---

## 14. Historique des correctifs majeurs

Le code source contient des commentaires substantiels documentant des corrections de bugs significatifs découverts lors d'un audit technique approfondi du compilateur, notamment :

- **Assignations postfixées** (`src/algi.y`, règle `postfix '=' expression`) : correction d'un bug où l'expression assignée (RHS) était écrasée par la cible d'affectation avant d'être stockée, du fait du partage de l'attribut `left` du nœud d'assignation ; la cible postfixe est désormais conservée dans `node->right`, préservant la RHS dans `node->left`.
- **Perte des paramètres de fonction** (`src/algi.y`, règle `function_def`) : la liste des paramètres analysée par le parser était auparavant ignorée lors de la construction du nœud `NODE_FUNCTION`, rendant inopérante toute fonction ALGI définie avec des arguments ; la liste est désormais correctement convertie et transmise à `create_function()`.
- **Blocs multi-instructions sans accolades** (`src/algi.y`, règle `block`) : la règle ne gérait auparavant qu'une seule instruction pour les blocs sans accolades (`{ }`), alors que la totalité des programmes ALGI du dépôt (écrits sans accolades) reposent sur plusieurs instructions entre un mot-clé structurel (`ke`, `w`, `pou`...) et `mi` ; la règle a été corrigée pour accepter une séquence d'instructions.
- **Écrasement mémoire par union (`ast.h`)** : les nœuds `NODE_FUNCTION` et `NODE_NEW_INSTANCE` écrivaient successivement, dans le même champ union, le nom (fonction/classe) puis les données d'arguments, provoquant la corruption du nom ; des champs `name` dédiés ont été ajoutés aux structures `data.function` et `data.call`.
- **Déclarations manquantes de fonctions du runtime** (`runtime.h`) : plusieurs fonctions de gestion des exceptions (`algi_push_try`, `algi_pop_try`, `algi_setjmp_try`, `algi_get_last_error`) étaient implémentées dans `runtime.c` sans prototype déclaré, entraînant des déclarations implicites dangereuses (notamment la troncature du pointeur retourné par `algi_get_last_error()` en `int`) ; les prototypes ont été ajoutés à l'en-tête.

> Une **révision technique complémentaire** a par ailleurs permis d'identifier et de corriger trois anomalies critiques d'exécution : un **débordement de tampon sur la pile** dans la fonction d'impression de l'AST, un problème d'**aliasing de registres qubits multi-registres** dans la génération de représentation intermédiaire (module de génération de code), ainsi qu'un **mécanisme de graine aléatoire (random seed) défectueux**. Ces trois correctifs ont été livrés avec une documentation dédiée et intégrés au Makefile via des cibles de test supplémentaires.

---

## 15. Outillage éditeur (extension VS Code)

Le dossier `vscode-algi/` fournit une **extension Visual Studio Code** dédiée au langage ALGI, comprenant :

- une **grammaire TextMate** (`syntaxes/algi.tmLanguage.json`) pour la coloration syntaxique des fichiers `.algi` ;
- une **configuration de langage** (`language-configuration.json`) : appariement des délimiteurs, commentaires ;
- un **thème d'icônes** dédié pour les fichiers et dossiers ALGI (`icons/`) ;
- un **script d'installation** (`install.sh`) automatisant le déploiement de l'extension dans VS Code.

Installation :

```bash
cd vscode-algi
chmod +x install.sh
./install.sh
```

---

## 16. Tests

| Cible / fichier | Portée |
|---|---|
| `tests/test_lexer.algi` + `make test-lexer` | Vérifie isolément le comportement de l'analyseur lexical sur un jeu de tokens représentatif. |
| `make test` | Compile et exécute `examples/test.algi` de bout en bout. |
| `./test-all` (via `make test-all`) | Exécute la suite de tests automatisés du dépôt. |
| `examples/*.algi` compilés en mode `-c` | Servent également de tests d'intégration de bout en bout (lexer → parser → AST → codegen C → exécution). |

---

## 17. Pistes d'évolution

- Ajout d'un système de types statique optionnel (vérification à la compilation plutôt qu'à l'exécution).
- Extension du backend assembleur à la concaténation de chaînes, aux flottants et aux listes.
- Introduction d'un ramasse-miettes (comptage de références ou marquage-balayage) pour le runtime C généré.
- Enrichissement du modèle objet (héritage, méthodes, visibilité).
- Amélioration de la récupération d'erreurs syntaxiques multiples en une seule passe de compilation.
- Élargissement du corpus lexical Bamiléké à des constructions supplémentaires (modules, importation, génériques).

---

## 18. Licence

Ce projet est développé dans un **cadre académique** (Master 1 Sécurité Informatique, Université de Yaoundé I) et est mis à disposition à des fins pédagogiques et de recherche. En l'absence de fichier de licence explicite dans le dépôt, tout usage réutilisant ce code hors du cadre académique devrait faire l'objet d'une clarification préalable auprès de l'auteur.

---

## 19. Auteur et remerciements

**Auteur :** Alain — Master 1 Sécurité Informatique, Université de Yaoundé I

Ce projet s'inscrit dans une démarche de valorisation du patrimoine linguistique Bamiléké à travers l'informatique, et dans une mise en pratique rigoureuse des techniques de compilation (analyse lexicale, analyse syntaxique LALR, construction d'AST, génération de code C et assembleur x86).