// Routines used by an interrupt handler

void Routines()
{
	switch (_AH)
	{
	case 0x00 : {
//		PopRegs();
		_AX = 0xAAAA;
		_BX = 0xAAAA;
		_CX = 0xAAAA;
		_DX = 0xAAAA;
		break;
		}
	case 0x10 : {
//		PopRegs();
		PutDot(_CX,_DX,_AL);
		break;
		}
	case 0x11 : {
//		PopRegs();
		_AL = GetDot(_CX,_DX);
		break;
		}
	case 0x12 : {
//		PopRegs();
		Rectangle(_AL,_BX,_DH,_CX,_DL);
		break;
		}
	case 0x13 : {
//		PopRegs();
		SetGraphMode(_BX);
		break;
		}
	case 0x20 : {
//		PopRegs();
		_AL = GetKey();
		break;
		}
	case 0x21 : {
//		PopRegs();
		_AL = GetShiftStat();
		break;
		}
	}
}