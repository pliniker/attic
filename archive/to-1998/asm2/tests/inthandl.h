// Interrupt handler header

#include <dos.h>

#ifdef __cplusplus
    #define __CPPARGS ...
#else
    #define __CPPARGS
#endif


void interrupt ( *oldhandler)(__CPPARGS);

void interrupt IntrHandler(__CPPARGS)
{
	Routines();
}

void ReplaceIntVects(unsigned char Intr)
{
	oldhandler = getvect(Intr);
	setvect(Intr, IntrHandler);
}
