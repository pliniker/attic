
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "colours.h"
#include "windows.h"
#include "map.h"
#include "stats.h"
#include "../common/network.h"
#include "../common/gthreads.h"
#include "../common/pthreads.h"
#include "draw.h"




int window_x = DRAW_SIZE * TILE_SIZE;		// map window width
int window_y = DRAW_SIZE * TILE_SIZE;		// map window height

unsigned char map_width = 0;
unsigned char map_height = 0;
unsigned char tile_size = TILE_SIZE;		// tile width & height

int DrawFlag = 0;				// set this flag to 1 to redraw the map at any time
int DrawEnable = 0;
int UpdateFlag = 0;				// set this to 1 to get new data
int PauseFlag = 0;				// set this to 1 to pause getting new data
int SpeedFlag = 0;
int UpdateSpeed = 0;
int TerrAltFlag = 0;
int GetStatsFlag = 0;
int AddCritterFlag = 0;
int AddCritterType = 0;
int TimerFlag = 0;
int AckFlag = 0;
int EnAck = 1;
int SAckFlag = 0;
int EnSack = 1;
int ReadStatsFlag = 0;
unsigned char Speed;
int RefreshRate = REFRESH;
int Counter = 0;

int ReadFlag = 0;
int SendFlag = 0;
Timer *draw_thread;
Thread *io_in_thread, *io_out_thread;

Network *client;				// network object for client
Map *map;					// our map
Tile *map_tile;					// one tile, whose attributes can be changed to draw
Tile *critter_tile;
ColourMap *map_terrain_colours[8];		// colour map for the tiles
ColourMap *map_altitude_colours;
ColourMap *critters_colours;

WindowMain *main_win;				// main window
Box *main_win_box;
Box *main_win_vbox;
Toggle *main_win_mapshow;			// show/hide map button
Button *main_win_quit;				// quit button
Toggle *main_win_pause;				// pause/unpause button
Toggle *main_win_stats;				// view global statistics button
Toggle *main_win_addc;				// add herbivore
Button *main_win_redraw;			// force a redraw
Button *main_win_terralt;			// show terrain type or altitude
Scrollbar *main_win_speed;			// engine speed
Label *main_win_speed_label;
Scrollbar *main_win_scale;
List *main_win_critters;

WindowSub *draw_win;				// map window
Box *draw_win_box;
Box *draw_win_vbox;
Draw *draw_win_drawarea;			// map drawing area
Scrollbar *draw_win_vscroll;
Scrollbar *draw_win_hscroll;

StatsWindow *statistics;			// statistics window


/******************************************************************************

	Button handlers

******************************************************************************/

// this function is the handler for the main_win_mapshow button
void main_win_mapshow_handler(GtkWidget *widget, gpointer data)
{
	if (draw_win->Status()) {
		draw_win->Hide();
		main_win_scale->Disable();
		EnAck = 0;
	} else {
		draw_win->Show();
		if (main_win_pause->GetState()) {
			main_win_scale->Enable();
		}
		EnAck = 1;
		DrawFlag = 1;
	}
}

// main_win_newc handler - spawns a new carnivore
void main_win_addc_handler(GtkWidget *widget, gpointer data)
{
 
}

// force a map redraw
void main_win_redraw_handler(GtkWidget *widget, gpointer data)
{
	UpdateFlag = 1;
	DrawFlag = 1;
	AckFlag = 1;
}

// pause button
void main_win_pause_handler(GtkWidget *widget, gpointer data)
{
	client->SendMsg(PAUSE, 0, 0);
	
	if (main_win_pause->GetState()) {
	  PauseFlag = 1;
		if (draw_win->Status()) {
			main_win_scale->Enable();
		}
	} else {
	  PauseFlag = 0;
		main_win_scale->Disable();
	}
}

// speed scrollbar
void main_win_speed_handler(GtkWidget *widget, gpointer data)
{
	Speed = 10 - main_win_speed->Value();
	SpeedFlag = 1;
}

