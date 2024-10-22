# List of all source files
SOURCES = main.cpp mapContainer.cpp y.tab.cpp
# Generate object files from the source files
OBJECTS = $(SOURCES:.cpp=.o)
# Compilation flags from teacher
CXXFLAGS = -std=c++11 -Wall

etapa2: lex.yy.o $(OBJECTS)
	g++ -o etapa2 lex.yy.o $(OBJECTS) $(CXXFLAGS)

# lex depends on parser
lex.yy.cpp: scanner.l y.tab.hpp
	flex --c++ -o lex.yy.cpp scanner.l

# -d generates header(hpp
y.tab.cpp y.tab.hpp: parser.y
	bison -d --language=c++ -o y.tab.cpp parser.y

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f lex.yy.cpp lex.yy.o y.tab.cpp y.tab.hpp stack.hh $(OBJECTS) etapa2
	
test: $(EXECUTABLE)
	@echo "Running all tests in the e1 directory..."
	@for test_script in tests/e1/*.sh; do \
	    bash $$test_script; \
	done
