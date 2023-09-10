OPTIONS=-O2

all : program testdh

program : main.cpp agenda.o coordonnees.o dateheure.o
	g++ ${OPTIONS} -std=c++11 -o program main.cpp agenda.o coordonnees.o dateheure.o
	
coordonnees.o : coordonnees.cpp coordonnees.h
	g++ ${OPTIONS} -std=c++11 -c -o coordonnees.o coordonnees.cpp

dateheure.o : dateheure.cpp dateheure.h
	g++ ${OPTIONS} -std=c++11 -c -o dateheure.o dateheure.cpp

agenda.o : agenda.cpp agenda.h coordonnees.h dateheure.h arbreavl.h
	g++ ${OPTIONS} -std=c++11 -c -o agenda.o agenda.cpp

testdh : testdh.cpp dateheure.o
	g++ $(OPTIONS) -std=c++11 -o testdh testdh.cpp dateheure.o

clean:
	rm -rf program testdh *~ *.o *.gch

