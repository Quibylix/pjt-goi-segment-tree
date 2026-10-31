CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2 -g
SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

TARGET := segment_tree_runner
SRC := st_test.cpp segment_tree.cpp
HDR := segment_tree.hpp

.PHONY: all test clean debug valgrind help

all: test

test: $(TARGET)
	@echo "==> Ejecutando suite de pruebas competitivas..."
	./$(TARGET)

$(TARGET): $(SRC) $(HDR)
	@echo "==> Compilando $(TARGET) con ASan y UBSan habilitados..."
	$(CXX) $(CXXFLAGS) $(SAN_FLAGS) $(SRC) -o $(TARGET)

debug: CXXFLAGS += -O0
debug: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)_debug
	@echo "==> Binario de depuración creado: $(TARGET)_debug"

valgrind: $(SRC) $(HDR)
	$(CXX) -std=c++17 -Wall -Wextra -O2 -g $(SRC) -o $(TARGET)_valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)_valgrind
	@rm -f $(TARGET)_valgrind

clean:
	@echo "==> Limpiando ejecutables generados..."
	rm -f $(TARGET) $(TARGET)_debug $(TARGET)_valgrind

help:
	@echo "Comandos disponibles:"
	@echo "  make        - Compila y corre la suite con ASan y UBSan"
	@echo "  make test   - Idem 'make'"
	@echo "  make debug  - Genera binario -O0 para depurador gdb"
	@echo "  make valgrind - Ejecuta bajo Valgrind Memcheck"
	@echo "  make clean  - Elimina los binarios"