// speed scrollbar
void draw_win_hscroll_handler(GtkWidget *widget, gpointer data)
{
	map->params.win_x = draw_win_hscroll->Value();
	UpdateFlag = 1;
}

void draw_win_vscroll_handler(GtkWidget *widget, gpointer data)
{
	map->params.win_y = draw_win_vscroll->Value();
	UpdateFlag = 1;
}

void main_win_terralt_handler(GtkWidget *widget, gpointer data)
{
	switch (TerrAltFlag) {
	case 0:	TerrAltFlag = 1;
		break;
	case 1: TerrAltFlag = 2;
		break;
	case 2: TerrAltFlag = 0;
		break;
	}
	
	DrawFlag = 1;
}

// this function is the handler for the main_win_mapshow button
void main_win_stats_handler(GtkWidget *widget, gpointer data)
{
	if (statistics->Status()) {
		statistics->Hide();
		EnSack = 0;
	} else {
		statistics->Show();
		EnSack = 1;
	}
}


// resize the map window handler
void draw_win_resize()
{
	int flag = 0;
	int newx = draw_win->GetX(), newy = draw_win->GetY();
	
	if (newx != window_x) {
		window_x = newx;
		map->params.win_size_x = newx / tile_size - (TILE_SIZE / tile_size);
		
		if (map->params.win_size_x > map->params.map_size_x) {
			map->params.win_size_x = map->params.map_size_x;
		}

		draw_win_hscroll->SetMax(map_width - map->params.win_size_x + 2);
		flag = 1;
	}
	
	if (newy != window_y) {
		window_y = newy;
		map->params.win_size_y = newy / tile_size - (TILE_SIZE / tile_size);
		
		if (map->params.win_size_y > map->params.map_size_y) {
			map->params.win_size_y = map->params.map_size_y;
		}
		
		draw_win_vscroll->SetMax(map_height - map->params.win_size_y + 2);
		flag = 1;
	}

	if (flag) { 
		draw_win_drawarea->SetSize( (map->params.win_size_x) * tile_size, 
									(map->params.win_size_y) * tile_size);
		UpdateFlag = 1;
	}
}


// tile scaling scrollbar handler
void main_win_scale_handler(GtkWidget *widget, gpointer data)
{
	int Flag = 0;
	
	tile_size = main_win_scale->Value();
	
	map->params.win_size_x = window_x / tile_size - (TILE_SIZE / tile_size);
	if (map->params.win_size_x > map->params.map_size_x)
		map->params.win_size_x = map->params.map_size_x;
	
	map->params.win_size_y = window_y / tile_size - (TILE_SIZE / tile_size);
	if (map->params.win_size_y > map->params.map_size_y)
		map->params.win_size_y = map->params.map_size_y;
	

	if ((map->params.win_size_x * tile_size) < draw_win_drawarea->GetX()) {
		Flag = 1;
	}
		
	if ((map->params.win_size_y * tile_size) < draw_win_drawarea->GetY()) {
		Flag = 1;
	}
		
	if (Flag) {
		draw_win_drawarea->SetSize( map->params.win_size_x * tile_size, 
			map->params.win_size_y * tile_size);
	}

	map->params.win_x = 0;
	map->params.win_y = 0;

	draw_win_hscroll->SetMax(map_width - map->params.win_size_x + 2);
	draw_win_vscroll->SetMax(map_height - map->params.win_size_y + 2); 
	
	critter_tile->SetSize(tile_size / 2, tile_size / 2);
	map_tile->SetSize(tile_size, tile_size);
	UpdateFlag = 1;
	
}

// handles when the user clicks inside the draw area
static gint draw_win_drawarea_handler(GtkWidget *widget, GdkEventButton *event)
{
	unsigned char type[3];
	if ((event->button == 1) && (main_win_addc->GetState())) {
		type[0] = (unsigned char) (event->x / tile_size + map->params.win_x);
		type[1] = (unsigned char) (event->y / tile_size + map->params.win_y);
		type[2] = (unsigned char) AddCritterType;
		client->SendMsg(NEWCRIT, 3, type);
		//main_win_addc->SetState(OFF);
		//printf("x,y = %i,%i\n", type[0], type[1]);
		UpdateFlag = 1;
  	}
	return TRUE;
}


