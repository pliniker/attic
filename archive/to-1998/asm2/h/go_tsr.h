// TSR header file

#include <dos.h>
#define DOS 0x21

void GoTSR(char ReturnVal,int Paragraphs)
{
	_AH = 0x31;
	_AL = ReturnVal;
	_DX = Paragraphs;
	geninterrupt(DOS);
}
// the value of Paragraphs should be the size of the compiled
// program divided by 16