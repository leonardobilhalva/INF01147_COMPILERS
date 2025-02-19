CXX = g++
CXXFLAGS = -std=c++11 -Wall
BISON = bison
FLEX = flex
LEXER = lex.yy.c
PARSER = y.tab.c y.tab.h
OBJS = main.o y.tab.o lex.yy.o mapContainer.o ast.o semantic.o
TARGET = etapa4

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

lex.yy.c: scanner.l
	$(FLEX) -o $@ $^

y.tab.c y.tab.h: parser.y
	$(BISON) -d -o y.tab.c $^

main.o: main.cpp mapContainer.h y.tab.h ast.hh semantic.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

y.tab.o: y.tab.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

lex.yy.o: lex.yy.c y.tab.h mapContainer.h ast.hh semantic.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

mapContainer.o: mapContainer.cpp mapContainer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

ast.o: ast.cpp ast.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

semantic.o: semantic.cpp semantic.hh ast.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LEXER) y.tab.* $(TARGET) output.txt outnew.txt


test: ## to test semantics
	@echo "Running ./etapa4 with sample.txt -> output.txt to test semantics"
	./etapa4 sample.txt output.txt

test_e3: ## to test old etapa3 stuff
	@echo "Running ./etapa4 with sample.txt -> output.txt"
	./etapa4 sample.txt output.txt
	@echo "Running ./etapa4 with output.txt -> outnew.txt"
	./etapa4 output.txt outnew.txt
	@echo "Comparing output.txt and outnew.txt"
	diff output.txt outnew.txt && echo "The files are identical." || echo "The files differ."
