mytiny2c: mytiny.o mytiny_tree.o mytiny_parse.o mytiny_lex.o
	gcc -o mytiny2c mytiny.o mytiny_tree.o mytiny_parse.o mytiny_lex.o

debug:
	bison -d --report=all -o tiny_parse.c tiny_parse.y

mytiny_parse.h:	mytiny_parse.y
	bison -d -o mytiny_parse.c mytiny_parse.y

mytiny_parse.c:	mytiny_parse.y
	bison -d -o mytiny_parse.c mytiny_parse.y

mytiny_parse.o:	mytiny_parse.c mytiny.h mytiny_parse.h mytiny_tree.h
	gcc -c -o mytiny_parse.o mytiny_parse.c

mytiny_lex.c:	mytiny_lex.l
	flex -omytiny_lex.c mytiny_lex.l

mytiny_lex.o:	mytiny_lex.c mytiny.h mytiny_parse.h mytiny_tree.h
	gcc -c -o mytiny_lex.o mytiny_lex.c

mytiny_tree.o: mytiny_tree.c mytiny_tree.h mytiny.h
	gcc -c -o mytiny_tree.o mytiny_tree.c

mytiny.o:	mytiny.c mytiny.h mytiny_parse.h mytiny_tree.h
	gcc -c -o mytiny.o mytiny.c

circle: circle.c
	gcc -o circle circle.c

circle.c: circle.t
	./mytiny2c circle.t

test: test.c
	gcc -o test test.c

test.c: test.t
	./mytiny2c test.t

clean:
	rm parse mytiny.o mytiny_tree.o mytiny_parse.o mytiny_parse.c mytiny_parse.h mytiny_lex.c mytiny_lex.o

