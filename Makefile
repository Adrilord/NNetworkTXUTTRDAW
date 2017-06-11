DEBUGMODE = -g # --debug optionel

main: Layer.o NNetwork.o xmlparser.o iomanager.o
	g++ -std=c++11 -Wall $(DEBUGMODE) -I/usr/local/include -c main.cpp
	g++ -std=c++11 -L/usr/local/lib -lgsl -lgslcblas -lm -ltinyxml xmlparser.o main.o Layer.o NNetwork.o iomanager.o -o main

iomanager.o :
	g++ -std=c++11 -Wall $(DEBUGMODE)  -I/usr/local/include -c iomanager.cpp

xmlparser.o : NNetwork.o Layer.o
	g++ -std=c++11 -Wall $(DEBUGMODE)  -I/usr/local/include -c xmlparser.cpp
	
NNetwork.o : Layer.o
	g++ -std=c++11 -Wall $(DEBUGMODE)  -I/usr/local/include -c NNetwork.cpp
	
Layer.o:
	g++ -std=c++11 -Wall $(DEBUGMODE)  -I/usr/local/include -c Layer.cpp
	
clean :
	rm *.o

