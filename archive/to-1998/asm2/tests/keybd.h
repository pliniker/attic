// Keyboard input header file

#include <dos.h>
#define KEYBOARD 0x16
#define ON 1
#define OFF 0

void SetKbdClick(char OnOff)
{
	_AH = 0x04;
	_AL = OnOff;
	geninterrupt(KEYBOARD);
}

char GetKey()
{
	char ReturnVal;
	_AH = 0x10;
	geninterrupt(KEYBOARD);
	ReturnVal = _AL;
	return ReturnVal;
}

char PutKey(char Key,char Scan)
{
	_AH = 0x05;
	_CH = Scan;
	_CL = Key;
	geninterrupt(KEYBOARD);
	return _AL;
}
// returns AL:
//   00h = success
//   01h = buffer full

char GetShiftStat()
{
	char ReturnVal;
	_AH = 0x12;
	geninterrupt(KEYBOARD);
	ReturnVal = _AL;
	return ReturnVal;
// ret vals:
// 01 - right shift
// 02 - left shift
// 04 - left ctrl
// 08 - left alt
// 16 - scroll
// 32 - num
// 64 - caps
// 128 - insert
}

