all: release debug

release:
	gcc -c src/*.c -I inc
	gcc -o svm *.o

debug:
	gcc -c src/*.c -I inc -Ddebug
	gcc -o svm-debug *.o