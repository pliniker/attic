#if !defined(__Table)
#define __Table

#include <stdio.h>
#include "String.h"

class Table {

protected:

	String **sContents;
	int      iSize;
	int      iFull;

	void     initTable(int);

public:

	void     read(FILE *);
	void     write(FILE *);
	void     dump();
	int      add(String *);
	int      remove(String *);
	int      contains(String *);

	int      getFull();
	String **getContents();
	
};

#endif  /* __Table */
