all: shell379

shell379: shell379.o parser.o cmdexecuter.o
	g++ shell379.o parser.o cmdexecuter.o -o shell379 -g

shell379.o: main.cpp cmdexecuter.cpp parser.cpp
	g++ -c -Wall main.cpp -o shell379.o

parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o

cmdexecuter.o: cmdexecuter.cpp
	g++ -c cmdexecuter.cpp -o cmdexecuter.o

clean:
	rm *.o shell379