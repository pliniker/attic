#include "LibraryFile.h"
#include "ExtraTypes.h"

int main()
{
	BYTE test[20];

	printf("Building library with five critters.\n");
	LibraryFile *CritterLib = new LibraryFile("critters.lib");
	CritterLib->addCritter("carny.dat");
	CritterLib->addCritter("herby.dat");
	CritterLib->addCritter("beast1.dat");
	CritterLib->addCritter("beast2.dat");
	CritterLib->addCritter("beast3.dat");
	CritterLib->addCritter("beast4.dat");
	CritterLib->addCritter("beast5.dat");
	CritterLib->saveLibrary();
	CritterLib->loadData("HERBY",test);
	printf("\nHERBY %d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",test[0],test[1],test[2],test[3],test[4],test[5],test[6],test[7],test[8],test[9],test[10],test[11],test[12],test[13],test[14],test[15],test[16]);
	CritterLib->loadData("CARNY",test);
	printf("CARNY  %d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",test[0],test[1],test[2],test[3],test[4],test[5],test[6],test[7],test[8],test[9],test[10],test[11],test[12],test[13],test[14],test[15],test[16]);
	delete CritterLib;
	printf("Library completed.\n");


	printf("Opening existing library\n");
	CritterLib = new LibraryFile("critters.lib");
	CritterLib->loadData("HERBY",test);
	printf("HERBY %d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",test[0],test[1],test[2],test[3],test[4],test[5],test[6],test[7],test[8],test[9],test[10],test[11],test[12],test[13],test[14],test[15],test[16]);
	CritterLib->loadData("CARNY",test);
	printf("CARNY  %d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d\n",test[0],test[1],test[2],test[3],test[4],test[5],test[6],test[7],test[8],test[9],test[10],test[11],test[12],test[13],test[14],test[15],test[16]);
	delete CritterLib;

}
