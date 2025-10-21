# MAKEFILE ES1

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

# File sorgenti libreria
LIBS_SRCS = $(SRC_DIR)/selectionSort.c 	\
			$(SRC_DIR)/quickSort.c 		\
			$(SRC_DIR)/hybridSort.c		\
			$(SRC_DIR)/recordUtils.c

# Eseguibili
MAIN_TARGET = $(BIN_DIR)/mainEx1
TEST_TARGET = $(BIN_DIR)/testEx1

# Regola di default: compila con entrambi gli eseguibili
all: $(MAIN_TARGET) $(TEST_TARGET)

# Creazione della directory bin
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Creazione della directory data (se non esiste)
$(DATA_DIR):
	mkdir -p $(DATA_DIR)

# Compilazione mainEx1
$(MAIN_TARGET): $(LIBS_SRCS) $(TEST_DIR)/mainEx1.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ mainEx1 compilato con successo"

# Compilazione testEx1
$(TEST_TARGET): $(LIBS_SRCS) $(TEST_DIR)/testEx1.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^ $(LDFLAGS)
	@echo "✓ testEx1 compilato con successo"


# Pulizia totale (rimuove anche i file .bin generati)
clean:
	rm -rf $(BIN_DIR)
	rm -f $(DATA_DIR)/*.bin $(DATA_DIR)/sorted_*.bin
	@echo "✓ Pulizia totale completata"


# Esegui i test
test: $(TEST_TARGET)
	@echo ""
	@echo "=== ESECUZIONE TEST AUTOMATICI ==="
	@echo ""
	./$(TEST_TARGET)

# Come runnare l'esercizio
howto:
	@echo "=== Esecuzione del programma principale ==="
	@echo "  - ./bin/mainEx1 data/records.bin data/<output_file> <field> <k> > risultati.txt 2>&1"
	@echo ""
	@echo "Dove:"
	@echo "  - output_file può essere:"
	@echo "      data/sorted_id.bin"
	@echo "      data/sorted_field1.bin"
	@echo "      data/sorted_field2.bin"
	@echo "      data/sorted_field3.bin"
	@echo ""
	@echo "  - field: 1=id, 2=field1, 3=field2, 4=field3"
	@echo "  - k: soglia per algoritmo ibrido (0, 10, 50, 100, 500, 1000, 5000, 10000, 1000000)"
	@echo ""
	@echo "Esempi:"
	@echo "  ./bin/mainEx1 data/records.bin data/sorted_id.bin 1 50"
	@echo "  ./bin/mainEx1 data/records.bin data/sorted_field1.bin 2 100"
	@echo ""
	@echo "Per salvare i risultati in un file:"
	@echo "  ./bin/mainEx1 data/records.bin data/output.bin 1 50 > risultati.txt 2>&1"
	@echo ""
	@echo "Per i risultati:"
	@echo "  hexdump -C data/sorted_id.bin | head -n 50"


# Lista comandi
help:
	@echo "Target disponibili:"
	@echo "  make all		- Compila tutti gli eseguibili"
	@echo "  make clean 	        - Rimuove la directory /bin e i file .bin in data/"
	@echo "  make test		- Compila ed esegue i test"
	@echo "  make howto		- Guida per l'uso"
	@echo "  make help		- Mostra questo messaggio"

.PHONY: all clean test howto help
