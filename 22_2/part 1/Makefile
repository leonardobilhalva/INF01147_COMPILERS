# List of all source files
SOURCES = main.cpp mapContainer.cpp
# Generate object files from the source files
OBJECTS = $(SOURCES:.cpp=.o)

etapa1: lex.yy.o $(OBJECTS)
	g++ -o etapa1 lex.yy.o $(OBJECTS) -ll

lex.yy.cc: scanner.l
	flex --c++ scanner.l

lex.yy.o: lex.yy.cc
	g++ -c lex.yy.cc -o lex.yy.o

%.o: %.cpp
	g++ -c $< -o $@

clean:
	rm -f lex.yy.cc lex.yy.o $(OBJECTS) etapa1

test: $(EXECUTABLE)
	@echo "Running all tests in the e1 directory..."
	@for test_script in tests/e1/*.sh; do \
	    bash $$test_script; \
	done
