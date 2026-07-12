# Makefile pour le compilateur ALGI
# Utilise flex, bison, gcc

# ===== VARIABLES =====
CC = gcc
CFLAGS = -Wall -g -I./src -D_GNU_SOURCE
LDFLAGS = -lm

LEX = flex
YACC = bison
YFLAGS = -d -v -t

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
OUTPUT_DIR = output
RUNTIME_DIR = runtime

# Fichiers sources
LEX_SRC = $(SRC_DIR)/algi.l
YACC_SRC = $(SRC_DIR)/algi.y
AST_SRC = $(SRC_DIR)/ast.c
SYMBOL_SRC = $(SRC_DIR)/symbol.c
CODEGEN_SRC = $(SRC_DIR)/codegen.c
CODEGEN_ASM_SRC = $(SRC_DIR)/codegen_asm.c
RUNTIME_SRC = $(SRC_DIR)/runtime.c

# Fichiers objets
LEX_OBJ = $(BUILD_DIR)/lex.yy.o
YACC_OBJ = $(BUILD_DIR)/algi.tab.o
AST_OBJ = $(BUILD_DIR)/ast.o
SYMBOL_OBJ = $(BUILD_DIR)/symbol.o
CODEGEN_OBJ = $(BUILD_DIR)/codegen.o
CODEGEN_ASM_OBJ = $(BUILD_DIR)/codegen_asm.o
RUNTIME_OBJ = $(BUILD_DIR)/runtime.o

OBJECTS = $(LEX_OBJ) $(YACC_OBJ) $(AST_OBJ) $(SYMBOL_OBJ) $(CODEGEN_OBJ) $(CODEGEN_ASM_OBJ) $(RUNTIME_OBJ)

# Cible principale
TARGET = $(BIN_DIR)/algi

# ===== RÈGLES =====
all: directories $(TARGET) scripts

directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(OUTPUT_DIR)
	@echo "📁 Répertoires créés: $(BUILD_DIR), $(BIN_DIR), $(OUTPUT_DIR)"

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✅ Compilateur ALGI créé: $(TARGET)"

# ===== COMPILATION FLEX =====
$(LEX_OBJ): $(LEX_SRC) $(BUILD_DIR)/algi.tab.h
	$(LEX) -o $(BUILD_DIR)/lex.yy.c $<
	$(CC) $(CFLAGS) -c -o $@ $(BUILD_DIR)/lex.yy.c

# ===== COMPILATION BISON =====
$(YACC_OBJ): $(BUILD_DIR)/algi.tab.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/algi.tab.c $(BUILD_DIR)/algi.tab.h: $(YACC_SRC)
	$(YACC) $(YFLAGS) -o $(BUILD_DIR)/algi.tab.c $<

# ===== COMPILATION DES MODULES =====
$(AST_OBJ): $(SRC_DIR)/ast.c $(SRC_DIR)/ast.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(SYMBOL_OBJ): $(SRC_DIR)/symbol.c $(SRC_DIR)/symbol.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(CODEGEN_OBJ): $(SRC_DIR)/codegen.c $(SRC_DIR)/codegen.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(CODEGEN_ASM_OBJ): $(SRC_DIR)/codegen_asm.c $(SRC_DIR)/codegen_asm.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(RUNTIME_OBJ): $(SRC_DIR)/runtime.c $(SRC_DIR)/runtime.h
	$(CC) $(CFLAGS) -c -o $@ $<

# ===== SCRIPTS =====
scripts:
	@chmod +x algi algi-compile test-all 2>/dev/null || true
	@echo "✅ Scripts rendus exécutables"

# ===== NETTOYAGE =====
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -f $(TARGET)
	@echo "🧹 Nettoyage terminé"

