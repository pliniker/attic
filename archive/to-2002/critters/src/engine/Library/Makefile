
CC=gcc -g -O2 -Wall
CXX=g++ -g -O2 -Wall
CFLAGS+=-g -O2 -Wall
LIBS=


OBJS=	CritterFile.o \
	LibraryFile.o \
	String.o \
	Table.o \
	StringTable.o \
	HashTable.o

main:	$(OBJS) main.o
	$(CXX) $(LIBS) $(CFLAGS) -o main $(OBJS) main.o

buildlib:	$(OBJS) buildlib.o
		$(CXX) $(LIBS) $(CFLAGS) -o buildlib $(OBJS) buildlib.o

clean:
	-rm *.o *.lib main buildlib
		
