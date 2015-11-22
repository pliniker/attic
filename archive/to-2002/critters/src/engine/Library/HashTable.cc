#include <stdio.h>
#include <string.h>

#include "String.h"
#include "HashTable.h"

void   HashTable::read(FILE *InputStream)
{
	fread(&iSize,sizeof(int),1,InputStream);
	initTable(iSize);
	fread(&iFull,sizeof(int),1,InputStream);
	fread(iContents,sizeof(int),iSize,InputStream);
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

void   HashTable::write(FILE *OutputStream)
{
	fwrite(&iSize,sizeof(int),1,OutputStream);
	fwrite(&iFull,sizeof(int),1,OutputStream);
	fwrite(iContents,sizeof(int),iSize,OutputStream);
	for (int a=0;a<iSize;a++)
	{
		if (sContents[a]!=NULL) sContents[a]->fput(OutputStream);
		else fputc(0,OutputStream);
	}

}



void HashTable::dump()
{
	for (int a=0;a<iSize;a++)
	{
		if(sContents[a]!=NULL)
		{
			printf("%d:%d:",a,sContents[a]->hash()%iSize);
			sContents[a]->put();
			printf(":%d\n",iContents[a]);
		}
	}
}

void HashTable::initTable(int Size)
{
	//if (sContents != NULL) delete sContents;
	sContents = new (String*) [Size];
	iContents = new int    [Size];
	for (int a = 0;a<Size;a++)
	{
		sContents[a]=NULL;
		iContents[a]=0;
	}
	iSize = Size;
	iFull = 0;
}

HashTable::HashTable(int Size)
{
	initTable(Size);
}

HashTable::HashTable()
{
	initTable(256);
}


HashTable::~HashTable()
{
	delete sContents;
	delete iContents;
}

int HashTable::add(String *Data)
{
	return add(Data,0);
}

int HashTable::add(String *Data,int value)
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

		iContents[temp] = value;
		sContents[temp] = new String(Data);
		iFull++;


		return temp;
	}
	else
	{
		return -1;

	
	}
}

int HashTable::getValue(String *Data)
{
	int temp;
	temp = contains(Data);

	if (temp != -1)
	{
		return iContents[temp];
	}

	return -1;

}

int HashTable::remove(String *Data)
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

int HashTable::contains(String *Data)
{
	int temp,xx,yy;
	yy = -1;
	temp = Data->hash()%iSize;

	for (int a=0;a<iSize;a++)
	{
		xx = (a + temp) % iSize;
		//printf("Searching %d\n",xx);
		if ((sContents[xx] !=NULL) && (sContents[xx]->equals(Data)))
		{
			yy = xx;
			break;
		}

	}
	return yy;

}