clean-all: clean
	rm -rf $(OUTPUT_DIR)
	rm -f examples/*.{c,o,asm,exe,out}
	rm -f *.c *.asm *.o *.exe
	@echo "🧹 Nettoyage avancé terminé"

# ===== EXÉCUTION =====
run: $(TARGET)
	./$(TARGET) examples/test.algi

# ===== EXÉCUTION AVEC LES SCRIPTS =====
run-c: scripts
	./algi -c examples/test.algi

run-asm: scripts
	./algi -a examples/test.algi

run-gdb: scripts
	./algi -g examples/test.algi

run-verbose: scripts
	./algi -v -c examples/test.algi

# ===== COMPILATION DES EXEMPLES =====
examples:
	@mkdir -p examples
	@echo "📁 Dossier examples créé"

example-c: $(TARGET)
	@echo "📝 Génération du code C..."
	./$(TARGET) -c examples/test.algi
	@if [ -f examples/test.c ]; then \
		mv examples/test.c $(OUTPUT_DIR)/test.c 2>/dev/null || true; \
	fi
	@if [ -f test.c ]; then \
		mv test.c $(OUTPUT_DIR)/test.c 2>/dev/null || true; \
	fi
	@echo "✅ Code C généré: $(OUTPUT_DIR)/test.c"

example-asm: $(TARGET)
	@echo "📝 Génération du code assembleur..."
	./$(TARGET) -a examples/test.algi
	@if [ -f examples/test.asm ]; then \
		mv examples/test.asm $(OUTPUT_DIR)/test.asm 2>/dev/null || true; \
	fi
	@if [ -f test.asm ]; then \
		mv test.asm $(OUTPUT_DIR)/test.asm 2>/dev/null || true; \
	fi
	@echo "✅ Code assembleur généré: $(OUTPUT_DIR)/test.asm"

example-run: example-c
	@echo "🔨 Compilation du code C..."
	$(CC) $(CFLAGS) -o $(OUTPUT_DIR)/test_run $(OUTPUT_DIR)/test.c $(RUNTIME_SRC) $(LDFLAGS)
	@echo "🚀 Exécution..."
	./$(OUTPUT_DIR)/test_run

example-asm-run: example-asm
	@echo "🔨 Assemblage du code ASM..."
	@if command -v nasm >/dev/null 2>&1; then \
		nasm -f elf32 -o $(OUTPUT_DIR)/test.o $(OUTPUT_DIR)/test.asm; \
		gcc -m32 -o $(OUTPUT_DIR)/test_asm $(OUTPUT_DIR)/test.o -lc; \
		echo "🚀 Exécution..."; \
		./$(OUTPUT_DIR)/test_asm; \
	else \
		echo "⚠️ NASM n'est pas installé. Ignoré."; \
	fi

# ===== TESTS =====
test: $(TARGET)
	@echo "🧪 Exécution des tests..."
	./$(TARGET) examples/test.algi

test-all: scripts
	@echo "🧪 Exécution de la suite de tests..."
	./test-all

test-lexer:
	@echo "🧪 Test du lexer..."
	@if [ -f tests/test_lexer.algi ]; then \
		./$(TARGET) tests/test_lexer.algi 2>&1 | head -20; \
	else \
		echo "⚠️ Fichier test_lexer.algi non trouvé"; \
	fi


# ===== INSTALLATION =====
install: $(TARGET)
	@echo "📦 Installation du compilateur..."
	@mkdir -p /usr/local/bin
	@cp $(TARGET) /usr/local/bin/algi
	@chmod +x /usr/local/bin/algi
	@echo "✅ Compilateur installé dans /usr/local/bin/algi"

uninstall:
	@echo "🗑️ Désinstallation du compilateur..."
	@rm -f /usr/local/bin/algi
	@echo "✅ Compilateur désinstallé"

# ===== GÉNÉRATION DE DOCUMENTATION =====
doc:
	@echo "📚 Génération de la documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile 2>/dev/null || echo "⚠️ Doxyfile non trouvé"; \
	else \
		echo "⚠️ Doxygen n'est pas installé"; \
	fi

# ===== FORMATAGE DU CODE =====
format:
	@echo "🎨 Formatage du code..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i src/*.c src/*.h 2>/dev/null || true; \
		echo "✅ Code formaté"; \
	else \
		echo "⚠️ clang-format n'est pas installé"; \
	fi

# ===== ANALYSE STATIQUE =====
analyze:
	@echo "🔍 Analyse statique du code..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all src/ 2>&1 | head -50; \
	else \
		echo "⚠️ cppcheck n'est pas installé"; \
	fi

# ===== AIDE =====
help:
	@echo "📖 Aide du Makefile ALGI"
	@echo ""
	@echo "🔧 COMPILATION:"
	@echo "  make           - Compile le compilateur ALGI"
	@echo "  make clean     - Nettoie les fichiers de compilation"
	@echo "  make clean-all - Nettoyage complet (y compris output/)"
	@echo "  make install   - Installe le compilateur"
	@echo "  make uninstall - Désinstalle le compilateur"
	@echo ""
	@echo "🚀 EXÉCUTION (avec scripts):"
	@echo "  make scripts   - Rend les scripts exécutables"
	@echo "  make run-c     - Compile test.algi vers C et exécute"
	@echo "  make run-asm   - Compile test.algi vers ASM et exécute"
	@echo "  make run-gdb   - Compile vers C et exécute avec GDB"
	@echo "  make run-verbose - Compile avec affichage détaillé"
	@echo ""
	@echo "📝 EXEMPLES:"
	@echo "  make bonjour      - Compile et exécute bonjour.algi"
	@echo "  make calculatrice - Compile et exécute calculatrice.algi"
	@echo "  make fibonacci    - Compile et exécute fibonacci.algi"
	@echo "  make boucles      - Compile et exécute boucles.algi"
	@echo "  make variables    - Compile et exécute variables.algi"
	@echo ""
	@echo "🧪 TESTS:"
	@echo "  make test      - Exécute les tests simples"
	@echo "  make test-all  - Exécute tous les tests automatiquement"
	@echo "  make test-lexer - Teste le lexer"
	@echo ""
	@echo "📚 AUTRES:"
	@echo "  make doc       - Génère la documentation"
	@echo "  make format    - Formate le code source"
	@echo "  make analyze   - Analyse statique du code"
	@echo "  make help      - Affiche cette aide"

.PHONY: all clean clean-all run run-c run-asm run-gdb run-verbose \
        examples example-c example-asm example-run example-asm-run \
        test test-all test-lexer \
        bonjour calculatrice fibonacci boucles variables \
        install uninstall doc format analyze \
        scripts help directories