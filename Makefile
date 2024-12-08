# Compilador
CXX = g++

# Diretórios
SRC_DIR = .
BUILD_DIR = build

# Arquivos fontes e cabeçalhos
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
HEADERS = $(wildcard $(SRC_DIR)/*.h)

# Nome do programa final
EXEC = $(BUILD_DIR)/main.exe

# Bibliotecas
LIBS = -L. -lgcrypt

# Flags do compilador
CXXFLAGS = -Wall -Wextra -g --std=c++20
LDFLAGS = $(LIBS)

# Regra padrão
all: $(EXEC)

# Regra para gerar o executável
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Regra para compilar arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -rf $(BUILD_DIR)

# Rodando o programa
run:
	$(BUILD_DIR)/main.exe

.PHONY: all clean