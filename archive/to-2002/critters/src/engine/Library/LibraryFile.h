#if !defined(__LibraryFile)
#define __LibraryFile

#define MAXCRITTERS 32

#include "LibraryFile.h"
#include "ExtraTypes.h"
#include "StringTable.h"
#include "HashTable.h"

#include <stdio.h>

class LibraryFile {

private:

	int          NoOfParameters,
	             NoOfCritters;
	BYTE         bytecode[MAXCRITTERS][256];

	HashTable   *Names;
	StringTable *Parameters;
	String      *LibFileName;


public:

	             LibraryFile(String*);
	             LibraryFile(char*);
	            ~LibraryFile();

	void         saveLibrary();
	int          getParameterOf(int, char *);
	int          getParameterOf(int, String *);
	int 	     getParameterOf(char *critter, char *param);
	void         loadData(String*,BYTE *);
	void         loadData(char *,BYTE *);
	void         loadLibrary(String *);
	void         addParameters(String **,int);
	void         addCritter(String *);
	void         addCritter(char*);

	int          getNoOfParameters();
	int          getNoOfCritters();
	StringTable *getParameters();
	StringTable *getNames();
	
		
};

#endif /* __LibraryFile */
