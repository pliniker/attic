#include <stdio.h>


void main(void)
{
        int i,j; 
	int map_x,map_y;
	
	FILE *mapfile;
	char *filename="test.map"; 

	if((mapfile = fopen(filename,"r"))==NULL)
		{
		printf("Couldn't open %s!\n",filename);
		perror("Fatal Error!");
		exit(-1);
		}

	fscanf(mapfile,"%d,%d;",&map_x,&map_y);
	perror("ouput of fscanf!");
	printf("%d, %d\n",map_x,map_y);

	if(!(fclose(mapfile)))
		{
		printf("Couldn't close %s!\n",filename);
		perror("Result of fclose");
		}

	printf("Allocationg memory for map %d x %d....\n",map_x , map_y); 
//	printf("Size required = %d bytes\n", map_x*map_y*sizeof(Map_Square_Struct)); 

//	Map = new Map_Square_Struct * [map_x]; 
//	for (i=0;i<=map_x;i++) 
//		Map[i] = new Map_Square_Struct [map_y]; 


        for (i=0;i<map_x;i++) 
                for(j=0;j<map_y;j++) 
                        { 
//                        Map[i][j].terrain = 5; // random veg type 
//                        Map[i][j].occupant = NULL; 
//                        Map[i][j].food_status = rand() % 255; 
                        } 
               
}
