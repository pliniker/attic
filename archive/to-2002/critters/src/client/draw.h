

#define TILE_SIZE 32		// default tile size on startup. height = width
#define DRAW_SIZE 16		// number of tiles per row and column

#define REFRESH 50			// map is redrawn every 50ms by default

// call these in the order they appear
void InitWindows();			// setup all windows, buttons and other widgets, and assign handlers
int InitNetwork(char *h);
void InitMap();				// create the map
void InitThreads();			// create the io thread and the redraw monitor thread

void ShutDown();			// call this after gtk_main() has finished
