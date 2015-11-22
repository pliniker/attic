/* Routines for critters 
	ver 0.0.1
	20/5/99
*/

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

extern Engine_Class *engine;


/* Private Function definitions */

void Base_Critter_Class::Think(void) // recalulate knowledge based on last move...
{
	int i=0,c=0;
	
	if(status.energy < HUNGRY_LEVEL)
		brain.hungry = 1;
	else
		brain.hungry = 0;

	brain.targetx=x;
	brain.targety=y;

	if(brain.looked) // if we've seen sommit nice
	{
		for(i=0;i<10;i++)
		{
			if(brain.target[i] > c)
			{
				brain.targetx = ((i % 3)-1)+x;
				brain.targety = ((i / 3)-1)+y;
			}			
		}
	}
	else
	{
		while((brain.targetx==x)&&(brain.targety==y)&&(i++<20))
		{
			brain.targetx = x+1-(rand()%3); // keep guessing until we find a target
			brain.targety = y+1-(rand()%3);	// other than our current square
						// (max of 20 guesses)
	
			if (brain.targetx > (engine->map_x))
				brain.targetx = engine->map_x;
			if (brain.targetx <0)
				brain.targetx = 0; // don't move...
	
			if (brain.targety > (engine->map_y))
				brain.targety = engine->map_y;
			if (brain.targety <0)
				brain.targety = 0; // don't move...

			if (!(engine->Map[brain.targetx][brain.targety].terrain & 
				dna.characteristics[TERRAIN_MASK]))
			{
				brain.targetx = x; // don't move onto impassable land.
				brain.targety = y; 
			}
		}
	}

	if (brain.moved)
		brain.happy = 1;	
	
}

char Base_Critter_Class :: CanEatV(int x1, int y1)
{
	if((x1<0)||(x1>engine->map_x)||(y1<0)||(y>engine->map_y))
		return 0;
	else
		if(dna.characteristics[VEG_MASK] & engine->Map[x1][y1].terrain)
			return engine->Map[x1][y1].food_status;
		else
			return 0;
}

char Base_Critter_Class :: CanEatM(int x1, int y1)
{
	Base_Critter_Class *other;

	if((x1<0)||(x1>engine->map_x)||(y1<0)||(y>engine->map_y))
		return 0;
	else
	{
	other = engine->Map[x1][y1].occupant;

	if ((other != NULL) && (other != this)) // don't eat yourself.....
		{
		if (other->dna.characteristics[TYPE] &
			dna.characteristics[MEAT_MASK]) // if he's edible.....
			return 1;
		else
			return 0;
		}
	else
		return 0;
	}
}

/* Public Function definitions */

Base_Critter_Class :: Base_Critter_Class (unsigned char x_pos, unsigned char y_pos, 
	char type, char energy, int id)
{
x = x_pos;
y = y_pos;

dna.characteristics = new unsigned char [MAX_DNA];
dna.behaviour = new unsigned char [MAX_PRIORITIES];

status.energy = energy; // start off with full energy
dna.characteristics[TYPE] = type;
dna.characteristics[TERRAIN_MASK] = 253; // everything can go everywhere except water(for now)
if (type == 1) // herbivore
	{
	dna.characteristics[MEAT_MASK] = 0; // no meat
	dna.characteristics[VEG_MASK] = 255; // any veg
	}
if (type == 2) // carnivores can eat herbivores
	{
	dna.characteristics[MEAT_MASK] = 253; // any meat
	dna.characteristics[VEG_MASK] = 0; // no veg though....
	}

dna.behaviour[0]=LOOK;
dna.behaviour[1]=EAT;
dna.behaviour[2]=MOVE;
dna.behaviour[3]=BREED;
dna.id=id;

}

