/* Routines for critters 
	ver 0.0.1
	20/5/99
*/

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

extern Engine_Class *engine;


/* Private Function definitions */

void Critter_Class :: Think (void) // ponder what move to make...
{
	int i=0;

	if (!lookedx) // if we haven't found anything good......
	{
		targetx=x;

		while((targetx==x)&&(i++<10))
		{ 					// move randomly
			targetx = x+1-(rand()%3); // keep guessing until we find a target
						// other than our current square
						// (max of 10 guesses)
	
			if (targetx > (engine->map_x))
				targetx = engine->map_x;
			if (targetx <0) // don't move outside the map bounds
				targetx = 0; 

		}
	}
	
	i=0;

	if (!lookedy) // if we haven't found anything good......
	{
		targety=y;

		while((targety==y)&&(i++<10))
		{ 					// move randomly
			targety = y+1-(rand()%3); // keep guessing until we find a target
						// other than our current square
						// (max of 10 guesses)
	
			if (targety > (engine->map_y))
				targety = engine->map_y;
			if (targety <0) // don't move outside the map bounds
				targety = 0; 

		}
	}

}

void Critter_Class :: DumpDNA (void) // dump DNA to stdout for debugging
{
	int i;

	printf("DNA dump: \n\n");
	printf("id: %li\n", dna.id);

	printf("numsegs: %d\n", dna.num_segs);

	printf("type (food): %d\n", dna.characteristics[TYPE]);
	printf("foodmask: %d\n",dna.characteristics[FOOD_MASK]);
	printf("other: %d\n",dna.characteristics[OTHER]);
	printf("terrainmask: %d\n",dna.characteristics[TERRAIN_MASK]);
	
	for(i=0;i<dna.num_segs;i++)
		printf("segment %d: %d\n",i,dna.segments[i]);

}

void Critter_Class :: Look (void) // look around a bit
{					// and find which square looks nice
	int i; // counter
	int a=0,b=0; // tmp storage for food comparisons

	lookedx=0;
	lookedy=0;

	if((x-1)>=0)	//look left
		for(i=-1;i<2;i++)
		{
			if(((y+i)>=0) && ((y+i)<=engine->map_y))
			{
				if(CanBreed(x-1,y+i)!=NULL)
					a++;
				a+=CanEatV(x-1,y+i);
				a+=CanEatM(x-1,y+i);
			}
		}

	if((x+1)<=engine->map_x) // look right
		for(i=-1;i<2;i++)
		{
			if(((y+i)>=0) && ((y+i)<=engine->map_y))
			{
				if(CanBreed(x+1,y+i)!=NULL)
					b++;
				b+=CanEatV(x+1,y+i);
				b+=CanEatM(x+1,y+i);
			}
		}

	a=a-b; // find the difference....

	if(a>45)
		{
		targetx=x-1;
		lookedx=1;
		}
	if(a<-45)
		{
		targetx=x+1;
		lookedx=1;
		}

	a=0;
	b=0;

	if((y-1)>=0) // look up
		for(i=-1;i<2;i++)
		{
			if(((x+i)>=0) && ((x+i)<=engine->map_y))
			{
				if(CanBreed(x+i,y-1)!=NULL)
					a++;
				a+=CanEatV(x+i,y-1);
				a+=CanEatM(x+i,y-1);
			}
		}

	if((y+1)<=engine->map_y) // look down
		for(i=-1;i<2;i++)
		{
			if(((x+i)>=0) && ((x+i)<=engine->map_y))
			{
				if(CanBreed(x+i,y+1)!=NULL)
					b++;
				b+=CanEatV(x+i,y+1);
				b+=CanEatM(x+i,y+1);
			}
		}

	a=a-b; // find the difference

	if(a>45)
		{
		targety=y-1;
		lookedy=1;
		}
	if(a<-45)
		{
		targety=y+1;
		lookedy=1;
		}

}

unsigned char Critter_Class :: CanEatV(int x1, int y1)
{
	if(status.energy > HUNGRY_LEVEL)
		return 0;
	else
		if((dna.characteristics[FOOD_MASK] & engine->Map[x1][y1].terrain)
				&&(dna.type & 1))
			return engine->Map[x1][y1].food_status;
		else
			return 0;
}

void Critter_Class :: EatV (void) // eat vegetation
{
	if (CanEatV(targetx,targety) > 5)
	{	
		if (engine->Map[targetx][targety].food_status > 30)
		{	
			engine->Map[targetx][targety].food_status -= 30;
			status.energy += 30;
			// increase energy
		}
		else
		{
			status.energy += engine->Map[targetx][targety].food_status;
			engine->Map[targetx][targety].food_status = 0;
 			// increase energy
		}
	}
	
}

