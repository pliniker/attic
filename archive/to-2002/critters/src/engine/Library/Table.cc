#include <stdio.h>
#include <string.h>

#include "String.h"
#include "Table.h"

void Table::initTable(int Size)		{return;}
void Table::read(FILE *InputStream)	{return;}
void Table::write(FILE *OutputStream)	{return;}
void Table::dump()			{return;}
int  Table::add(String *Data)		{return 0;}
int  Table::remove(String *Data)	{return 0;}
int  Table::contains(String *Data)	{return 0;}

String **Table::getContents()
{
	int x = 0;
	String **list = new String *[iFull];
	for (int a=0;a<iSize;a++)
	{
		if(sContents[a]!=NULL)
		{
			list[x]=sContents[a];
			x++;
		}
	}
	return list;
}

int Table::getFull()
{
	return iFull;
}

