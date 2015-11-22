// Header file for copyright

#include "grstring.h"

void _SetGraphMode(char Mode);
void CopyRight();

void _SetGraphMode(char Mode)
{
	asm {
		mov ah,0x00;
		mov al,Mode;
		int 0x10;
	}
}

void CopyRight()
{
	unsigned int ctr = 1;
	unsigned int num = 1;
	unsigned int var = 1;
	ctr = 1;
	num = 1;
	var = 1;
	while (num < 360)
	{
		while (ctr < 21000)
		{
			var = num - ctr * var / num;
			_buf[ctr] = var / 128 * 128;
			ctr++;
		}
		while (ctr < 42000)
		{
			var = num + ctr * var / num;
			_buf[ctr] = var / 128 *128;
			ctr++;
		}
		while (ctr < 64000)
		{
			var = num - ctr * var / num;
			_buf[ctr] = var / 128 * 128;
			ctr++;
		}
		_ViewScreen(_buf);
		PutString(num,-num,15,"C O P Y R I G H T    P E T E R    L I N I K E Re");
		PutString(num,102,15,"C O P Y R I G H T    P E T E R    L I N I K E Re");
		PutString(-num,num,15,"C O P Y R I G H T    P E T E R    L I N I K E Re");
		num++;
		ctr = 0;
	}
}
