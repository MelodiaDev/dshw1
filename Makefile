FLAGS=-lncurses
main: main.c editor.o ui.o container.o

container.o: container.h

.PHONY: clean
clean:
	rm -rf editor.o ui.o container.o main
