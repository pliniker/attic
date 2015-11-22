/* Engine routines for critters 
	ver 0.0.1
	20/5/99
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "engine.h"

/* Private Function definitions */

void Engine_Class :: Update_Map (void)
{
int x, y;

for (x=0;x<map_x;x++)
	for(y=0;y<map_y;y++)
		Map[x][y].food_status+=1;
		

}

void Engine_Class :: Update_Critters (void)
{
	Critter_Class *p, *n;
	unsigned char c;

        p=first_critter;
	stats->critters=0;
	stats->herbs=0;
	stats->carns=0;
	stats->omnis=0;
	stats->legs=0;
	stats->fins=0;
	stats->wings=0;

        while(p != NULL)
        {
                c=p->Update();// if critter runs out of energy
		n=p->next;
		if (!c)
			Kill(p);
		else
		{
			stats->critters++;
			if((p->dna.type & 1)&&(p->dna.type &2))
				stats->omnis++;
			else
			{
				if(p->dna.type & 1)
					stats->herbs++;
				if(p->dna.type & 2)
					stats->carns++;
			}
			if(p->dna.type & 4)
				stats->legs++;
			if(p->dna.type & 8)
				stats->fins++;
			if(p->dna.type & 16)
				stats->wings++;
			stats->segs+=p->dna.num_segs;
		}
		p=n;		
        }

	if(stats->critters>0)
		stats->segs = stats->segs / stats->critters;	
	stats->omnis = stats->critters-(stats->herbs + stats->carns);
		// if they're not herbs or carns, they're omni's.....
                
}

void Engine_Class::Update_Stats (void)
{
	int i;
	unsigned char count=0;
	unsigned char data[256];

	stats->generations++;

	if(sack) // if it's clear to send more stats...
	{
		for(i=0;i<stats->watch_num;i++)
		{
			switch(stats->watch[i])
			{
				case GENERATIONS:
					data[count++]=(unsigned char)GENERATIONS;
					data[count++]=(unsigned char)(stats->generations>>24);
					data[count++]=(unsigned char)((stats->generations>>16)&255);
					data[count++]=(unsigned char)((stats->generations>>8)&255);
					data[count++]=(unsigned char)((stats->generations)&255);
					break;
				case CARNS:
					data[count++]=(unsigned char)CARNS;
					data[count++]=(unsigned char)(stats->carns>>8);
					data[count++]=(unsigned char)((stats->carns)&255);
					break;
				case HERBS:
					data[count++]=(unsigned char)HERBS;
					data[count++]=(unsigned char)(stats->herbs>>8);
					data[count++]=(unsigned char)((stats->herbs)&255);
					break;
				case OMNIS:
					data[count++]=(unsigned char)OMNIS;
					data[count++]=(unsigned char)(stats->omnis>>8);
					data[count++]=(unsigned char)((stats->omnis)&255);
					break;
				case CRITTERS:
					data[count++]=(unsigned char)CRITTERS;
					data[count++]=(unsigned char)(stats->critters>>8);
					data[count++]=(unsigned char)((stats->critters)&255);
					break;
				case LEGS:
					data[count++]=(unsigned char)LEGS;
					data[count++]=(unsigned char)(stats->legs>>8); 				 	
					data[count++]=(unsigned char)((stats->legs)&255);  	 		 		 
					break;  	 		
				case FINS:  	 	 		
					data[count++]=(unsigned char)FINS;  
				 	data[count++]=(unsigned char)(stats->fins>>8);
					data[count++]=(unsigned char)((stats->fins)&255);
					break; 	  	
				case WINGS:
					data[count++]=(unsigned char)WINGS;
					data[count++]=(unsigned char)(stats->wings>>8);
					data[count++]=(unsigned char)((stats->wings)&255);
			  	  	break;
				case SEGS:
					data[count++]=(unsigned char)SEGS;
					data[count++]=(unsigned char)stats->segs;
					break;
 	   		} 	 
		}

	if(count > 0)
		{
		network->SendMsg(STATS,count,data);
		sack=0;
		}

	
	} // otherwise don't send anything.....
}

void Engine_Class :: Draw_Map ()
{

	int i,j;
	int count=0;

	unsigned char data[255];

	const struct timespec wait={0,10};
	struct timespec remain;


	if (redraw)
	{
		network->SendMsg(MAP_START,0,0);

		for(j=y1;j<y2;j++)
			for(i=x1;i<x2;i++)
			{
				data[count++] = Map[i][j].food_status;
				if (Map[i][j].occupant != NULL)
 					data[count++] = Map[i][j].occupant->dna.type;
				else	
					data[count++] = 0; 		
				if(count == 254)
				{
					network->SendMsg(MAP,254,data);
					count = 0;
				}
			}

		if (count != 0)
			network->SendMsg(MAP,count,data);
		
		redraw = 0;

//		network->SendMsg(REDRAW,0,0);
	}
	else // waiting for GUI to deal with last redraw!
	{
	nanosleep(&wait, &remain);
	
	}
}

