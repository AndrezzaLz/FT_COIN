# Define o compilador C++ a ser usado
CXX = g++
# Define as flags do compilador (padrão C++11, otimização -O2, e avisos -Wall)
CXXFLAGS = -std=c++11 -O2 -Wall
# Define as flags do linker (biblioteca do MariaDB)
LDFLAGS = -lmariadbcpp

# Nome do programa executável final
TARGET = FT_Coin

# O make encontrará automaticamente todos os arquivos .cpp no diretório
SOURCES = $(wildcard *.cpp)

# Gera automaticamente a lista de arquivos objeto (.o) a partir dos fontes (.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

#O que acontece quando você digita apenas "make")
all: $(TARGET)

# Regra para linkar e criar o executável final
# Ela depende de todos os arquivos objeto (.o) estarem prontos
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "Projeto $(TARGET) compilado com sucesso!"

# Regra para compilar cada arquivo .cpp em um arquivo .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa os arquivos  
clean:
	@echo "Limpando arquivos compilados..."
	rm -f $(TARGET) $(OBJECTS)

# Declara que 'all' e 'clean' não são nomes de arquivos
.PHONY: all clean