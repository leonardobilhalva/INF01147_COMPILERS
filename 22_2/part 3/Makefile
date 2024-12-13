CXX = g++
CXXFLAGS = -std=c++11 -Wall
BISON = bison
FLEX = flex
LEXER = lex.yy.c
PARSER = y.tab.c y.tab.h
OBJS = main.o y.tab.o lex.yy.o mapContainer.o ast.o
TARGET = etapa3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

lex.yy.c: scanner.l
	$(FLEX) -o $@ $^

y.tab.c y.tab.h: parser.y
	$(BISON) -d -o y.tab.c $^

main.o: main.cpp mapContainer.h y.tab.h ast.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

y.tab.o: y.tab.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.o: lex.yy.c y.tab.h mapContainer.h ast.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

mapContainer.o: mapContainer.cpp mapContainer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

ast.o: ast.cpp ast.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LEXER) y.tab.* $(TARGET)
