#include <string.h>
#include <stdio.h>

#include "String.h"

String::String(String *inString)
{
	if (inString->getLength() != 0)
	{
		data = new char [inString->getLength()+1];
		strcpy(data,inString->getString());
	}
	else 
	{
		data = NULL;
	}
}

String::String(char *inString)
{
	if (strlen(inString) != 0)
	{
		data = new char [strlen(inString)+1];
		strcpy(data,inString);
	}
	else 
	{
		data = NULL;
	}
}

String::String(FILE   *InputStream)
{
	char BUF[256];
	int ptrBUF=0;

	if ((fread(BUF,1,1,InputStream) ==1 ) && ( *(BUF) != 0))
	{
		while (*(BUF+ptrBUF) != 0)
		{
			fread(BUF+(++ptrBUF),1,1,InputStream);
		}
		data = new char [strlen(BUF)+1];
		strcpy(data,BUF);
	}
	else
	{
		data = NULL;		
	}

}

String::~String()
{
	delete data;
}

char *String::getString()
{
	return data;
}

int String::getLength()
{
	if (data != NULL) return strlen(data);
	else return 0;
}

bool String::equals(String *inString)
{
	return (strcmp(inString->getString(),data) == 0 );
}

bool String::endsWith(String *inString)
{
	return (strcmp(inString->getString(),data+strlen(data)-(inString->getLength())) == 0 );
}

void String::put()
{
	printf("%s",data);
}

int String::fput(FILE *outfile)
{
	if (!isnull()) fputs(data,outfile);
	fputc(0,outfile);
	return 5;
}

bool String::isnull()
{
	return (data==NULL);
}

int String::hash()
{
	int temp=0;
	for(int a=0;a<getLength();a++)
		temp+=data[a];
	return temp;
}


