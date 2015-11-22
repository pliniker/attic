/* Definitions of engine and map classes
	20/5/99
*/

#include "../common/network.h"
#include "Library/LibraryFile.h"
#include "critters.h"


struct Map_Square_Struct
{
        unsigned char terrain; // type of ground
	unsigned char altitude; // height of ground
        unsigned char food_status; // amount of food & food type
        Critter_Class *occupant; 
//	bool changed; // has the square changed since last time?
};


class Engine_Class
{
private:
	bool halt; // signal to halt the engine
	int speed; // speed of emulation
	int num; // number of critters in world
	int redraw; // redraw or not flag
	int sack; // send stats or not flag
	char **critlist; // list of critter names
	unsigned char x1,x2,y1,y2; // co-ords (top-left & bot. right) of window
	Network *network; // instance of network class
	LibraryFile *lib; // instance of the critter lib class
        Engine_Stats_Struct *stats; // stats for current game
        Critter_Class *first_critter; // pointer to 1st critter in list
	Critter_Class *last_critter; // pointer to end of list 
	int Load_Lib(char *filename); // load the critter library
        int Load_Map(char *filename); // load a landscape 
        void Update_Map(void); // update status of map
        void Update_Critters(void); // update all  the critters & critter stats
	void Update_Stats(void); // update all the world stats
        void Spawn(char type,char x, char y, char energy); // add a new critter
		// will eventually take info about critter type
        void Draw_Map(void); // call the GUI to update the screen
        void Get_Input(void); // get any user input from the GUI	


public:
	int map_x, map_y; // x & y size of map
        Map_Square_Struct **Map; // 2d array holding the map
 	
	void Kill(Critter_Class *critter); // remove a dead critter 
        Engine_Class(char *mapfile, char *libfile); // 
	void Add_Critter(char x, char y, DNA_Struct *dna, char energy); // add to the engine

        void Main_Loop(void); 
        ~Engine_Class(void); // de-allocate memory etc
};

