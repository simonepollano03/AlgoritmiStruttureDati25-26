# MAKEFILE ES3

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

# Directory esercizio 2 (per hash table)
EX2_SRC_DIR = ../ex2/src
EX2_INC_DIR = ../ex2/include

# File sorgenti libreria
LIBS_SRCS = $(SRC_DIR)/priorityQueue.c \
            $(SRC_DIR)/scheduler.c \
            $(EX2_SRC_DIR)/hashTable.c

# Unity framework
UNITY_SRCS = $(UNITY_DIR)/unity.c

# Eseguibili
MAIN_TARGET = $(BIN_DIR)/mainEx3
TEST_TARGET = $(BIN_DIR)/testEx3

# Regola di default: compila entrambi gli eseguibili
all: $(MAIN_TARGET) $(TEST_TARGET)

# Creazione della directory bin
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilazione main_ex4
$(MAIN_TARGET): $(LIBS_SRCS) $(TEST_DIR)/mainEx3.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(EX2_INC_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ mainEx3 compilato con successo"

# Compilazione test_ex4 (include Unity)
$(TEST_TARGET): $(LIBS_SRCS) $(UNITY_SRCS) $(TEST_DIR)/testEx3.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(EX2_INC_DIR) -I$(TEST_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ testEx3 compilato con successo"

# Pulizia totale (rimuove bin ma mantiene italian_dist_graph.csv in data)
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
	@echo "  ./bin/mainEx3 <file_input> <file_output> "
	@echo ""
	@echo "Dove:"
	@echo "  - file_input: file CSV con i task  (Id, Start, Length, Priority)"
	@echo "  - file_output: file CSV per i risultati  (Id, Start, StartTime, EndTime)"
	@echo ""
	@echo "Esempi:"
	@echo "  ./bin/mainEx3 data/tasks.csv data/output.csv"
	@echo ""
	@echo "NOTA: Il programma simula uno scheduler non-preemptive con priorità"
	@echo "	     utilizzando una Priority Queue basata su heap ternario."

# Lista comandi
help:
	@echo "Target disponibili:"
	@echo "  make all    - Compila tutti gli eseguibili"
	@echo "  make clean  - Rimuove la directory /bin"
	@echo "  make test   - Compila ed esegue i test"
	@echo "  make howto  - Guida per l'uso"
	@echo "  make help   - Mostra questo messaggio"

.PHONY: all clean test howto help