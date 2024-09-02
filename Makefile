all:
	clang -g -o game main.c -lraylib -lGL -lm

run: all
	./game