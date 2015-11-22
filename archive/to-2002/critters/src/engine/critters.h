/* Definitions of critter classes
	25/5/99
*/

#include "stats.h"
#include "segments.h"

#define TYPE		0
#define SIZE		1
#define FOOD_MASK	2
#define	OTHER		3
#define TERRAIN_MASK	4
#define MAX_DNA		5

#define RANDY		255	// return values for checking compatiblity
#define HEADACHE	0	//		:o)

#define HUNGRY_LEVEL	150
#define BREED_LEVEL	160

struct DNA_Struct
{ 
	unsigned char *characteristics; 
	unsigned char *segments; 
	unsigned char num_segs;
	unsigned char type; // herbivore, carnivore or omnivore
	unsigned long int id;
}; 

class Critter_Class 
{ 
private: 
	Critter_Stats_Class status; 
	int x,y;
	int age;
	int targetx; // co-ords of current target square.
	int targety; // 
	int lookedx;
	int lookedy;
	unsigned char move_counter;

	// char Hungry(void); // returns hungry or not....
	void Look (void); // look around a bit...
	void Think (void); // think....
	unsigned char CanEatV (int x1, int y1); // Can I eat this grass?
	void EatV (void); // try & eat some veg
	unsigned char CanEatM (int x1, int y1); // is there a critter here i can eat?

	void DumpDNA(void); // prints out full DNA to screen for debug purposes...

	void EatM (void); // try & eat a critter
	char CanMove (int x1, int y1); // Can I go there?
	void Walk (void); // move....
	void Breed (void); // have babies.
	Critter_Class *CanBreed(int x1, int y1); // Can i breed with the tasty specimen here?
	void Swim (void); // go skinny dipping
	void Fly (void); //move anywhere (takes more energy)

public: 
 
	Critter_Class *next;
	Critter_Class *last;
	DNA_Struct dna;  

        Critter_Class(unsigned char x_pos, unsigned char y_pos, 
		DNA_Struct *data, char energy); 
	unsigned char Update (void); 
	~Critter_Class(void); 
 
};