void Engine_Class :: Get_Input(void)
{
	int pause = 0;
	int i;
	const struct timespec wait={0,10};
	struct timespec remain;

	unsigned char com, nargs;
	unsigned char data[255];

	do
	{
		while (network->GetMsg(&com, &nargs, data) > 0)
		{
			switch(com)
			{		
				case QUIT:
					halt = 1;
					pause=0;
					break;
				case BUFFER:
					x1=data[0];
					y1=data[1];
					x2=data[2];
					y2=data[3];
					network->SendMsg(BACK,4,data);
					Draw_Map(); // just incase we're paused.....
					break;
				case NEWCRIT:
					Spawn(data[2],data[0],data[1],255); // spawn carnivore
					break;
				case PAUSE:
					pause=1-pause; // sit and wait for an unpause msg
					break;
				case SPEED:
					speed = data[0];
					break;
				case STATS_REQ:
					for(i=0;i<nargs;i++)
						stats->watch[i] = data[i];
					stats->watch_num=nargs;
					break;
				case ACK:
					redraw = 1;
					break;
				case SACK:
					sack = 1;
					break;
				case START:
					redraw = 1;
					sack = 1; // the client is ready
					break;	  // so start sending stuff
				default:
					printf("Unknown msg recv'd: %d",com);
					break;
			}
		}
	nanosleep(&wait,&remain);
	}
	while(pause); // only do network stuff if we're paused.
}

void Engine_Class :: Add_Critter(char x, char y, DNA_Struct *dna, char energy)
{
//	char data[80];
//	char name[80];
	Critter_Class *critter;

	dna->id = num++;
	critter = new Critter_Class(x,y,dna,energy); // pass to the critter
	// type to replaced by 'data'
//	printf("Spawning...\n");


//	library.access_proc_load(name, data);	
	
	if(first_critter == NULL) 
        { 
	       	first_critter = critter; 
	        critter->last = NULL; 
        	critter->next = NULL; 
	       	last_critter = critter; 
	} 
        else 
        { 
		last_critter->next = critter; 
	       	critter->last = last_critter; 
		critter->next = NULL;
	        last_critter = critter; 
 	} 

	Map[x][y].occupant = critter; 

}
void Engine_Class :: Spawn (char type, char x, char y,char energy)
{
	int i;
	DNA_Struct *dna; // buffer for loaded dna 
	char seglabel[10][9];
	seglabel[0] = "Segment0";
	seglabel[1] = "Segment1";
	seglabel[2] = "Segment2";
	seglabel[3] = "Segment3";
	seglabel[4] = "Segment4";
	seglabel[5] = "Segment5";
	seglabel[6] = "Segment6";
	seglabel[7] = "Segment7";
	seglabel[8] = "Segment8";
	seglabel[9] = "Segment9";

	dna = new DNA_Struct; // load the dna & store it here

	dna->characteristics = new unsigned char [MAX_DNA];

	dna->characteristics[TYPE] = lib->getParameterOf(critlist[type], "Type");

	dna->num_segs=lib->getParameterOf(critlist[type],"NumSegs");
	if(dna->num_segs > 10)
		dna->num_segs = 10; // no more than 10 segments plz :o)

	dna->segments = new unsigned char [dna->num_segs];

	dna->characteristics[FOOD_MASK] = lib->getParameterOf(critlist[type], "FoodMask");
	dna->characteristics[OTHER] = 0;
	dna->characteristics[TERRAIN_MASK] = lib->getParameterOf(critlist[type], "Terrain");
	
	for(i=0;i<dna->num_segs;i++)
		dna->segments[i]=lib->getParameterOf(critlist[type], seglabel[i]);

	// find the next free map square....

	if ((Map[x][y].occupant == NULL)&&(Map[x][y].terrain 
		& dna->characteristics[TERRAIN_MASK]))
		{
		Add_Critter(x,y,dna,energy);
//		printf("New critter spawned: type %s, no: %d\n",critlist[type], num-1);
//		printf("x=%d, y=%d, terrain = %d\n",x,y,Map[x][y].terrain);
		}

	delete dna->characteristics;
	delete dna->segments;
	delete dna; // remove our buffer 

}

void Engine_Class :: Kill (Critter_Class *critter)
{
	if(first_critter != critter)
		critter->last->next = critter->next;
	else
		first_critter = critter->next;
	if(last_critter != critter)
		critter->next->last = critter->last;
	else
		last_critter = critter->last;
	delete critter;	

}


/* Public Function definitions */

