#Mia's makefile for the final project database application.

G = g++-4.9
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei

Inked: Project.o
	${G} ${LFLAGS} -Wall -Wextra -o Inked Project.o 

Project.o: Project.cpp Project.hpp
	${G} -c ${CFLAGS} Project.cpp

clean:
	rm -f *.o Inked