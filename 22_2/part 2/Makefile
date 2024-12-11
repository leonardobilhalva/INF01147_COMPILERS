# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Bison e Flex
BISON = bison
FLEX = flex

# Arquivos fonte e gerados
LEXER = lex.yy.c
PARSER = y.tab.c y.tab.h
OBJS = main.o y.tab.o lex.yy.o mapContainer.o
TARGET = etapa2

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

# Gera o lex.yy.c a partir do scanner.l
lex.yy.c: scanner.l
	$(FLEX) -o $@ $^

# Gera y.tab.c e y.tab.h a partir do parser.y
y.tab.c y.tab.h: parser.y
	$(BISON) -d -o y.tab.c $^

# Compila main.cpp e garante que y.tab.h foi gerado antes
main.o: main.cpp mapContainer.h y.tab.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compila y.tab.c
y.tab.o: y.tab.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compila lex.yy.c e garante que y.tab.h foi gerado antes
lex.yy.o: lex.yy.c y.tab.h mapContainer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compila mapContainer.cpp
mapContainer.o: mapContainer.cpp mapContainer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LEXER) y.tab.* $(TARGET)
