etapa5: main.o lex.yy.o y.tab.o hash.o semantic.o ast.o tac.o
	gcc main.o lex.yy.o y.tab.o hash.o semantic.o ast.o tac.o -o etapa5

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

hash.o: hash.c
	gcc -c hash.c

semantic.o: semantic.c
	gcc -c semantic.c

tac.o: tac.c
	gcc -c tac.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.c: parser.y
	yacc -d parser.y
	
lex.yy.c: scanner.l 
	lex scanner.l
	
clean: 
	-rm lex.yy.c y.tab.c *.o etapa5
