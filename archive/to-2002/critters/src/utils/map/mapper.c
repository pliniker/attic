//mapper version 0.4

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int get_int_args(char *value,char *name, int use_stdout) {
    //pre: value must be an array of 4 or more chars
    //pre: name must point to a string; this could be just a null char.
    int arg = 0;
    if (isdigit(value[3])) { arg = atoi(value+3); }
    else { printf("Incomplete argument %s: needs a positive integer value after %c.\n", value, value[1]); }
    if (!use_stdout) { printf("User defined %s percentage: %d\n",name,arg); }
    return arg;
    //post: arg will be a valid integer
}

int main (int argc, char *argv[])
{
    struct Terrain_Square
    {
        signed int terrain;
	signed int altitude;
    };
    struct Terrain_Square **map = 0;
    int utime;
    long ltime;
    int okay, use_stdout;
    char *output_choice;
    FILE *output_file;
    int i,j;
    int total;
    int water, plains, forest, impass;
    int water_pc, plains_pc, forest_pc, impass_pc;
    int width, height;
    signed int terrain, altitude, food;
    int avg_alt;
    int num_avg;    
    int level, terrain_level;
    int terrain_set;
    ltime = time(NULL);
    utime = (unsigned int) ltime/2;
    srand(utime);
    
    output_choice = NULL;
    output_file = NULL;
    use_stdout = -1;
    water = plains = forest = impass = total = 0;
    width = height = 0;
    
    for (i=1;i<argc;i++) {
        for (j=0;j<(signed int)strlen(argv[i]);j++) {
            argv[i][j] = tolower(argv[i][j]);
        }
        if (argv[i][0] != '-') {
            printf ("Unknown argument %s: must start with a -.\n", argv[i]);
        }
        else if (strlen(argv[i]) < 4) {
            printf ("Unknown argument %s: argument cannot be a single -.\n", argv[i]);
        }
        else {
            switch (argv[i][1]) {
            case 't': switch(argv[i][2]) {
                          case 'w': water = get_int_args(argv[i],"water", use_stdout);
                                    break;
                          case 'p': plains = get_int_args(argv[i],"plains", use_stdout);
                                    break;
                          case 'f': forest = get_int_args(argv[i],"forest", use_stdout);
                                    break;
                          case 'i': impass = get_int_args(argv[i],"impassable", use_stdout);
                                    break;
                          default:  printf("Unknown terrain argument: %s\n", argv[i]);
                                    break;
                }
                      break;
            case 'm': switch(argv[i][2]) {
                          case 'w': width = get_int_args(argv[i],"width", use_stdout);
                                    break;
                          case 'h': height = get_int_args(argv[i],"height", use_stdout);
                                    break;
                          default:  printf("Unknown map argument: %s\n", argv[i]);
                                    break;
                }
                      break;
            case 'f': if (strlen(argv[i]) < 3) {
                          printf("Unknown file output: %s cannot be just -f\n", argv[i]);
                          printf("Please specify either:\n");
                          printf("\t-f<filename> to write to a file\n");
                          printf("\t-fstdout to write to the console\n");
                      }
                      else {
                          output_choice = (char *) malloc(strlen(argv[i])-2);
                          if (!&output_choice) {
                              printf("Null pointer returned from malloc.\nAborted.\n");
                              exit(-1);
                          }
                          strcpy(output_choice, argv[i]+2);
                          if (!strcmp("-fstdout",argv[i])) {
                              output_file = stdout;
                              use_stdout = -1;
                          }
                          else {
                              output_file = fopen(output_choice, "w");
                              if(!output_file) {
                                  printf("Cannot write to %s.\nAborted.\n", argv[i]+2);
                                  exit(-1);
                              }
                              else {
                                  printf("\nCritter Map Generator\nVersion 0.4\n\n");
                                  printf("Writing to %s...\n", output_choice);
                              }
                              use_stdout = 0;
                          }                        
                      }
                      break;
            case '?':
            case 'h': printf("Critter Map Generator\nVersion 0.4\nHelp: see included file README\n");
                      break;
            default:  printf("Unknown argument: %s\n", argv[i]);
                      break;
            }
        }
    }
    if (!output_choice) {
        output_file = stdout;
        use_stdout = -1;
    }
    if ((!water) && (!plains) && (!forest) && (!impass)) {
        if (!use_stdout) { printf("No terrain ratios specified, using defaults:\n"); }
        water = 10;
        plains = 50;
        forest = 25;
        impass = 15;
        total = 100;
        if (!use_stdout) { printf("Water: %d\nPlains %d\nForest: %d\nImpassable: %d\nTotal: %d\n", water, plains, forest, impass, total); }
    }
    else {
        total = water + plains + forest + impass;
        if (total != 100) {
            if (!use_stdout) { printf("\nTotal percentage is not 100; total is actually %d; rescaling...\n\n", total); }
        }
        if (water) { water = (water*100) / total; }
        if (plains) { plains = (plains*100) / total; }
        if (forest) { forest = (forest*100) / total; }
        if (impass) { impass = (impass*100) / total; }
        total = water +  plains + forest + impass;
        if (total < 100) {
            plains += (100 - total);
        }
        if (!use_stdout) { 
            printf ("New water percentage: %d\n",water);
            printf ("New plains percentage: %d\n",plains);
            printf ("New forest percentage: %d\n",forest);
            printf ("New impassable percentage: %d\n",impass);
        }
    }
    if (!width) {
        width = 25;
        if (!use_stdout) { printf("\nNo map width defined, using default of %d squares.\n", width); }
    }
    else {
        if (!use_stdout) { printf("\nUser defined map width: %d squares\n", width); }
    }
    if (!height) {
        height = 25;
        if (!use_stdout) { printf("No map height defined, using default of %d squares.\n", height); }
    }
    else {
        if (!use_stdout) { printf("User defined map height: %d squares\n", height); }
    }

    total = width * height;
    water = (water * total) / 100;
    plains = (plains * total) / 100;
    forest = (forest * total) / 100;
    impass = (impass * total) / 100;
    total = water +  plains + forest + impass;
    if (total < width * height) {
        plains += ((width * height) - total);
    }
    total = water + plains + forest + impass;
    water_pc = water;
    plains_pc = plains;
    forest_pc = forest;
    impass_pc = impass;
    if (!use_stdout) { printf("\nWater squares: %d\nPlains squares: %d\nForest squares: %d\nImpassable squares: %d\nTotal squares: %d\n", water, plains, forest, impass, total); }
    fprintf(output_file, "%d,%d;\n", width, height);
    map = new struct Terrain_Square *[height];
    for (i = 0;i < height; i++) {
        map[i] = new struct Terrain_Square [width];
    }
    for (i = 0;i < height; i++) {
        for (j = 0; j < width; j++) {
            okay = 0;
	    num_avg = 0;
	    avg_alt = 0;
	    if (i>0) {
		num_avg++;
	        avg_alt += map[i-1][j].altitude;
	        if (j>0) {
		    num_avg++;
		    avg_alt += map[i-1][j-1].altitude;
		}
		if (j<(height-1)) {
		    num_avg++;
		    avg_alt += map[i-1][j+1].altitude;		    
		}
            }
	    if (j>0) {
	        num_avg++;
		avg_alt += map[i][j-1].altitude;
	    }
	    if (!avg_alt) {
	        avg_alt = rand() % 8;
	    }
	    else {
	        avg_alt = avg_alt / num_avg;
            }
	    if (!(rand() % 2)) {
                avg_alt += rand() % 3;
	    }
	    else {
	        avg_alt -= rand() % 3;
            }	    
	    if (avg_alt < 0) { avg_alt = 0; }
	    if (avg_alt > 7) { avg_alt = 7; }
            altitude = avg_alt;

            food = rand() % 255;

            terrain = 0;
	    level = 0;
	    terrain_set = 0;
	    terrain_level = rand() % total;
	    level += plains_pc;
	    if (terrain_level < level){
	         terrain = 4;
	         terrain_set = -1;
	    }
	    else { level += water_pc; }
	    if ((!terrain_set) && (terrain_level < level)){
	         terrain = 2;
	         terrain_set = -1;
	    }
	    else { level += forest_pc; }
	    if ((!terrain_set) && (terrain_level < level)){
	         terrain = 1;
	         terrain_set = -1;
	    }
	    else { level += impass_pc; }
	    if ((!terrain_set) && (terrain_level < level)){
	         terrain = 0;
	         terrain_set = -1;
	    }
            /* next, we check what the squares above and above right contain.
            if they have two the same, we put terrain to that type.
            otherwise, set terrain to the previous type.
            the type might not be allowed in the switch statement
            (that type might have been exhausted). */
            if (((rand() % 6) > 0) && (i>0) && (j>0)) { // ~0.66 chance
                if (map[i-1][j].terrain == map[i-1][j+1].terrain) {
                    map[i][j].terrain = map[i-1][j].terrain;
                    terrain = map[i][j].terrain;
                }
                else {
                    terrain = map[i][j-1].terrain;
                }
            }
            switch (terrain) {
                case 4: if (plains > 0) { plains--; okay = -1; }
                        else { if (j>0) j--; }
                        break;
                case 2: if (water > 0) { water--; okay = -1; }
                        else { if (j>0) j--; }
                        break;
                case 1: if (forest > 0) { forest--; okay = -1; }
                        else { if (j > 0) j--; }
                        break;
                case 0: if (impass > 0) { impass--; okay = -1; }
                        else { if (j > 0) j--; }
                        break;
            }
	    if (okay) { fprintf(output_file, "%d,%d;", terrain + (altitude * 16), food); }
	    map[i][j].terrain = (signed int) terrain;
	    map[i][j].altitude = (signed int) altitude;
        }
        fprintf(output_file, "\n");
    }
    if (output_file != stdout) {
        if (fclose(output_file) != 0) {
            printf("Could not close %s.\n", output_choice);
        }
    }
    return 0;
}
