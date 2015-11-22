
CC=gcc -g -Wall
CXX=gcc -g -Wall
CFLAGS+=-g -Wall 
LIBS= 


OBJS=	engine.o \
	critters.o \
	main.o \
	../common/network.o \
	./Library/CritterFile.o \
	./Library/LibraryFile.o \
	./Library/String.o \
	./Library/Table.o \
	./Library/StringTable.o \
	./Library/HashTable.o

all:	$(OBJS)
	$(CXX) $(LIBS) $(CFLAGS) -o engine $(OBJS)

clean:
	-rm *.o engine
		
