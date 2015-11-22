


#define UNKNOWN 0
#define NO_CHANGE 1



struct Critter_Struct {
	bool exists;
	unsigned char size;
	unsigned char energy;
	unsigned char critter_type;
	unsigned char age;
};

struct Tile_Struct
{
        unsigned char terrain_type;
        unsigned char altitude;
	unsigned char food_status;
	Critter_Struct critter;
};

struct Map_Struct {
	unsigned char map_size_x, map_size_y;
	unsigned char win_size_x, win_size_y;
	unsigned char win_x, win_y;
};




class Map
{
public:
	struct Tile_Struct **buffer;
	struct Map_Struct params;
	
	
public:
	void create_map ();

//	void resize_window(	int window_pix_x, int window_pix_y);
					
//	void resize_buffer (int new_x,int new_y);
	

//	void scroll(int x, int y);
			
	void update_critter(int x, int y, bool exists,
					unsigned char size, unsigned char energy,
					unsigned char critter_type, unsigned char age);
					
	void update_terrain(int x, int y, bool critter_occupies,
		unsigned char density, unsigned char terrain_type);

	Map (unsigned char x, unsigned char y, unsigned char wx, unsigned char wy);

};