unsigned char Critter_Class :: CanEatM(int x1, int y1)
{
	Critter_Class *other;

	if((status.energy > HUNGRY_LEVEL) || (!(dna.type & 2)))
		return 0;
	else
	{
	other = engine->Map[x1][y1].occupant;

	if ((other != NULL) && (other != this)) // don't eat yourself.....
		{
		if (other->dna.characteristics[TYPE] &
			dna.characteristics[FOOD_MASK]) // if he's edible.....
			return 255;
		else
			return 0;
		}
	else
		return 0;
	}
}

void Critter_Class :: EatM () // eat a critter
{
	Critter_Class *other = engine->Map[targetx][targety].occupant;

	if (CanEatM(targetx,targety)&&status.energy<HUNGRY_LEVEL)
	{
		if((status.energy + other->status.energy) < 255)
			status.energy += other->status.energy; 
		else
		{
			status.energy = 255;
		// consume the energy of the other critter
		}
//		printf("Eating a critter!! (my id= %li)\n",dna.id); 	
		engine->Kill(other);
//		intf("x: %d, y: %d\n",x,y); 		
	} 	
}

char Critter_Class ::CanMove(int x1, int y1)
{
	if((engine->Map[x1][y1].occupant==NULL)&&
		(engine->Map[x1][y1].terrain & dna.characteristics[TERRAIN_MASK]))
		return 1;
	else
		return 0;
}

void Critter_Class :: Walk(void)
{
	int moved =0;

	if((CanMove(targetx,targety))&&(engine->Map[targetx][targety].terrain != WATER))
	{
		engine->Map[x][y].occupant = NULL; // moveout of current square
		x=targetx;		// and into the new one...
		y=targety;
		engine->Map[x][y].occupant = this;
		moved = abs(targetx-x) + abs(targety-y);

	} 	 	

	if (status.energy > (moved))
		status.energy -= (moved);
	else
		status.energy = 0;
}

