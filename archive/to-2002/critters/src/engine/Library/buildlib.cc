#include "LibraryFile.h"
#include "ExtraTypes.h"
#include <dirent.h>
 
int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *ep;
	char *filename="critters.lib";

	if (argc > 1) 
	{
		filename=argv[1];
	}

	printf("Building library with all critters in /. .\n");
	LibraryFile *CritterLib = new LibraryFile(filename);

	dp = opendir ("./");
	if (dp != NULL)
	{
 		while ((ep = readdir (dp)))
			if ((new String(ep->d_name))->endsWith(new String(".dat")))
			{
				CritterLib->addCritter(ep->d_name);
			}
		closedir (dp);
	}
	else
	{
		puts ("Couldn't open the directory. ABORTING!");
		delete CritterLib;
		return -1;
     	}
 
	CritterLib->saveLibrary();

	delete CritterLib;

	printf("Library completed.\n");


}
