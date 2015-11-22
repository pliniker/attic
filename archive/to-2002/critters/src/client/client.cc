

#include <string.h>
#include "gtk/gtk.h"
#include "draw.h"



char *searchargs(int argc, char *argv[])
{
	int i;
	
	for (i = 0; i < (argc - 1); i++) {
		if (strcmp(argv[i], "-h") == 0) {
			return argv[i + 1];
		}
	}
	
	return "localhost";
}



int main(int argc, char *argv[])
{
// always call this first to avoid segfaults
	gtk_init(&argc, &argv);
	
// allocate memory for various things - widgets, map, threads.
	InitWindows();
	if (!InitNetwork(searchargs(argc, argv))) return -1;
	InitMap();
	InitThreads();

// now enter the gtk main loop
	gtk_main();
	
// deallocate mem etc
	ShutDown();
	
	return 0;
}
