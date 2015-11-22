#include <stdio.h>
#include <string.h>

#include "String.h"
#include "StringTable.h"

void   StringTable::read(FILE *InputStream)
{
	fread(&iSize,sizeof(int),1,InputStream);
	initTable(iSize);
	fread(&iFull,sizeof(int),1,InputStream);
	for (int a=0;a<iSize;a++)
	{
		sContents[a] = new String(InputStream);
		if (sContents[a]->isnull())
		{
			delete sContents[a];
			sContents[a] = NULL;
		}
	}
}

void   StringTable::write(FILE *OutputStream)
{
	fwrite(&iSize,sizeof(int),1,OutputStream);
	fwrite(&iFull,sizeof(int),1,OutputStream);
	for (int a=0;a<iSize;a++)
	{
		if (sContents[a]!=NULL) sContents[a]->fput(OutputStream);
		else fputc(0,OutputStream);
	}

}



void StringTable::dump()
{
	for (int a=0;a<iSize;a++)
	{
		if(sContents[a]!=NULL)
		{
			sContents[a]->put();
		}
	}
}

void StringTable::initTable(int Size)
{
	//if (sContents != NULL) delete sContents;
	sContents = new (String*) [Size];
	for (int a = 0;a<Size;a++)
	{
		sContents[a]=NULL;
	}
	iSize = Size;
	iFull = 0;
}

StringTable::StringTable(int Size)
{
	initTable(Size);
}

StringTable::StringTable()
{
	initTable(256);
}


StringTable::~StringTable()
{
	delete sContents;
}

int StringTable::add(String *Data)
{
	
	if ((iFull < iSize))
	{
		int temp;
		temp = Data->hash() % iSize;

		while (sContents[temp]!=NULL)
		{
			temp++;
			if (temp == iSize) temp = 0;
		}

		sContents[temp] = new String(Data);
		iFull++;


		return temp;
	}
	else
	{
		return -1;

	
	}
}


int StringTable::remove(String *Data)
{
	int temp;
	temp = contains(Data);

	if (temp != -1)
	{
		delete sContents[temp];
		sContents[temp]=NULL;
		iFull--;
	}

return temp;

}

int StringTable::contains(String *Data)
{
	int temp,xx,yy;
	yy = -1;
	temp = Data->hash()%iSize;

	for (int a=0;a<iSize;a++)
	{
		xx = (a + temp) % iSize;
		
		if ((sContents[xx] !=NULL) && (sContents[xx]->equals(Data)))
		{
			yy = xx;
			break;
		}

	}
	return yy;

}
