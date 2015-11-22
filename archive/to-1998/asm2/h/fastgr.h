// fast graphics

#include <mem.h>
#include "grstring.h"

#define VGABUFFER 0xA000

char buf[64000];

void SetGraphMode(char Mode)
{
	asm {
		mov ah,0x00;
		mov al,Mode;
		int 0x10;
	}
}

void ViewScreen(char near *buffer)
{
	movedata(_DS, (unsigned)buffer,VGABUFFER, 0, 64000);
}

void Palette(char Color,char New)
{
	asm {
		mov ah,0x10;
		mov al,0;
		mov bl,Color;
		mov bh,New;
		int 0x10;
	}
}

int main(void)
{
	SetGraphMode(0x13);
	unsigned int ctr = 1;
	unsigned int num = 1;
	unsigned int var = 1;
	while (ctr < 14)
	{
		Palette(ctr,14);
		ctr++;
	}
	ctr = 1;
	num = 1;
	var = 1;
	while (num < 360)
	{
		while (ctr < 21000)
		{
			var = ctr + var / num;
			buf[ctr] = var;
			ctr++;
		}
		while (ctr < 42000)
		{
			var = var - ctr / num;
			buf[ctr] = var;
			ctr++;
		}
		while (ctr < 64000)
		{
			var = ctr - var / num;
			buf[ctr] = var;
			ctr++;
		}
		ViewScreen(buf);
		PutString(num,95,15,"C O P Y R I G H T    P E T E R    L I N I K E Re");
		num++;
		ctr = 0;
	}
	SetGraphMode(0x03);
	return 0;
}
