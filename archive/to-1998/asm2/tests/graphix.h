// Graphics header file.

#include <dos.h>
#include <stdio.h>
#include <mem.h>

#define VIDEO 0x10
#define TEXT 0x03
#define VGALO 0x13
#define VGAHI 0x12

enum Boolean{FALSE = 0,TRUE};
char _Image[320][200];

void ScrollUp();
void ScrollDn();
void Rectangle(char _Color,int X1,int Y1,
				int X2,int Y2);
void PutDot(int _X,int _Y,char _Color);
char GetDot(int _X,int _Y);
void SetGraphMode(int _Mode);
Boolean LoadImage(char *ImgName);
Boolean ViewImage();
Boolean SaveImage(char ImgName[12],int _X1,int _Y1,int _X2,int _Y2);

/* .................................................................... */

// Scroll the current page up by 1 unit (controlled by the al register)
void ScrollUp()
{
// scroll up
	_AH = 0x07;
	_AL = 0x01;
	_BX = 0x0000;
	geninterrupt(VIDEO);
}

/* ....................................................................... */

// Scroll the current page down by 1 unit (see ScrollUp)
void ScrollDn()
{
// Scroll down
	_AH = 0x06;
	_AL = 0x01;
	_BX = 0x0000;
	geninterrupt(VIDEO);
}

/* ...................................................................... */

// Put a dot at position (X,Y) of color Color.
void PutDot(int _X,int _Y,char _Color)
{
// set the position
	_AH = 0x02;
	_CX = _X;
	_DX = _Y;
	geninterrupt(VIDEO);
// write the dot
	_AH = 0x0c;
	_AL = _Color;
	geninterrupt(VIDEO);
}

/* ...................................................................... */

// Read the color of the pixel at (X,Y)
char GetDot(int _X,int _Y)
{
// set position
	_AH = 0x02;
	_CX = _X;
	_DX = _Y;
	geninterrupt(VIDEO);
// read pixel color
	_AH = 0x0d;
	geninterrupt(VIDEO);
	return(_AL);
}

/* ...................................................................... */

// Change the video mode. Mode can be any value from 0..13. If you have
// an SVGA graphics card, you can use it. Mode = 0x03 is normal DOS text
// mode.
void SetGraphMode(int _Mode)
{
// change video mode
	_AH = 0x4f;
	_AL = 0x02;
	_BX = _Mode;
	geninterrupt(VIDEO);
}

/* ...................................................................... */

// Draw a rectangle on the screen.
void Rectangle(char _Color,int X1,int Y1,int X2,int Y2)
{
// Draw each of the 4 lines of the rectangle;
	unsigned int Ctr1 = X1;
	unsigned int Ctr2 = Y1;
	while (Ctr1 != X2)
	{
		PutDot(Ctr1,Ctr2,_Color);
		Ctr1++;
	}
	Ctr1 = X1;
	Ctr2 = Y2;
	while (Ctr1 != X2)
	{
		PutDot(Ctr1,Ctr2,_Color);
		Ctr1++;
	}
	Ctr1 = X1;
	Ctr2 = Y1;
	while (Ctr2 != Y2)
	{
		PutDot(Ctr1,Ctr2,_Color);
		Ctr2++;
	}
	Ctr1 = X2;
	Ctr2 = Y1;
	while (Ctr2 != Y2)
	{
		PutDot(Ctr1,Ctr2,_Color);
		Ctr2++;
	}
}

/* ...................................................................... */

Boolean ViewImage()
{
	char _Color;
	int _X,_Y;
	_X = 0;
	_Y = 0;
	do
	{
		do
		{
			_Color = _Image[_X][_Y];
			PutDot(_X,_Y,_Color);
			_X++;
		}
		while (_X <= 320);
		_Y++;
		_X = 0;
	}
	while (_Y <= 200);
	return(TRUE);
}

Boolean LoadImage(char *ImgName)
{
	char Img[12];
	char ctr = 0;
	while (*ImgName != 'e')
	{
		Img[ctr] = *ImgName;
		*ImgName++;
		ctr++;
	}
	FILE *in;
	if ((in = fopen(Img, "rt"))
	== NULL)
	{
		return(FALSE);
	}
	unsigned char RX1,RY1,RX2,RY2 , _Color;
	int _X1,_Y1,_X2,_Y2 , _X,_Y;
	RX1 = fgetc(in);	_X1 = RX1 * 2;
	RY1 = fgetc(in);	_Y1 = RY1;
	RX2 = fgetc(in);	_X2 = RX2 * 2;
	RY2 = fgetc(in);	_Y2 = RY2;
	_X = _X1;		_Y1 = _Y1;
	do
	{
		do
		{
			_Color = fgetc(in);
			_Image[_X][_Y] = _Color;
			PutDot(_X,_Y,_Color);
			_X++;
		}
		while (_X <= _X2);
		_X = _X1;
		_Y++;
	}
	while (_Y <= _Y2);
	fclose(in);
	return(TRUE);
}

Boolean SaveImage(char ImgName[12], int _X1,int _Y1,int _X2,int _Y2)
{
	FILE *out;
	if ((out = fopen(ImgName,"wt"))
	== NULL)
	{
		return(FALSE);
	}
	char WX1,WY1,WX2,WY2,_Color;
	int _X,_Y;
	WX1 = _X1 / 2;          fputc(WX1,out);
	WY1 = _Y1;		fputc(WY1,out);
	WX2 = _X2 / 2;		fputc(WX2,out);
	WY2 = _Y2;		fputc(WY2,out);
	_X = _X1;		_Y = _Y1;
	do
	{
		do
		{
			_Color = GetDot(_X,_Y);
			fputc(_Color,out);
			_X++;
		}
		while (_X < _X2+1);
		_X = _X1;
		_Y++;
	}
	while (_Y < _Y2+1);
	fclose(out);
	return(TRUE);
}

/* ....................................................................... */