void main_win_critters_handler(GtkWidget      *clist,
		    			gint            row,
		    			gint            column,
		    			GdkEventButton *event,
		    			gpointer        data )
{
  AddCritterType = (int) row;
//  printf("row=%i ", row);
  return;
}

// is called from Draw - does any necy cleaning up
void draw_win_various_things()
{
}

/******************************************************************************

	Initialise widgets & show them

******************************************************************************/

// initialise all windows buttons and widgets
void InitWindows()
{
	printf("CLIENT: creating widgets...");
	ColoursInit();
	
	// main window containing controls
	main_win = new WindowMain("Critters Client Control", 130, 340, 3);
	main_win_box = new Box(VERT, FALSE, FALSE);
	main_win_vbox = new Box(HORIZ, FALSE, FALSE);
	main_win->Add(main_win_vbox);
	
	// main window's buttons
	main_win_mapshow = new Toggle("Show/Hide Map");
	main_win_quit = new Button("Quit");
	main_win_pause = new Toggle("Pause/Unpause");
	main_win_stats = new Toggle("Show/Hide Stats");
	main_win_addc = new Toggle("Add Critter");
	main_win_redraw = new Button("Redraw");
	main_win_terralt = new Button("Food/Terrain/Alt");
	main_win_speed = new Scrollbar(VERT, 0.0, 11.0);
	main_win_scale = new Scrollbar(HORIZ, 8.0, 33.0);
	main_win_scale->Set(32);
	main_win_speed->Set(10);
	main_win_critters = new List(12);
	main_win_critters->Resize(110, 180);
	
	main_win_pause->SetState(ON);
	main_win_mapshow->SetState(ON);
	main_win_stats->SetState(ON);
	main_win_addc->SetState(OFF);
		
	// add the buttons to the main window
	main_win_box->Pack(main_win_mapshow);
	main_win_box->Pack(main_win_stats);
	main_win_box->Pack(main_win_addc);
	main_win_box->Pack(main_win_terralt);
	main_win_box->Pack(main_win_pause);
	main_win_box->Pack(main_win_quit);
	main_win_box->Pack(main_win_redraw);
	main_win_box->Pack(main_win_scale);
	main_win_box->Pack(main_win_critters);

	main_win_vbox->Pack(main_win_box);
	main_win_vbox->Pack(main_win_speed);
	
	// set the event handlers for each button
	main_win_mapshow->SetHandler((void *) main_win_mapshow_handler);
	main_win_quit->SetHandler((void *) AppKiller);
	main_win_addc->SetHandler((void *) main_win_addc_handler);
	main_win_redraw->SetHandler((void *) main_win_redraw_handler);
	main_win_pause->SetHandler((void *) main_win_pause_handler);
	main_win_speed->SetHandler((void *) main_win_speed_handler);
	main_win_terralt->SetHandler((void *) main_win_terralt_handler);
	main_win_stats->SetHandler((void *) main_win_stats_handler);
	main_win_scale->SetHandler((void *) main_win_scale_handler);
	main_win_critters->SetHandler((void *) main_win_critters_handler);

	// create the stats window
	statistics = new StatsWindow();
	
	// map window initialisation
	draw_win = new WindowSub("Map", window_x + 40, window_y + 40);
	draw_win_box = new Box(HORIZ, FALSE, FALSE);
	draw_win_vbox = new Box(VERT, FALSE, FALSE);
	draw_win_hscroll = new Scrollbar(HORIZ, 0.0, 2.0);
	draw_win_vscroll = new Scrollbar(VERT, 0.0, 2.0);
	draw_win_drawarea = new Draw(window_x, window_y);
	draw_win_box->Pack(draw_win_drawarea);
	draw_win_box->Pack(draw_win_vscroll);
	draw_win_vbox->Pack(draw_win_box);
	draw_win_vbox->Pack(draw_win_hscroll);
	draw_win->Add(draw_win_vbox);
	
	draw_win_hscroll->SetHandler((void *) draw_win_hscroll_handler);
	draw_win_vscroll->SetHandler((void *) draw_win_vscroll_handler);
	draw_win_drawarea->SetHandler((void *) draw_win_drawarea_handler);
	
	main_win->Show();
	draw_win->Show();
	
	printf("done\n");
}


