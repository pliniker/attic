// Some Assembler routines

void PushRegs()
{
	asm {
		push ax;
		push bx;
		push cx;
		push dx;
	}
}

void PopRegs()
{
	asm {
		pop  ax;
		pop  bx;
		pop  cx;
		pop  dx;
	     }
}
