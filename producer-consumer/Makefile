all: prodcon

prodcon: prodcon.o helper.o tands.o
	g++ prodcon.o -o prodcon -O -lpthread

prodcon.o: prodcon.cpp
	g++ -c -Wall prodcon.cpp -o prodcon.o

helper.o: helper.cpp
	g++ -c helper.cpp -o helper.o

tands.o: tands.cpp
	g++ -c tands.cpp -o tands.o

clean:
	rm *.o prodcon