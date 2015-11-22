/* Definition of all stats classes
	25/5/99
*/

struct Engine_Stats_Struct
{ 
	unsigned char watch[255]; // the stats the GUI is watching
	int watch_num;

	unsigned long int generations;
	unsigned long int omnis; // number of omnivores
	unsigned long int carns; // number of carnivores
	unsigned long int herbs; // number of herbivores
	unsigned long int critters; // overall number of critters
	unsigned long int legs; // overall number of legs in world
 	unsigned long int fins; // no. of fins
	unsigned long int wings; // wings....
	unsigned long int segs; // average number of segments
}; 

struct Critter_Stats_Class 
{ 
	unsigned char energy;
};