Critter_Class *Critter_Class :: CanBreed(int x1, int y1) // can i breed with this lovely specimen?
{ 
	int i;
	int c=0; // comapatiblity factor
	Critter_Class *mate; //	potential mate
	
	if((status.energy > BREED_LEVEL)&&(mate=engine->Map[x1][y1].occupant) !=NULL) 
	{	// if someone's there
		if((mate != this)&&(mate->dna.num_segs == dna.num_segs))
		{
			for(i=0;i<MAX_DNA;i++)
				if((mate->dna.characteristics[i])!=(dna.characteristics[i]))
					c++;
			for(i=0;i<8;i++)
				if((mate->dna.type)&(1<<i) != (dna.type)&(1<<i))
					c++;
			for(i=0;i<dna.num_segs;i++)
				if((mate->dna.segments[i])!=(dna.segments[i]))
					c++;
			if(c <= rand()%6)
				return mate;
			else
				return NULL;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

void Critter_Class :: Breed(void)
{
	int i;
	int a,b;
	int x2=-1,y2=-1; // co-ords of where to drop the sprog
	DNA_Struct *newdna; // buffer for loaded dna 
	Critter_Class *mate; // mate for breeding
	DNA_Struct *from;

	if ((mate = CanBreed(targetx, targety))!=NULL)
	{
		for(b=y-1;b<y+2;b++) // find somewhere to drop the sprog
			for(a=x-1;a<x+2;a++)
			{
				if(!((a<0)||(a>engine->map_x)||(b<0)||(b>engine->map_y)))
					if (CanMove(a,b))
					{
						x2 = a;
						y2 = b;
					}
			}
		
		if((x2!=-1)&&(y2!=-1)) // if we found somewhere to drop it
		{

			newdna = new DNA_Struct; // work out new dna & store it here

			if(rand()%4)
				from = &this->dna;
			else
				from = &mate->dna;

			newdna->num_segs=from->num_segs;
	
			newdna->characteristics = new unsigned char [MAX_DNA];
			newdna->segments = new unsigned char [newdna->num_segs];

			// splice the dna (should merge with another!)
			for (i=0;i<MAX_DNA;i++)
			{
				if(rand()%4) // crossover
				{				
					if(from==&this->dna)
						from=&mate->dna;
					else
						from=&this->dna;
				}
				newdna->characteristics[i] = from->characteristics[i];
			}
			for (i=0;i<newdna->num_segs;i++)
			{
				if(rand()%4)
				{
					if(from==&this->dna)
						from=&mate->dna;
					else
						from=&this->dna;
				}
					newdna->segments[i] = from->segments[i];
			}
	//		printf("Breeding! my energy = %d\n",status.energy);
			engine->Add_Critter(targetx,targety,
				newdna,70);
			status.energy-=70;
			delete newdna->characteristics;
			delete newdna->segments;
			delete newdna; // remove our buffer 
		}
	}
}

void Critter_Class :: Swim(void)
{
	int moved = 0;

	if((CanMove(targetx,targety))&&(engine->Map[targetx][targety].terrain == WATER))
	{
		engine->Map[x][y].occupant = NULL; // moveout of current square
		x=targetx;		// and into the new one...
		y=targety;
		engine->Map[x][y].occupant = this;
		moved = abs(targetx-x) + abs(targety-y);

	} 	 	

	if (status.energy > (moved))
		status.energy -= (moved);
	else
		status.energy = 0;
}

void Critter_Class :: Fly(void)
{
	int moved = 0;

	if(engine->Map[targetx][targety].occupant == NULL)
	{
		engine->Map[x][y].occupant = NULL; // moveout of current square
		x=targetx;		// and into the new one...
		y=targety;
		engine->Map[x][y].occupant = this;
		moved = abs(targetx-x) + abs(targety-y);
	}

	if (status.energy > (moved*2))
		status.energy -= (moved*2);
	else
		status.energy = 0;

}

/* Public Function definitions */

Critter_Class :: Critter_Class (unsigned char x_pos, unsigned char y_pos, 
	DNA_Struct *data, char energy)
{
	int i;

	x = x_pos;
	y = y_pos;
	dna.type=0;

	age=0;

	dna.num_segs = data->num_segs;
	
	if((rand()%5) > 3) // random new segment
		dna.num_segs ++; 

	dna.characteristics = new unsigned char [MAX_DNA];
	dna.segments = new unsigned char [dna.num_segs];

	// copy over the dna (should merge with another!)
	for (i=0;i<MAX_DNA;i++)
		if(rand()%8)
		{
			dna.characteristics[i] = data->characteristics[i];
		}
		else
		{
			dna.characteristics[i]= data->characteristics[i]+rand()%3;
		}

	if (dna.characteristics[TYPE] == 0)
		dna.characteristics = (unsigned char *)(rand() % 255);

	for (i=0;i<dna.num_segs;i++)
	{
		if(i<data->num_segs) // if we're still copying
			dna.segments[i] = data->segments[i];
		else
			dna.segments[i] = (rand()%7);
				// random new segment
		switch(dna.segments[i])
		{
			case EATV:
				dna.type |= 1;
				break;
			case EATM:
				dna.type |= 2;
				break;
			case LEG:
				dna.type |= 4;
				break;
			case FIN:
				dna.type |= 8;
				break;
			case WING:
				dna.type |= 16;
				break;
			case BREED:
				dna.type |= 32;
				break;

		}

	}

	if((rand()%5)> 3) // random lose segment
	{
		for(i=rand()%dna.num_segs;i<dna.num_segs-1;i++)
			dna.segments[i]=dna.segments[i+1];
		dna.num_segs--;
	}

	move_counter=0;
	lookedx=0;
	lookedy=0;
	targetx=x;
	targety=y;

	status.energy = energy; // start off with full energy
	dna.id=data->id;

//	DumpDNA(); // testing purposes......

}


unsigned char Critter_Class :: Update (void)
{

	Think(); // always have a ponder & find a random target if we have no other

	switch(dna.segments[move_counter++])
	{
		case LOOK:
			Look();
			break;
		case EATV:
			EatV();
			break;
		case EATM:
			EatM();
			break;
		case LEG:
			Walk();
			break;
		case BREED:
			Breed();
			break;
		case FIN:
			Swim();
			break;
		case WING:
			Fly();
			break;
	}		
	
	if(move_counter > dna.num_segs)
	{
		move_counter = 0;
	}

	age++;

	if (status.energy>2)
		status.energy-=2;
	else
		status.energy=0;

//	printf("Type: %d ", dna.characteristics[TYPE]);
// 	printf("Position: %d, %d Energy: %d\n",x,y, status.energy); 
//	printf("Food status: %d\n",engine->Map[x][y].food_status);
	
	return status.energy;

}


Critter_Class ::~Critter_Class (void)
{
engine->Map[x][y].occupant = NULL;

//printf("Dead, type: %d, id: %li\n",dna.characteristics[TYPE],dna.id);

delete dna.characteristics;
delete dna.segments;

}
