/* Defines of network messages
ver 0.9
17/6/99		*/

// client to server msgs
#define QUIT		127
#define BUFFER		7
#define PAUSE		9
#define START		10
#define SPEED		11
#define NEWC		32
#define NEWH		33
#define STATS_REQ	40
#define ACK		49
#define SACK		50 // stats ACK..... :o)
#define NEWCRIT         51 // x,y coords, index into list

// server to client msgs
#define REDRAW		128
#define MAP_START	132
#define MAP		129
#define MAP_SIZE	130
#define TERRAIN		131
#define STATS		200
#define CRIT_START      209
#define CRIT_DATA       210
#define CRIT_END        211
#define BACK		140

// terrain types
#define PLAIN		4
#define WATER		2
#define FOREST		1
#define IMPASSABLE	0

// critter type:
// meaning		bit
// herbivore		0
// carnivore		1
// legs			2
// fins			3
// wings		4
// breed		5
// look			6

// stats types
// these are sent as high byte/low byte pairs
#define GENERATIONS	0	// sent as 4 chars (order: high to low significance...)
#define CARNS		1	// number of carnivores 
#define HERBS		2	// number of herbivores
#define OMNIS		3	// number of omnivores (all sent as 2 chars, as per CRITTERS)
#define CRITTERS	4	// send as 2 chars (order: high sig. low sig.)
#define LEGS		5
#define FINS		6
#define WINGS		7
#define SEGS		8	// 1 char