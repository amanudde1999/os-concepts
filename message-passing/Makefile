all: client server

client: client.cpp tands.cpp
	g++ client.cpp tands.cpp -o client -O
	groff -Tpdf -man client.man > client.pdf

	
server: server.cpp tands.cpp
	g++ server.cpp tands.cpp -o server -O
	groff -Tpdf -man server.man > server.pdf
tands.o: tands.cpp
	g++ -c tands.cpp -o tands.o

clean:
	rm client server client.pdf server.pdf