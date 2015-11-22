#if !defined(__StringTable)
#define __StringTable

#include "String.h"
#include "Table.h"

#include <stdio.h>

class StringTable : public Table {

protected:

	void initTable(int);

public:

	     StringTable(int);
	     StringTable();
	    ~StringTable();

	void read(FILE *);
	void write(FILE *);
	void dump();
	int  add(String *);
	int  remove(String *);
	int  contains(String *);
 

};

#endif  /* __StringTable */
