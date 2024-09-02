all:
	clang -g -o game -lraylib main.c

run: all
	./game