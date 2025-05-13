# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src/models -I./src/utils -I./src/construction

# Diretórios
SRC_DIR = src
MODEL_DIR = $(SRC_DIR)/models
UTILS_DIR = $(SRC_DIR)/utils
CONSTRUCTION_DIR = $(SRC_DIR)/construction

# Fontes e objetos
SRCS = $(SRC_DIR)/main.cpp $(MODEL_DIR)/Solution.cpp $(UTILS_DIR)/Data.cpp $(CONSTRUCTION_DIR)/Construction.cpp
OBJS = $(SRCS:.cpp=.o)

# Nome do executável
TARGET = main

# Regra padrão
all: $(TARGET)

# Linkagem do executável e remoção dos objetos
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
	@rm -f $(OBJS)

# Compilação dos objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza do executável
clean:
	rm -f $(TARGET)

# Recompila tudo
rebuild: clean all
