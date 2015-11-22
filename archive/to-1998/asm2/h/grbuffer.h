// graphics buffer prog
#include <mem.h>
#define VGABUFFER 0xA000

char _buf[64000];

void _ViewScreen(char near *buffer)
{
	movedata(_DS, (unsigned)buffer,VGABUFFER, 0, 64000);
}