/******************************************************************************

	create the map storage buffer
	create a tile colourmap

******************************************************************************/

// initialise the map - set the size and reset all values
void InitMap()
{
	printf("CLIENT: creating colourmaps...");
	// create a new rgb colour map for the ground tiles
	// colours starting from light brown merging to pure green

	map_terrain_colours[PLAIN] = new ColourMap(0x9F,0x5F,0, 0x7F,0x8F,0x40);
	map_terrain_colours[WATER] = new ColourMap(0,0,0x7F, 0,0,0xFF);
	map_terrain_colours[FOREST] = new ColourMap(60,70,10, 10,0x7F,10);
	#define GREY 0x404040
	map_altitude_colours = new ColourMap(0x40,0x40,0X40, 0xF0,0xF0,0XF0);
	critters_colours = new ColourMap(0xFF,0x7F,0xFF, 0x7F,0xFF,0xFF);

	printf("done\n");
	
	printf("CLIENT: creating map tiles...");
	// create the map tile for drawing on the drawarea
	map_tile = new Tile(draw_win_drawarea, tile_size, tile_size);
	critter_tile = new Tile(draw_win_drawarea, 16, 16);
	
	printf("done\n");
}


/******************************************************************************

	Map redrawing thread and handlers

******************************************************************************/

// redraw the ground tiles. this should only be called from Draw()
void draw_ground()
{
	int x, y;
	int width = map->params.win_size_x, height = map->params.win_size_y;
	int xs = map->params.win_x, ys = map->params.win_y;

	for (y = ys; y < (height + ys); y++) {
		for (x = xs; x < (width + xs); x++){
			switch (map->buffer[x][y].terrain_type) {
			case PLAIN: map_tile->SetColour(map_terrain_colours[PLAIN]->
							GetColour(map->buffer[x][y].food_status));
						break;
			case WATER: map_tile->SetColour(map_terrain_colours[WATER]->
							GetColour(map->buffer[x][y].food_status));
						break;
			case FOREST: map_tile->SetColour(map_terrain_colours[FOREST]->
							 GetColour(map->buffer[x][y].food_status));
						break;
			case IMPASSABLE: map_tile->SetColour(GREY); break;
			default: map_tile->SetColour(0x0);
			}
			map_tile->Draw( (x-xs) * tile_size, (y-ys) * tile_size);
		}
	}
}

void draw_altitude()
{
	int x, y;
	int width = map->params.win_size_x, height = map->params.win_size_y;
	int xs = map->params.win_x, ys = map->params.win_y;

	for (y = ys; y < (height + ys); y++) {
		for (x = xs; x < (width + xs); x++){
			map_tile->SetColour(map_altitude_colours->
					    GetColour(map->buffer[x][y].altitude));
			map_tile->Draw( (x-xs) * tile_size, (y-ys) * tile_size);
		}
	}
}

void draw_terrain()
{
	int x, y;
	int width = map->params.win_size_x, height = map->params.win_size_y;
	int xs = map->params.win_x, ys = map->params.win_y;

	for (y = ys; y < (height + ys); y++) {
		for (x = xs; x < (width + xs); x++){
			switch (map->buffer[x][y].terrain_type) {
			case PLAIN: map_tile->SetColour(0x708030); break;
			case WATER: map_tile->SetColour(0x00207F); break;
			case FOREST: map_tile->SetColour(0x008000); break;
			case IMPASSABLE: map_tile->SetColour(GREY); break;
			default: map_tile->SetColour(0x0);
			}
			map_tile->Draw( (x-xs) * tile_size, (y-ys) * tile_size);
		}
	}
}

