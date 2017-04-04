main: Layer.o NNetwork.o
	g++ -Wall -I/usr/local/include -c main.cpp
	g++ -L/usr/local/lib main.o Layer.o -lgsl -lgslcblas -lm -ltinyxml -o main
	
Layer.o:
	g++ -Wall -I/usr/local/include -c Layer.cpp
	
NNetwork.o :
	g++ -Wall -I/usr/local/include -c NNetwork.cpp
	
clean :
	rm *.o
