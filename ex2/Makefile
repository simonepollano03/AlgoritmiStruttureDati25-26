# MAKEFILE ES2

# Compiler e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -g
LDFLAGS = -lm

# Directory
SRC_DIR  = src
INC_DIR  = include
BIN_DIR  = bin
TEST_DIR = test
DATA_DIR = data
UNITY_DIR = $(TEST_DIR)/unity

# File sorgenti libreria
LIBS_SRCS = $(SRC_DIR)/hashTable.c

# Unity framework
UNITY_SRCS = $(UNITY_DIR)/unity.c

# Eseguibili
MAIN_TARGET = $(BIN_DIR)/mainEx2
TEST_TARGET = $(BIN_DIR)/testEx2

# Regola di default: compila entrambi gli eseguibili
all: $(MAIN_TARGET) $(TEST_TARGET)

# Creazione della directory bin
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Creazione della directory data (se non esiste)
$(DATA_DIR):
	mkdir -p $(DATA_DIR)

# Compilazione main_ex2
$(MAIN_TARGET): $(LIBS_SRCS) $(TEST_DIR)/mainEx2.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ mainEx2 compilato con successo"

# Compilazione test_ex2 (include Unity)
$(TEST_TARGET): $(LIBS_SRCS) $(UNITY_SRCS) $(TEST_DIR)/testEx2.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(TEST_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ testEx2 compilato con successo"

# Pulizia totale (rimuove bin ma mantiene iliade.txt in data)
clean:
	rm -rf $(BIN_DIR)
	@echo "✓ Pulizia completata"

# Esegui i test
test: $(TEST_TARGET)
	@echo ""
	@echo "=== ESECUZIONE TEST AUTOMATICI ==="
	@echo ""
	./$(TEST_TARGET)

# Come runnare l'esercizio
howto:
	@echo "=== Esecuzione del programma principale ==="
	@echo "  ./bin/main_ex2 data/iliade.txt <lunghezza_minima>"
	@echo ""
	@echo "Dove:"
	@echo "  - lunghezza_minima: lunghezza minima delle parole da considerare (n > 0)"
	@echo ""
	@echo "Esempi:"
	@echo "  ./bin/mainEx2 data/iliade.txt 6"
	@echo "  ./bin/mainEx2 data/iliade.txt 10"
	@echo ""
	@echo "Il programma stamperà:"
	@echo "  - Il numero di parole uniche trovate"
	@echo "  - La parola più frequente con lunghezza >= soglia"
	@echo "  - Il numero di occorrenze di quella parola"

# Lista comandi
help:
	@echo "Target disponibili:"
	@echo "  make all    - Compila tutti gli eseguibili"
	@echo "  make clean  - Rimuove la directory /bin"
	@echo "  make test   - Compila ed esegue i test"
	@echo "  make howto  - Guida per l'uso"
	@echo "  make help   - Mostra questo messaggio"

.PHONY: all clean test howto help