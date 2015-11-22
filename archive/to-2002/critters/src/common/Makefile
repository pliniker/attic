
CXX=g++ -g -O2 -Wall `gtk-config --cflags`
CC=gcc
CFLAGS+=-g -O2 -Wall

OBJS=	network.o \
		pthreads.o \
		gthreads.o

all:	$(OBJS)
	$(CXX) $(CFLAGS) -c $(OBJS)

clean:
	-rm *.o
		
