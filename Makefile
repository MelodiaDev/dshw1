#FLAGS=-lncurses
#main: main.c editor.o ui.o container.o

test: test.o container.o
test.o: container.h
container.o: container.h

.PHONY: clean
clean:
	rm -rf editor.o ui.o container.o main test.o
