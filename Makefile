# Diretórios
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Nome do executável
TARGET = $(BIN_DIR)/main

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INC_DIR)

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Objetos correspondentes
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Regra principal
all: $(TARGET)

# Linka os objetos
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compila cada arquivo .cpp para .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria diretório bin se não existir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpa binários
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
