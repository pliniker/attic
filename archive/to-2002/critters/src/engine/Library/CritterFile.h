#if !defined(__CritterFile)
#define __CritterFile

#include "HashTable.h"
#include "String.h"
#include <stdio.h>

class CritterFile {

private:

	HashTable *CritterTable;
	FILE      *InfoFile;
	char       Name[80];
	char       Description1[80];
	char       Description2[80];
	char       Description3[80];
	char       Description4[80];

public:

	bool       OK;
	           CritterFile(String *);
	           CritterFile(char *);
	          ~CritterFile();
	
	int        ParseNumber(char *);
	int        makeValue(char *);
	void       InitCritterFile(char *);


	int        getParameter(char *);
	int        getParameter(String *);
	int        getNoOfParameters();
	String   **getParameters();
	char      *getName();
	char      *getDescription1();
	char      *getDescription2();
	char      *getDescription3();
	char      *getDescription4();


};

#endif /* __CritterFile */