// redraw the critters. this should only be called from Draw(), and after draw_ground()
void draw_critters()
{
	int x, y;
	int width = map->params.win_size_x, height = map->params.win_size_y;
	int xs = map->params.win_x, ys = map->params.win_y;
	int offset = tile_size / 4
	;

	for (y = ys; y < (height + ys); y++) {
		for (x = xs; x < (width + xs); x++){
			if (map->buffer[x][y].critter.critter_type > 0) {
				critter_tile->SetColour(CalcColour(map->buffer[x][y].critter.critter_type));
				critter_tile->Draw( (x-xs) * tile_size + offset, (y-ys) * tile_size + offset);
			}
		}
	}
}



// map redrawing monitor thread. redraws the map every time it detects DrawFlag to be > 0
// this is called 20 times a second, for a MAXIMUM refresh rate of 20Hz (by default)
// see REFRESH in draw.h
int Draw(gpointer data)
{

	Counter++;
	if ((Counter > 10) && (!PauseFlag)) {
		GetStatsFlag = 1;
		Counter = 0;
	}

//	if (Counter > 2)
		TimerFlag = 1;
	
	if (ReadStatsFlag) {
	  statistics->Draw();
	  ReadStatsFlag = 0;
	}

	UpdateSpeed++;

	if ((UpdateSpeed > Speed/2) || (main_win_pause->GetState())) {
		if (DrawFlag) {
			DrawFlag = 0;
			switch (TerrAltFlag) {
			case 0:	draw_ground(); break;
			case 1: draw_terrain(); break; 
			case 2: draw_altitude(); break;
			}
	
			draw_critters();
			draw_win_drawarea->Update();
		}
		UpdateSpeed = 0;
	}
	
	draw_win_resize();
	draw_win_various_things();
	
	return TRUE;
}


/******************************************************************************

	Network IO thread & handlers

******************************************************************************/

unsigned char actual_x = 0, actual_y = 0;
unsigned char actual_width = DRAW_SIZE, actual_height = DRAW_SIZE;

// sends the BUFFER request
void send_buffer()
{
	unsigned char args[4];
	UpdateFlag = 0;
	
	// prevent too many BUFFERs / sec being sent & the engine not being able to keep up
	// necy because of some window managers window resizing opaque.
	if (TimerFlag) {
		args[0] = map->params.win_x;
		args[1] = map->params.win_y;
		args[2] = map->params.win_x + map->params.win_size_x;
		args[3] = map->params.win_y + map->params.win_size_y;
		client->SendMsg(BUFFER, 4, args);
	}
	
	TimerFlag = 0;
}

// handles the MAP info
void read_map()				
{
	int x, y;
	int width = actual_width, height = actual_height;
	int xs = actual_x, ys = actual_y;
	int i = 0, total, count = actual_width * actual_height * 2;
	unsigned char com, nargs, args[256];

	client->GetMsg(&com, &nargs, args);

	if (com != MAP) {
		printf("read_map(): WARNING: MAP data expected. not got\n");
		AckFlag = 1;
		return;
	}

	total = nargs;

	for (y = ys; y < (height + ys); y++) {
		for (x = xs; x < (width + xs); x++) {
			map->buffer[x][y].food_status = args[i++];
			map->buffer[x][y].critter.critter_type = args[i++];
			

			if ((i >= nargs) && (total < count)) {
				client->GetMsg(&com, &nargs, args);
			
				total += nargs;
				i = 0;
				
				if (com != MAP) {
					
					if (total != count) {
						printf("read_map(): WARNING: bytes read not equal to visible map area: %i/%i\n", total, count);
					}

					printf("read_map(): WARNING: non MAP data recvd: com=%i; nargs=%i\n", com, nargs); 
					printf("read_map(): attempting to regain sync\n");
					AckFlag = 1;
					return;
				}
			}
		}
	}
	
	if (total != count) {
		printf("read_map(): WARNING: bytes read not equal to visible map area: %i/%i\n", total, count);
	}

	AckFlag = 1;
}

