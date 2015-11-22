
CC=gcc `gtk-config --cflags` -g -O2 -Wall
CXX=gcc `gtk-config --cflags` -g -O2 -Wall
CFLAGS+=-g -O2 -Wall `gtk-config --cflags`
LIBS= `gtk-config --libs` -lpthread


OBJS=	windows.o \
	client.o \
	colours.o \
	map.o \
	draw.o \
	stats.o \
	../common/pthreads.o \
	../common/gthreads.o \
	../common/network.o

all:	$(OBJS)
	$(CXX) $(LIBS) $(CFLAGS) -o client $(OBJS)

clean:
	-rm *.o client
		
