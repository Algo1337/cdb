.PHONY: all

all: compile test lib

compile:
	gcc cli.c -o cli src/*.c -g -g3 -ggdb

lib:
	gcc -c src/*.c
	ar rcs cdb.a *.o; rm *.o
	mv cdb.a /usr/local/lib/libcdb.a
	cp cli /bin/

test:
	gcc tests/create.c -o create src/*.c -g -g3 -ggdb && ./create test
	gcc tests/view.c -o view src/*.c -g -g3 -ggdb && ./view test

debug:
	gcc tests/view.c -o view src/*.c -g -g3 -ggdb -fsanitize=address
	gcc tests/create.c -o create src/*.c -g -g3 -ggdb -fsanitize=address && sudo ./create

clean:
	rm -rf create
	rm -rf view
	rm -rf cli