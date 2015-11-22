// Graphics string header file.

void PutDot(int X,int Y,char Color)
{
     asm {
     mov ah,0x0c;
     mov al,Color;
     mov cx,X;
     mov dx,Y;
     mov bh,0;
     int 0x10;
     }
}

void A(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
}

void B(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+3,Color);
}

void C(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+3,Y+4,Color);
}

void D(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void E(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
}

void F(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X,Y+4,Color);
}

void G(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void H(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
     PutDot(X+3,Y,Color);
}

void I(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void J(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X,Y+4,Color);
}

void K(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+3,Color);
     PutDot(X+3,Y+4,Color);
}

void L(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
}

void M(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+4,Y,Color);
     PutDot(X+4,Y+1,Color);
     PutDot(X+4,Y+2,Color);
     PutDot(X+4,Y+3,Color);
     PutDot(X+4,Y+4,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
}

void N(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+2,Y+2,Color);
}

void O(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void P(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
}

void Q(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+5,Color);
}

void R(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
}

void SA(int X,int Y,char Color)
{
     PutDot(X,Y+1,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void T(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
}

void U(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
}

void V(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
}

void W(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+4,Y,Color);
     PutDot(X+4,Y+1,Color);
     PutDot(X+4,Y+2,Color);
     PutDot(X+4,Y+3,Color);
     PutDot(X+4,Y+4,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+3,Color);
}

void XA(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+3,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
}

void YA(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X,Y+4,Color);
}

void Z(int X,int Y,char Color)
{
     PutDot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
}

void Comma(int X,int Y,char Color)
{
     PutDot(X,Y+4,Color);
     PutDot(X,Y+5,Color);
     PutDot(X-1,Y+6,Color);
}

void FStop(int X,int Y,char Color)
{
     PutDot(X,Y+4,Color);
}

void Space() {}


void PutChar(int X,int Y,char Color,char *s)
{
     if (*s == 'A') { A(X,Y,Color);}
     if (*s == 'B') { B(X,Y,Color);}
     if (*s == 'C') { C(X,Y,Color);}
     if (*s == 'D') { D(X,Y,Color);}
     if (*s == 'E') { E(X,Y,Color);}
     if (*s == 'F') { F(X,Y,Color);}
     if (*s == 'G') { G(X,Y,Color);}
     if (*s == 'H') { H(X,Y,Color);}
     if (*s == 'I') { I(X,Y,Color);}
     if (*s == 'J') { J(X,Y,Color);}
     if (*s == 'K') { K(X,Y,Color);}
     if (*s == 'L') { L(X,Y,Color);}
     if (*s == 'M') { M(X,Y,Color);}
     if (*s == 'N') { N(X,Y,Color);}
     if (*s == 'O') { O(X,Y,Color);}
     if (*s == 'P') { P(X,Y,Color);}
     if (*s == 'Q') { Q(X,Y,Color);}
     if (*s == 'R') { R(X,Y,Color);}
     if (*s == 'S') { SA(X,Y,Color);}
     if (*s == 'T') { T(X,Y,Color);}
     if (*s == 'U') { U(X,Y,Color);}
     if (*s == 'V') { V(X,Y,Color);}
     if (*s == 'W') { W(X,Y,Color);}
     if (*s == 'X') { XA(X,Y,Color);}
     if (*s == 'Y') { YA(X,Y,Color);}
     if (*s == 'Z') { Z(X,Y,Color);}
     if (*s == ',') { Comma(X,Y,Color);}
     if (*s == '.') { FStop(X,Y,Color);}
     if (*s == ' ') { Space;}
}

void PutString(int X,int Y,char Color,char *String)
{
	int Ctr,Xq;
	Xq = X;
     do
     {
	   PutChar(Xq,Y,Color,String);
	   Xq = Xq + 5;
	   *String++;
     }
     while (*String != 'e');
}
