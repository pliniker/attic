#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CritterFile.h"
#include "String.h"
#include "../segments.h"

#define FALSE false
#define TRUE  true

CritterFile::CritterFile(String *FileName)
{
	InitCritterFile(FileName->getString());
}

CritterFile::CritterFile(char FileName[])
{
	InitCritterFile(FileName);
}

void CritterFile::InitCritterFile(char FileName[])
{
// This member does the majority of the work in reading in 
// a critter definiton file, and setting up the internal data
// structure.

	char *Buffer = new char[81];
	InfoFile = fopen(FileName,"rt");
	if (InfoFile == NULL)
		OK = FALSE;
	else
	{
		// File is open OK
		OK = TRUE;

		CritterTable = new HashTable();

		// Scan to dentification section
		while (strcmp(fgets(Buffer,80,InfoFile),"[Identification]\n")!=0) {}
		fgets(Buffer,80,InfoFile);
		strcpy(Name,Buffer+5);
		*(Name+strlen(Name)-1) = 0;
		fgets(Buffer,80,InfoFile);
		strcpy(Description1,Buffer+13);
		fgets(Buffer,80,InfoFile);
		strcpy(Description2,Buffer+13);
		fgets(Buffer,80,InfoFile);
		strcpy(Description3,Buffer+13);
		fgets(Buffer,80,InfoFile);
		strcpy(Description4,Buffer+13);

		// Scan to parameters section
		while (strcmp(fgets(Buffer,80,InfoFile),"[Parameters]\n")!=0) {}
		// read params
		while (strcmp(fgets(Buffer,80,InfoFile),"\n")!=0)
		{
			// Break and add strings to has table
			int a =0 ;
			while (*(Buffer+a) !='=')
				{a++;}
			*(Buffer+a) = 0;
			*(Buffer+a+strlen(Buffer+a+1)) = 0;

			String *temp = new String(Buffer);
			CritterTable->add(temp,ParseNumber((Buffer+a+1)));
			delete temp;

		}


	}
	// Clean up and return;
	delete Buffer;

}

CritterFile::~CritterFile()
{
	if(OK)
	{
		fclose(InfoFile);
		delete CritterTable;
	}
}

int CritterFile::ParseNumber(char Number[])
{
	int TempValue;
	switch (*Number)
	{
		case 'D' : sscanf(Number+1,"%d",&TempValue);break;
		case 'H' : sscanf(Number+1,"%x",&TempValue);break;
		case 'O' : sscanf(Number+1,"%o",&TempValue);break;
		case 'S' : TempValue = makeValue(Number+1);break;
		default  : TempValue = -1; break;
	}

	return TempValue;
}

int CritterFile::makeValue(char Segment[])
{
	if (strcmp(Segment,"LOOK")  == 0) {return LOOK;}
	if (strcmp(Segment,"EATV")  == 0) {return EATV;}
	if (strcmp(Segment,"LEG")   == 0) {return LEG;}
	if (strcmp(Segment,"BREED") == 0) {return BREED;}
	if (strcmp(Segment,"EATM")  == 0) {return EATM;}
	if (strcmp(Segment,"FIN")   == 0) {return FIN;}
	if (strcmp(Segment,"WING")  == 0) {return WING;}
	return -1;
}

int CritterFile::getParameter(char ParamName[])
{
	String *temp = new String(ParamName);
	int x = CritterTable->getValue(temp);
	delete temp;
	return x;
}

int CritterFile::getParameter(String *ParamName)
{
	return CritterTable->getValue(ParamName);
}

char *CritterFile::getDescription1()
{
	return Description1;
}

char *CritterFile::getDescription2()
{
	return Description2;
}

char *CritterFile::getDescription3()
{
	return Description3;
}

char *CritterFile::getDescription4()
{
	return Description4;
}

char *CritterFile::getName()
{
	return Name;
}

String **CritterFile::getParameters()
{
	return CritterTable->getContents();
}

int CritterFile::getNoOfParameters()
{
	return CritterTable->getFull();
}