// send request for stats
void send_statsreq()
{
	int i, total = 0;
	unsigned char args[VALUES];
	GetStatsFlag = 0;
	
	for (i = 0; i < VALUES; i++) {
		if (statistics->DataIsEnabled(i)) {
			args[total++] = i;
		}
	}
	
	if (total) {
		client->SendMsg(STATS_REQ, total, args);
        }
}

// reads statistics info from the engine
void read_stats(unsigned char *nargs, unsigned char *args)
{
  statistics->Update(args, (int) *nargs);
  SAckFlag = 1;
  ReadStatsFlag = 1;
}


// read the critters from the engine
void read_crits()
{
  unsigned char com, nargs, args[256];

  while (com != CRIT_END) {
    client->GetMsg(&com, &nargs, args);
    if (com == CRIT_DATA) {
      args[nargs] = 0;
      main_win_critters->Add(args);
    }
  }

}

// network IO monitor thread
void * IOinThread(void * arg)
{
	int msg_status;
	unsigned char com, nargs, args[256];
    const struct timespec wait = {0,1000};
	struct timespec remain;
	
	while (1) {
		while (ReadFlag) {
			msg_status = client->GetMsg(&com, &nargs, args);
			if (msg_status != -1) {	
				switch (com) {
				case MAP_START:	read_map(); // break;
				case REDRAW:	DrawFlag = 1; break;
				case STATS:    	read_stats(&nargs, args); break;
				case CRIT_START:read_crits(); break;
				case BACK:		actual_x = args[0]; actual_y = args[1]; 
								actual_width = args[2] - args[0]; 
								actual_height = args[3] - args[1]; break;
				default:       	msg_status = -1;
				}
			}

			if (msg_status == -1) {
				printf("IOinThread(): FATAL: client/engine sync error com=%i; msg_status=%i\n", com, msg_status);
				printf("IOinThread(): you won't get back into sync now! Oh well call it a day.\n");
				printf("IOinThread(): shutting down IO threads.\n");
				ReadFlag = 0;
				SendFlag = 0;
				client->SendMsg(QUIT, 0, 0);
				printf("IOinThread(): bailing out...\n");
				sleep(1);
				client->EndSession();
			}

			com = 0;
			nanosleep(&wait, &remain);
		}
	}
	
	printf("IOinThread(): WARNING: thread has been permanently disabled\n");
	while (1);
}

void * IOoutThread(void * arg)
{
	const struct timespec wait = {0,1000};
	struct timespec remain;
	
	while (1) {
		while (SendFlag) {

			if (UpdateFlag) {
				send_buffer();
			}
			
			if (SpeedFlag) {
				SpeedFlag = 0;
				client->SendMsg(SPEED, 1, &Speed);
			}
			
			if (GetStatsFlag) {
				send_statsreq();
			}
			
			if ((AckFlag) && (EnAck)) {
				AckFlag = 0;
				client->SendMsg(ACK, 0, 0);
			}
			
			if ((SAckFlag) && (EnSack)) {
				SAckFlag = 0;
				client->SendMsg(SACK, 0, 0);
			}

			nanosleep(&wait, &remain);
		}
	}
	
	printf("IOoutThread(): WARNING: thread has been permanently disabled\n");
	while (1);
}

// initialise threads
void InitThreads()
{
	printf("CLIENT: creating threads...\n");
	
	draw_thread = new Timer((void *)Draw, REFRESH);
	draw_thread->Start();
	
	io_in_thread = new Thread((void *)IOinThread);
	io_in_thread->Start();
	
	io_out_thread = new Thread((void *)IOoutThread);
	io_out_thread->Start();
	printf("\tdone\n");
	
	printf("CLIENT: starting threads...");
	SendFlag = 1;
	ReadFlag = 1;
	printf("done\n");
	
	client->SendMsg(START, 0, 0);
}


