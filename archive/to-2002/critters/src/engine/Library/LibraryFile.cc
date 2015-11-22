#include "LibraryFile.h"
#include "CritterFile.h"
#include "String.h"

LibraryFile::LibraryFile(String *FileName)
{
	Parameters     = new StringTable(256);
	LibFileName    = new String(FileName);
	Names          = new HashTable(MAXCRITTERS);
	NoOfParameters = 0;
	NoOfCritters   = 0;
	loadLibrary(LibFileName);
}

LibraryFile::LibraryFile(char *FileName)
{
	Parameters     = new StringTable(256);
	LibFileName    = new String(FileName);
	Names          = new HashTable(MAXCRITTERS);
	NoOfParameters = 0;
	NoOfCritters   = 0;
	loadLibrary(LibFileName);
}

LibraryFile::~LibraryFile()
{
	if (Parameters  != NULL) delete Parameters;
	if (Names       != NULL) delete Names;
	if (LibFileName != NULL) delete LibFileName;
}

int LibraryFile::getNoOfParameters()
{
	return NoOfParameters;
}

int LibraryFile::getNoOfCritters()
{
	return NoOfCritters;
}

StringTable *LibraryFile::getParameters()
{
	return Parameters;
}

StringTable *LibraryFile::getNames()
{
	return (StringTable*)Names;
}

int LibraryFile::getParameterOf(int a , char *Parameter)
{
	String *temp = new String(Parameter);
	int temp2 = getParameterOf(a,temp);
	delete temp;
	return temp2;
}

int LibraryFile::getParameterOf(int a , String *Parameter)
{
	int x = Parameters->contains(Parameter);
	for (int y=1 ;y<bytecode[a][0];y+=2)
	{
		if (bytecode[a][y] == x) return bytecode[a][y+1];
	}
	return -1;
}

int LibraryFile::getParameterOf(char *critter, char *param)
{
	String *Name = new String(critter);
	int a = Names->getValue(Name);
	delete Name;

	return getParameterOf(a,param);

}

void LibraryFile::loadData(char *crittername,BYTE *Data)
{
	
	Data[0]  = getParameterOf(crittername,"Type");
	Data[1]  = getParameterOf(crittername,"Size");
	Data[2]  = getParameterOf(crittername,"FoodMaskVeg");
	Data[3]  = getParameterOf(crittername,"FoodMaskMeat");
	Data[4]  = getParameterOf(crittername,"Terrain");
	Data[5]  = getParameterOf(crittername,"NumSegs");
	Data[6]  = getParameterOf(crittername,"Other2");
	Data[7]  = getParameterOf(crittername,"Segment0");
	Data[8]  = getParameterOf(crittername,"Segment1");
	Data[9]  = getParameterOf(crittername,"Segment2");
	Data[10] = getParameterOf(crittername,"Segment3");
	Data[11] = getParameterOf(crittername,"Segment4");
	Data[12] = getParameterOf(crittername,"Segment5");
	Data[13] = getParameterOf(crittername,"Segment6");
	Data[14] = getParameterOf(crittername,"Segment7");
	Data[15] = getParameterOf(crittername,"Segment8");
	Data[16] = getParameterOf(crittername,"Segment9");

}

void LibraryFile::addParameters(String **ParamNames,int Number)
{
	for (int a =0;a<Number;a++)
	{
		if (Parameters->contains(ParamNames[a]) == -1) Parameters->add(ParamNames[a]);
	}
	NoOfParameters = Parameters->getFull();
}

void LibraryFile::loadLibrary(String *FileName)
{
	FILE *InputLibrary;
	InputLibrary = fopen(FileName->getString(),"rb");
	if (InputLibrary == NULL) return;
	fread(&NoOfParameters,sizeof(int),1,InputLibrary);
	fread(&NoOfCritters,sizeof(int),1,InputLibrary);
	fread(bytecode,sizeof(BYTE)*MAXCRITTERS,256,InputLibrary);
	Names->read(InputLibrary);
	Parameters->read(InputLibrary);
	fclose(InputLibrary);

}

void LibraryFile::saveLibrary()
{
	FILE *OutputLibrary;
	OutputLibrary = fopen (LibFileName->getString(),"wb");
	fwrite(&NoOfParameters,sizeof(int),1,OutputLibrary);
	fwrite(&NoOfCritters,sizeof(int),1,OutputLibrary);
	fwrite(bytecode,sizeof(BYTE)*MAXCRITTERS,256,OutputLibrary);
	Names->write(OutputLibrary);
	Parameters->write(OutputLibrary);
	fclose(OutputLibrary);
	
}

void LibraryFile::addCritter(char *FileName)
{
	String *temp = new String(FileName);
	addCritter(temp);
	delete temp;
}

void LibraryFile::addCritter(String *FileName)
{

	int size = 1;
	int storage = 0;
	CritterFile *InputCritter = new CritterFile(FileName);

	if (InputCritter->OK)
	{
		String    **List = InputCritter->getParameters();
		int number       = InputCritter->getNoOfParameters();

		String *Name = new String(InputCritter->getName());

		if (Names->contains(Name) != -1) 
		{
			printf("Overwriting existing %s definition with %s from %s!\n",Name->getString(),Name->getString(),FileName->getString());
			storage = Names->contains(Name);
		}
		else 
		{
			printf("Adding %s from %s to library.\n",Name->getString(),FileName->getString());
			storage = NoOfCritters;
			Names->add(Name,NoOfCritters);
			NoOfCritters++;
		}
		
		
		addParameters(List,number);
		
		for (int a =0;a<number;a++)
		{
			bytecode[storage][size++] = Parameters->contains(List[a]);
			bytecode[storage][size++] = InputCritter->getParameter(List[a]);
		}

		bytecode[storage][0] = size - 1;
	
		delete Name;
		delete List;
	}
	else
		printf("A fatal error has occured, continuing anyway.......\n");
		
	delete InputCritter;

}

