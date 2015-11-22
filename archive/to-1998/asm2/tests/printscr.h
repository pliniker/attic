// Print image header file

#include <dos.h>

#define PRINTSCREEN 0x05

void PrintScreen()
{
	geninterrupt(PRINTSCREEN);
}