Engine_Class::Engine_Class (char *mapfile, char *libfile)
{		// default filenames passed by main();

	halt = 0;
	num=0;
	x1=0;x2=0;y1=0;y2=0; // set initial window to zero size
	redraw=0; // it's not ok to redraw!
	sack=0; // it's not ok to send stats!

	printf("Starting Game Engine...\n");

	printf("Starting Server...\n");
	network = new Network("localhost");
	if((network->StartServer()) < 0)
		{
		printf("Could not start server!");
		perror("Fatal Error!");
		exit(-1);
		} 

	if(Load_Map(mapfile) <= 0)
		exit(-1);
	if(Load_Lib(libfile) <= 0)
		exit(-1);

	stats = new Engine_Stats_Struct;

	stats->generations = 0;

	stats->watch_num=0;
	
	speed = 1;
        first_critter = NULL;
	printf("\n");

}

int Engine_Class::Load_Lib(char *filename)
{
	String **contents;
	int num,i;

	printf("Loading critter library from: %s...\n",filename);

	lib = new LibraryFile(filename);

	if(!(contents = lib->getNames()->getContents()))
		return -1;
	if((num = lib->getNoOfCritters()) <= 0)
		return -1;

	critlist = new char *[num];

	printf("num of critters in Lib: %d\n",num);

	network->SendMsg(CRIT_START,0,0); // tell the client we're sending a list of critters

	for(i=0;i<num;i++)
	{
		critlist[i] = new char[contents[i]->getLength()];
		strcpy(critlist[i],contents[i]->getString());
		network->SendMsg(CRIT_DATA,(unsigned char)contents[i]->getLength(),
			(unsigned char *)critlist[i]);
		printf("Critter %d: %s\n", i,critlist[i]);
	}

	network->SendMsg(CRIT_END,0,0);

	return 1;
}

int Engine_Class :: Load_Map (char *filename)
{
        int i,j;
	int temp; 
	int count=0;
	unsigned char data[256];
	
	FILE *mapfile;

	printf("Reading map data from: %s\n",filename);

	if((mapfile = fopen(filename,"r"))==NULL)
		{
		printf("Couldn't open %s!\n",filename);
		perror("Fatal Error!");
		return -1;
		}

	fscanf(mapfile,"%d,%d;",&map_x,&map_y);


	printf("Allocationg memory for map %d x %d....\n",map_x , map_y); 
	printf("Size required = %d bytes\n", map_x*map_y*sizeof(Map_Square_Struct)); 

	Map = new Map_Square_Struct * [map_x+1]; 
	for (i=0;i<=map_x;i++) 
		Map[i] = new Map_Square_Struct [map_y+1]; 

	data[0]=map_x;
	data[1]=map_y;
	network->SendMsg(MAP_SIZE,2,data); // tell the GUI the size of the map

        for (j=0;j<map_y;j++)
                for(i=0;i<map_y;i++)
                       
		{
			fscanf(mapfile,"%d,%d;",&temp, (int*)&Map[i][j].food_status);
			Map[i][j].terrain = (unsigned char)(temp & 0x0F);
			Map[i][j].altitude = (unsigned char)((temp >> 4) & 0x0F);

			data[count++]=(temp & 0xFF);

			if(count == 255) // send data in chunks of 255 tiles.
			{
				network->SendMsg(TERRAIN,255,data);
				count = 0;
			}

                        Map[i][j].occupant = NULL;
		}

	if (count != 0)
		network->SendMsg(TERRAIN,count,data); // send any remaining data

	if(fclose(mapfile)!=0)
		{
		printf("Couldn't close %s!\n",filename);
		perror("Result of fclose");
		return -1;
		}

	return 1;
              
}

void Engine_Class :: Main_Loop (void)
{
	int i;
	const struct timespec wait={0,10};
	struct timespec remain;

	Get_Input(); // get any inital handshaking messages

	while(!halt)
	{
	        Update_Map();
		Update_Critters();
		Update_Stats();
		for(i=0;i<10-speed;i++)
			nanosleep(&wait, &remain);

		Get_Input();
		Draw_Map();
	}

}

Engine_Class :: ~Engine_Class (void)
{

        Critter_Class *p,*n;

	printf("Destroying Game Engine...\n");

        printf("Removing critters...\n");

        p=first_critter;

        while(p != NULL)
                {
                n=p->next;
		Kill (p);
                p = n;
                }

	printf("Generations: %li\n",stats->generations);
	printf("Critters: %li (h: %li, c: %li, o: %li)\n",stats->critters, 
		stats->herbs, stats->carns, stats->omnis);

	delete stats;

	printf("De-allocating Library memory...\n");
	
	delete lib;

        printf("De-allocating Map memory...\n");

        delete Map;

	printf("Shutting down server...\n");
	network->EndSession();
	delete network;

	printf("\n");

}