void Base_Critter_Class :: Look (void) // look around a bit
{					// and find which square looks nice
	int i,j;
	int tmp=0;
	int tmp2=0;

	brain.looked=0;

	for(i=0;i<10;i++)
		brain.target[i]=0; // reset all target squares

	for(j=-2;j<2;j++)
		for(i=-1;i<2;i++)
		{
			if((x+i>0)&&(x+i<engine->map_x)&&(y>0)&&(y+j<engine->map_y))
			{
			if (!(engine->Map[x+i][y+j].terrain & 
				dna.characteristics[TERRAIN_MASK]))
				if(((tmp2=CanEatV(x+i,y+j))>tmp)&&brain.hungry)
				{
					tmp=tmp2;
					brain.target[(j+1)*3+(i+1)]=1;
					brain.looked=1;
				}

				if(CanEatM(x+i,y+j)&&brain.hungry)
				{
					brain.target[(j+1)*3+(i+1)]=2;
					brain.looked=1;
				}
			}
		}
}

void Base_Critter_Class :: EatV (void) // eat vegetation
{
	if ((CanEatV(brain.targetx,brain.targety) > 5) && brain.hungry)
	{	
		if (engine->Map[brain.targetx][brain.targety].food_status > 50)
		{	
			engine->Map[brain.targetx][brain.targety].food_status -= 50;
			status.energy += 50;
			// increase energy
			brain.happy=1; // be happy :o)
		}
		else
		{
			status.energy += engine->Map[brain.targetx][brain.targety].food_status;
			engine->Map[brain.targetx][brain.targety].food_status = 0;
 			// increase energy
			brain.happy = 1;
		}
	}
	
}

void Base_Critter_Class :: EatM () // eat a critter
{
	Base_Critter_Class *other = engine->Map[brain.targetx][brain.targety].occupant;

	if (CanEatM(brain.targetx,brain.targety)&&brain.hungry)
	{
		if((status.energy + other->status.energy) < 255)
			status.energy += other->status.energy; 
		else
			status.energy = 255;
		 // consume the energy of the other critter
 		printf("Eating a critter!! (my id= %d)\n",dna.id); 	
 		engine->Kill(other);
		//printf("x: %d, y: %d\n",x,y); 		
		brain.happy = 1;
	} 	
}

void Base_Critter_Class :: Move(void)
{
	
	brain.moved = abs(brain.targetx-x) + abs(brain.targety-y);

	if(engine->Map[brain.targetx][brain.targety].occupant == NULL)
	{ // if no-one's there......

		engine->Map[x][y].occupant = NULL; // moveout of current square
		x=brain.targetx;		// and into the new one...
		y=brain.targety;
		engine->Map[x][y].occupant = this;
	} 	 	
	else
		brain.moved = 0;

	if (status.energy > (brain.moved * 2 + 2))
	{
		status.energy -= (brain.moved*2) + 2;
	}
	else
	{
		brain.happy=1; // 'you'll be grateful when you're dead...'
		status.energy = 0;
	}	
}

void Base_Critter_Class :: Breed(void)
{
	if (status.energy > BREED_LEVEL)
	{
		if(engine->Map[brain.targetx][brain.targety].occupant == NULL)
		{
		printf("Breeding! my energy = %d\n",status.energy);
		engine->Add_Critter(brain.targetx,brain.targety,
			dna.characteristics[TYPE],70);
		status.energy-=70;
		brain.happy=1;
		}
	}
}

unsigned char Base_Critter_Class :: Update (void)
{
	int priority=0;

	brain.happy=0; // no move made yet.
	brain.moved=0;

	while((!brain.happy)&&(priority < MAX_PRIORITIES)) // try all moves
	{
		Think(); // recalculate things, base on last move made...

		switch(dna.behaviour[priority++])
		{
			case LOOK:
				Look();
				break;
			case EAT:
				EatV();
				EatM();
				break;
			case MOVE:
				Move();
				break;
			case BREED:
				Breed();
				break;
		}		
	
	}

//		printf("Type: %d ", dna.characteristics[TYPE]);
// 		printf("Position: %d, %d Energy: %d\n",x,y, status.energy); 
//		printf("Food status: %d\n",engine->Map[x][y].food_status);
	
	return status.energy;

}


Base_Critter_Class ::~Base_Critter_Class (void)
{
engine->Map[x][y].occupant = NULL;

printf("Dead, type: %d, id: %d\n",dna.characteristics[TYPE],dna.id);

delete dna.characteristics;
delete dna.behaviour;

}
