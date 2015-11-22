



#include <gtk/gtk.h>


#define C_HERB 0x00FFFF7F
#define C_CARN 0x00FFFFFF
#define C_OMNI 0x00FF7FFF
#define C_CRIT 0x00FF0000

#define c_herb	0x0000FF00
#define c_carn	0x000000FF
#define c_legs	0x007F0000
#define c_fins	0x00FF0000
#define c_wings	0x00FFFFFF
#define c_breed	0x00000000
#define c_look	0x00000000
#define c_segs  0x00007F7F

// meaning		bit
// herbivore		1
// carnivore		2
// legs			4
// fins			8
// wings		16
// breed		32
// look			64


void ColoursInit();


guint32 CalcColour(unsigned char c);


class ColourMap
{
private:
	guint32 colours[256];

public:
	ColourMap(	unsigned char r1, unsigned char g1, unsigned char b1,
			unsigned char r2, unsigned char g2, unsigned char b2);
	
	ColourMap(	unsigned char r1, unsigned char g1, unsigned char b1,
			unsigned char r2, unsigned char g2, unsigned char b2,
			unsigned char r3, unsigned char g3, unsigned char b3);
			
	guint32 GetColour(unsigned char index);
};
