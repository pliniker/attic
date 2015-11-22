#if !defined(__HashTable)
#define __HashTable

#include <stdio.h>
#include "String.h"
#include "Table.h"

class HashTable : public Table {

private:

	int *iContents;
	void initTable(int Size);

public:

	HashTable(int Size);
	HashTable();
	~HashTable();

	int    add(String *Data);

	int    add(String *Data,int value);
	int    add(char *Data,int value);
	int    getValue(String *Data);
	int    getValue(char *Data);
	int    remove(String *Data);
	int    contains(String *Data);
	void   dump();
	void   read(FILE *InputStream);
	void   write(FILE *OutputStream);
};

#endif  /* __HashTable */
