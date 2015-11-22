/* Definitions of critter classes
	25/5/99
*/

#include "stats.h"

#define LOOK		0	// priority stuff for behaviour
#define MOVE		1
#define	EAT		2
#define BREED		3

#define MAX_PRIORITIES	4 	// number of behaviours in list

#define TYPE		0
#define SIZE		1
#define VEG_MASK	2
#define MEAT_MASK	3
#define TERRAIN_MASK	4
#define MAX_DNA		5

#define HUNGRY_LEVEL	150
#define BREED_LEVEL	160

class DNA_Class 
{ 
public: 
	unsigned char *characteristics; 
	unsigned char *behaviour; 
	int id;
}; 
 
struct Brain_Struct
{
	int targetx; // target square co-ords
	int targety;
	char moved; // how much we've moved this turn.....

	char target[10]; // how favourable each surrounding square is
	char looked; // did I look & find sommit good?
	char hungry;
	char happy;
};

class Base_Critter_Class 
{ 
private: 
	Critter_Stats_Class status; 
	int x,y; 
	Brain_Struct brain;

	// char Hungry(void); // returns hungry or not....
	void Think (void); // sit and ponder any new data....
	void Look (void); // look around a bit...
	char CanEatV (int x1, int y1); // Can I eat this grass?
	void EatV (void); // try & eat some veg
	char CanEatM (int x1, int y1); // is there a critter here i can eat?
	void EatM (void); // try & eat a critter
	void Move (void); // move....
	void Breed (void); // have babies.

public: 
 
	Base_Critter_Class *next;
	Base_Critter_Class *last;
	DNA_Class dna;  

        Base_Critter_Class(unsigned char x_pos, unsigned char y_pos, 
		char type, char energy, int id); 
	unsigned char Update (void); 
	~Base_Critter_Class(void); 
 
};
