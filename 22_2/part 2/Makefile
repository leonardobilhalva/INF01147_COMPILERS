# Leonardo Bilhalva - 315768 | Artur Turatti - 304740
# List of all source files
SOURCES = main.cpp mapContainer.cpp
# Generate object files from the source files
OBJECTS = $(SOURCES:.cpp=.o)
# Compilation flags
CXXFLAGS = -std=c++11 -Wall

etapa1: lex.yy.o $(OBJECTS)
	g++ -o etapa1 lex.yy.o $(OBJECTS) $(CXXFLAGS)

lex.yy.cpp: scanner.l
	flex --c++ -o lex.yy.cpp scanner.l

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f lex.yy.cpp lex.yy.o $(OBJECTS) etapa1

test: $(EXECUTABLE)
	@echo "Running all tests in the e1 directory..."
	@for test_script in tests/e1/*.sh; do \
	    bash $$test_script; \
	done