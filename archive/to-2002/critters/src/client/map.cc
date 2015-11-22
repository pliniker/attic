/*

the resize thing is quite cute. resize_window changes the buffer_params
pixel parameters, then calls resize_uffer to handle the buffer recalc.
it should then call draw_map to update the screen.
if it's been made bigger than the previous buffer size then those squares
should be displayed as unknown.

oh, when a window is resized something should talk to the server and
tell it the size of the new buffer - i've provided procedures to get
this information as int. change it to char if you prefer, though you'll 
then also have to change the struct definition.

critters don't have uids. we can't track them on the client end without these.
i've put critters inside the squares and told the server to tell us for each 
square whether it has a critter, and if so to give us the critter information.


*/


#include "map.h"



int min(int x, int y)
{
	if (x < y) {
		return x;
	} else {
		return y;
	}
}

int max(int x, int y)
{
	if (x > y) {
		return x;
	} else {
		return y;
	}
}


Map::Map (unsigned char x, unsigned char y, unsigned char wx, unsigned char wy) {
	params.map_size_x = x;
	params.map_size_y = y;
	params.win_x = 0;
	params.win_y = 0;
	params.win_size_x = wx;
	params.win_size_y = wy;
	create_map();
}

void Map::create_map () {
	int i,j;
	buffer = new struct Tile_Struct *[params.map_size_x];
	for (i = 0; i < params.map_size_x; i++)
		buffer[i] = new struct Tile_Struct [params.map_size_y];
		
	for (i = 0; i < params.map_size_x; i++) {
		for (j = 0; j < params.map_size_y; j++) {
			buffer[i][j].terrain_type = UNKNOWN;
			buffer[i][j].food_status = 0;
			buffer[i][j].critter.exists = false;
		}
	}
}



//gtk window_resize method calls resize_window, then draw_map
/*
void Map::resize_window(int window_pix_x, int window_pix_y) {

	buffer_params.window_pix_height = window_pix_y;
	buffer_params.window_pix_width = window_pix_x;

	buffer_params.window_tile_height = buffer_params.window_pix_height / 
		buffer_params.window_tile_height;
		
	if ((buffer_params.window_pix_height % buffer_params.window_tile_height) != 0) {
		buffer_params.window_tile_height++;
	}
	
	buffer_params.window_tile_width = buffer_params.window_pix_width / 
		buffer_params.window_tile_width;
		
	if ((buffer_params.window_pix_width % buffer_params.window_tile_width) != 0) {
		buffer_params.window_tile_width++;
	}

	resize_buffer(	buffer_params.window_tile_height*2, 
				buffer_params.window_tile_height*2);
}


void Map::resize_buffer(int new_x,int new_y) {

	int i,j;
	struct Map_Square_Struct **new_map_ptr = 0;
	create_map (new_x, new_y, new_map_ptr);

	for (i=0;i<(min(new_x,buffer_params.buffer_tile_width));i++) {
		for (j=0;j<(min(new_y,buffer_params.buffer_tile_height));j++)
			new_map_ptr[i][j] = buffer[i][j];
	}
	buffer = new_map_ptr;
	buffer_params.buffer_tile_width += new_x;
	buffer_params.buffer_tile_height += new_y;
}
*/



//when a user scrolls, the gtk scroll method should first call this to update the buffer
//x and y can be any valid int.
//a negative int means move to the left or up.
//a positive int means move to the right or down.
//then, the gtk method should call draw_map
//then, the gtk method should get the network to send a message
//to the server with new buffer's-place-in-the-world coords.
//network must not update these coords in buffer_params, scroll does that.
//scroll will not call either of these procedures.
/*
void Map::scroll(int x, int y) {
	int i,j;

	buffer_params.window_in_buffer_x += x;
	buffer_params.window_in_buffer_y += y;
	buffer_params.buffer_in_world_x += x;
	buffer_params.buffer_in_world_y += y;
	for (i=buffer_params.window_in_buffer_x;i<buffer_params.window_tile_width;i++) {
		for (j=buffer_params.window_in_buffer_y;j<buffer_params.window_tile_height;j++)
			buffer[i][j].changed = true;
	}
}
*/

//network always calls update_critter for every square.
//if no critter information has been sent for a square,
//it sets exists to false and passes zero for all the rest.

//pete: at some later stage I will implement the critter characteristics
//an array of name&value pairs, but until then we'll just have the most
//obvious characteristics hardwired into the struct.
//at the moment, this means that you will have to use hardwired params
/*
void Map::update_critter(int x, int y, bool exists,
		unsigned char size, unsigned char energy,
		unsigned char critter_type, unsigned char age){
		//change to buffer_offset
	buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter_occupies = true;
	buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter.exists = exists;
	if (exists) { //might occasionally save a couple of cycles here...
		buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter.energy = energy;
		buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter.size = size;
		buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter.critter_type = critter_type;
		buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter.age = age;
	}
}

void Map::update_terrain(int x, int y, bool critter_occupies,
		unsigned char density, unsigned char terrain_type){
	buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].critter_occupies = critter_occupies;
	if (buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].density != NO_CHANGE) { buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].density = density; }
	if (buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].terrain_type != NO_CHANGE) { buffer[x - buffer_params.buffer_in_world_x][y - buffer_params.buffer_in_world_y].terrain_type = terrain_type; }
}
*/