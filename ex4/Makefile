# MAKEFILE ES4

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
LIBS_SRCS = $(SRC_DIR)/graph.c \
            $(SRC_DIR)/breadthFirstVisit.c \
            $(SRC_DIR)/csvParser.c \
            $(EX2_SRC_DIR)/hashTable.c

# Unity framework
UNITY_SRCS = $(UNITY_DIR)/unity.c

# Eseguibili
MAIN_TARGET = $(BIN_DIR)/mainEx4
TEST_TARGET = $(BIN_DIR)/testEx4

# Regola di default: compila entrambi gli eseguibili
all: $(MAIN_TARGET) $(TEST_TARGET)

# Creazione della directory bin
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Creazione della directory data (se non esiste)
$(DATA_DIR):
	mkdir -p $(DATA_DIR)

# Compilazione main_ex4
$(MAIN_TARGET): $(LIBS_SRCS) $(TEST_DIR)/mainEx4.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(EX2_INC_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ main_ex4 compilato con successo"

# Compilazione test_ex4 (include Unity)
$(TEST_TARGET): $(LIBS_SRCS) $(UNITY_SRCS) $(TEST_DIR)/testEx4.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(EX2_INC_DIR) -I$(TEST_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ test_ex4 compilato con successo"

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
	@echo "  ./bin/mainEx4 <file_csv> <città_partenza> <file_output>"
	@echo ""
	@echo "Dove:"
	@echo "  - file_csv: percorso del file CSV con i dati del grafo"
	@echo "  - città_partenza: nome della città da cui iniziare la BFS"
	@echo "  - file_output: file dove salvare i risultati della visita"
	@echo ""
	@echo "Esempi:"
	@echo "  ./bin/mainEx4 data/italian_dist_graph.csv torino output.txt"
	@echo "  ./bin/mainEx4 data/italian_dist_graph.csv milano risultati.txt"
	@echo "  ./bin/mainEx4 data/italian_dist_graph.csv roma cities.txt"
	@echo ""
	@echo "NOTA: La ricerca della città ignora maiuscole/minuscole"

# Lista comandi
help:
	@echo "Target disponibili:"
	@echo "  make all    - Compila tutti gli eseguibili"
	@echo "  make clean  - Rimuove la directory /bin"
	@echo "  make test   - Compila ed esegue i test"
	@echo "  make howto  - Guida per l'uso"
	@echo "  make help   - Mostra questo messaggio"

.PHONY: all clean test howto help