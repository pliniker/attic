/* Main file for simulation engine
	ver 0.0.1
	20/5/99
*/

#include <stdio.h>
#include "engine.h"

Engine_Class *engine; 

char *searchm(int argc, char *argv[])
{
	int i;
	
	for (i = 0; i < (argc - 1); i++) {
		if (strcmp(argv[i], "-m") == 0) {
			return argv[i + 1];
		}
	}
	
	return "test.map";
}


char *searchl(int argc, char *argv[])
{
	int i;
	
	for (i = 0; i < (argc - 1); i++) {
		if (strcmp(argv[i], "-l") == 0) {
			return argv[i + 1];
		}
	}
	
	return "critters.lib";
}


int main (int argc, char *argv[])
{

        engine = new Engine_Class(searchm(argc,argv),
				searchl(argc,argv)); // specify map & lib

	printf("Welcome to Critters!\n");

        engine->Main_Loop();

	delete engine;
	printf("finished...\n");
	return (0);
}
