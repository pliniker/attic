#if !defined(__String)
#define __String

#include <stdio.h>

class String {

private:

	char *data;

public:

	      String(String *);
	      String(char *);
	      String(FILE *);
	     ~String();

	int   hash();
	bool  equals(String *);
	bool  isnull();
	bool  endsWith(String *);
	void  put();
	int   fput(FILE *);

	char *getString();
	int   getLength();

};

#endif  /* __String */
