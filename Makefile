.PHONY: all

all: compile

compile:
	gcc main.c src/*.c -g -g3 -ggdb

debug:
	gcc main.c src/*.c -g -g3 -ggdb -fsanitize=address
