
/*		   Fixed Point Arithmetic Header File			*/

// numbers are in the format of a 32 bit number:
//	the 16 high bits are the integer part
//	the 16 lower bits make up the fraction
//	so 16.5 = 0010.1000h

// tells tc to use tasm for the assembly language, otherwise
// cannot handle 32bit regs
#pragma inline


// fixed point multiplication
long fpmul(num1,num2)
long num1,num2;
{
	long	result;
	asm{
		.386		// allows use of 32bit regs
		mov  eax,num1
		mov  edx,num2
		imul edx
		shrd eax,edx,16
		mov  result,eax
	}
	return result;
}

// fixed point division
long fpdiv(num1,num2)
long num1,num2;
{
	long	result;
	asm{
		.386
		mov  eax,num1
		and  eax,eax
		jns  FDP1
		inc  cx
		neg  eax
	}FDP1:			// labels unrecognized inside asm
	asm{
		sub  edx,edx
		rol  eax,16
		mov  dx,ax
		mov  ebx,num2
		and  ebx,ebx
		jns  FDP2
		dec  cx
		neg  ebx
	}FDP2:
	asm{
		div  ebx
		shr  ebx,1
		adc  ebx,0
		dec  ebx
		cmp  ebx,edx
		adc  eax,0
		and  cx,cx
		jz   FDP3
		neg  eax
	}FDP3:
	asm{
		mov  result,eax
	}
	return result;
}

// returns the square root of rootme
long fpsqrt (rootme)
long rootme;
{
	long	result;
	asm{
		.386
		mov  ebx,rootme
		xor  esi,esi
		xor  edx,edx
		mov  ecx,ebx
		mov  eax,ebx
	}iteration:
	asm{
		idiv  ebx
		xor  edx,edx
		add  eax,ebx
		shr  eax,1
		sub  esi,eax

		cmp  esi,1
		jbe  done

		mov  esi,eax
		mov  ebx,eax
		mov  eax,ecx

		jmp  iteration
	}done:
	asm{
		mov  result,eax
	}
	return result;
}

