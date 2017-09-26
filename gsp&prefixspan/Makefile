CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGs = -lm

all: clean gsp.o prefixspan.o

gsp.o: 
	${CC} ${CFLAGS} -o gsp gsp.cpp

prefixspan.o:
	${CC} ${CFLAGS} -o prefixspan prefixspan.cpp

clean:
	rm -rf *.o

