# Compilador e flags de compilação
CXX = g++

# Diretórios de onde buscar os cabeçalhos (.hpp)
INCLUDE_DIRS = -I$(SRC_DIR)/models \
               -I$(SRC_DIR)/utils \
               -I$(SRC_DIR)/construction \
               -I$(SRC_DIR)/localSearch \
               -I$(SRC_DIR)/metaheuristic

# Flags do compilador
# -O3 para otimização máxima
CXXFLAGS = -std=c++17 -Wall -Wextra $(INCLUDE_DIRS) -O3

# Adicione -lrt para clock_gettime em alguns sistemas Linux
# Adicionado como comentário, pois muitas distribuições modernas não exigem mais.
LDFLAGS = #-lrt

# Diretório de origem principal
SRC_DIR = src

# Lista de todos os arquivos fonte .cpp
# Usando a função wildcard para encontrar todos os .cpp automaticamente
# Isso é mais robusto do que listar cada arquivo manualmente
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/models/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp) \
       $(wildcard $(SRC_DIR)/construction/*.cpp) \
       $(wildcard $(SRC_DIR)/localSearch/*.cpp) \
       $(wildcard $(SRC_DIR)/metaheuristic/*.cpp)

# Gera a lista de arquivos objeto (.o) a partir dos arquivos fonte (.cpp)
OBJS = $(SRCS:.cpp=.o)

# Nome do executável final
TARGET = main

# --- Regras ---

# Regra padrão: compila o alvo principal
all: $(TARGET)

# Regra de linkagem: cria o executável a partir dos arquivos objeto
$(TARGET): $(OBJS)
	@echo "Linkando para criar o executável: $@"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso."

# Regra de compilação: transforma qualquer arquivo .cpp em .o
# Esta regra genérica lida com todos os subdiretórios
%.o: %.cpp
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra de limpeza: remove o executável e todos os arquivos objeto
clean:
	@echo "Limpando arquivos gerados..."
	rm -f $(TARGET) $(OBJS)
	@echo "Limpeza concluída."

# Regra para reconstruir tudo do zero
rebuild: clean all

# Declara regras que não representam arquivos
.PHONY: all clean rebuild
