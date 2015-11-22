

#include "colours.h"



void ColoursInit()
{
	gdk_rgb_init();
}



guint32 CalcColour(unsigned char c)
{
	guint32 temp = 0;
	
	if (c & 1)  temp |= c_herb;
	if (c & 2)  temp |= c_carn;
	if (c & 4)  temp |= c_legs;
	if (c & 8)  temp |= c_fins;
	if (c & 16)	temp |= c_wings;
	if (c & 32)	temp |= c_breed;
	if (c & 64) temp |= c_look;
	return temp;
}



ColourMap::ColourMap(	unsigned char r1, unsigned char g1, unsigned char b1,
				unsigned char r2, unsigned char g2, unsigned char b2)			
{
	float ci, cx;
	unsigned int index;

	ci = (float) (r2 - r1) / 256.0;
	index = 0;
	cx = (float) r1;
	while (index != 256) {
		colours[index] = ( (gushort) cx ) << 16;
		
		cx += ci;
		index++;
	}

	ci = (float) (g2 - g1) / 256.0;
	index = 0;
	cx = (float) g1;
	while (index != 256) {
		colours[index] |= ( (gushort) cx ) << 8;
		
		cx += ci;
		index++;
	}

	ci = (float) (b2 - b1) / 256.0;
	index = 0;
	cx = (float) b1;
	while (index != 256) {
		colours[index] |= (gushort) cx;
		
		cx += ci;
		index++;
	}
}

ColourMap::ColourMap(	unsigned char r1, unsigned char g1, unsigned char b1,
			unsigned char r2, unsigned char g2, unsigned char b2,
			unsigned char r3, unsigned char g3, unsigned char b3)	
{
	float ci, cx;
	unsigned int index;

	ci = (float) (r2 - r1) / 128.0;
	index = 0;
	cx = (float) r1;
	while (index != 128) {
		colours[index] = ( (gushort) cx ) << 16;
		
		cx += ci;
		index++;
	}

	ci = (float) (g2 - g1) / 128.0;
	index = 0;
	cx = (float) g1;
	while (index != 128) {
		colours[index] |= ( (gushort) cx ) << 8;
		
		cx += ci;
		index++;
	}

	ci = (float) (b2 - b1) / 128.0;
	index = 0;
	cx = (float) b1;
	while (index != 128) {
		colours[index] |= (gushort) cx;
		
		cx += ci;
		index++;
	}


	ci = (float) (r3 - r2) / 128.0;
	index = 128;
	cx = (float) r2;
	while (index != 256) {
		colours[index] = ( (gushort) cx ) << 16;
		
		cx += ci;
		index++;
	}

	ci = (float) (g3 - g2) / 128.0;
	index = 128;
	cx = (float) g2;
	while (index != 256) {
		colours[index] |= ( (gushort) cx ) << 8;
		
		cx += ci;
		index++;
	}

	ci = (float) (b3 - b2) / 128.0;
	index = 128;
	cx = (float) b2;
	while (index != 256) {
		colours[index] |= (gushort) cx;
		
		cx += ci;
		index++;
	}

}
			
guint32 ColourMap::GetColour(unsigned char index)
{
	return colours[index];
}


