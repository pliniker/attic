#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include "HashTable.h"
#define bool int
#define FALSE 0
#define TRUE 1

class DataFile {
public:
int ParseNumber(char Number[]);
HashTable *DataTable;
char Name[64];
char Description1[80];
char Description2[80];
char Description3[80];
char Description4[80];
bool OK;
FILE *InfoFile;

DataFile(char FileName[]);
~DataFile();

int GetParameter(char ParamName[]);

char *GetName();
char *GetDescription1();
char *GetDescription2();
char *GetDescription3();
char *GetDescription4();

};

DataFile::DataFile(char FileName[])
{
char *Buffer = new char[81];
InfoFile = fopen(FileName,"rt");
if (InfoFile == NULL)
OK = FALSE;
else
{
OK = TRUE;
DataTable = new HashTable();

while (strcmp(fgets(Buffer,80,InfoFile),"[Identification]\n")!=0)
{
//getch();
}

while (strcmp(fgets(Buffer,80,InfoFile),"[Parameters]\n")!=0) {}
while (strcmp(fgets(Buffer,80,InfoFile),"\n")!=0)
{
int a =0 ;
while (*(Buffer+a) !='=')
{a++;}
*(Buffer+a) = 0;
DataTable->add(Buffer,ParseNumber((Buffer+a+1)));

}

//while (strcmp(fgets(Buffer,80,InfoFile),"[Behaviour]")!=0) {}


}
delete Buffer;

}

DataFile::~DataFile()
{
if(OK)
{
fclose(InfoFile);
delete DataTable;
}
}

int DataFile::ParseNumber(char Number[])
{
int TempValue;
switch (*Number)
{
case 'D' : sscanf(Number+1,"%d",&TempValue);break;
default  : TempValue = -1; break;

}
return TempValue;
}

int DataFile::GetParameter(char ParamName[])
{
return DataTable->getValue(ParamName);
}

void main()
{
//clrscr();

DataFile *BeastFile = new DataFile("Beast.dat");

printf("%d\n",BeastFile->GetParameter("Strength"));

delete BeastFile;

}