/******************************************************************************

	Start up a client network connection

******************************************************************************/

int read_size()
{
	unsigned char com, nargs, args[2];
	client->GetMsg(&com, &nargs, args);
	if (com != MAP_SIZE) {
		g_print("read_size(): FATAL: MAP_SIZE message expected. not got.\n");
		return 0;
	}
	if (nargs != 2) {
		printf("read_size(): FATAL: MAP_SIZE more than 2 arguments\n");
		return 0;
	}
	
	map_width = args[0];
	map_height = args[1];
	
	printf("\tdownload map size = (%i,%i)\n", map_width, map_height);
	printf("\tdownload data = %i bytes\n", map_width * map_height);
	return 1;
}

// handles the MAP info
void read_terrain()
{
	int x, y, i = 0, count = map_width * map_height, total = 0;
	unsigned char com, nargs, args[256];

	printf("\tdownloading map.");

	client->GetMsg(&com, &nargs, args);
	if (com != TERRAIN) {
		printf("read_terrain(): WARNING: non TERRAIN data recvd: com=%i; nargs=%i\n", com, nargs);
	}
	
	printf("args[0] = %i,%i,%i\n", (int) args[0], args[1], args[2]);
	
	total = nargs;
	
	for (y = 0; y < map_height; y++) {
		for (x = 0; x < map_width; x++) {
			map->buffer[x][y].terrain_type = args[i] & 0x0F;
			map->buffer[x][y].altitude = (args[i] & 0xF0);
			i++;
			
			if ((i >= nargs) && (total < count)) {
				client->GetMsg(&com, &nargs, args);
				total += nargs;

				if (com != TERRAIN) {
				
					if (total != count) {
						printf("read_terrain(): FATAL: bytes read not equal to visible map area: %i/%i\n", total, count);
					}

					printf("read_terrain(): WARNING: non TERRAIN data recvd: com=%i; nargs=%i\n", com, nargs);
					printf("read_terrain(): attempting to regain sync.\n");
					AckFlag = 1; 
					return; 
				}
				printf(".");
				i = 0;
			}
		}
	}
	printf("\n");
	printf("\tactual bytes read = %i\n", total);
	
	if (total != count) {
		printf("read_terrain(): FATAL: bytes read not equal to visible map area: %i/%i\n", total, count);
	}
	
	AckFlag = 1;
}

int InitNetwork(char *h)
{
	printf("CLIENT: creating network connection...\n");
	Speed = 0;
	
	client = new Network(h, BLOCK);
	client->StartClient();

	if (!read_size()) return 0;
	map = new Map(map_width, map_height, DRAW_SIZE, DRAW_SIZE);
	draw_win_hscroll->SetMax(map_width - DRAW_SIZE + 2);
	draw_win_vscroll->SetMax(map_height - DRAW_SIZE + 2);

	read_terrain();	

	printf("\tbuffer: intial data: (%i,%i)-(%i,%i)\n", 
		map->params.win_x, map->params.win_y,
		map->params.win_x + map->params.win_size_x,
		map->params.win_y + map->params.win_size_y);
	send_buffer();

	printf("\tspeed: initial data: %i\n", Speed);
	client->SendMsg(SPEED,1, &Speed);
	
	PauseFlag = 1;
	client->SendMsg(PAUSE, 0, 0);

	DrawEnable = 1;

	statistics->SetScale(map_height * map_width);

	printf("\tdone\n");
	return 1;
}
	

/******************************************************************************

	delete all objects
	close network

******************************************************************************/

// call this to deallocate all memory etc.
void ShutDown()
{
	SendFlag = 0;
	ReadFlag = 0;
	printf("CLIENT: shutting down...\n");

	sleep(1);

	printf("CLIENT: sending QUIT\n");
	client->SendMsg(QUIT, 0, 0);
	
	printf("\twaiting...\n");
	sleep(1);
	
	printf("\tending session...");
	client->EndSession();
	
	g_print("done\n");
}

