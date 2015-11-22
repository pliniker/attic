{
     Graphics Tool
     Copyright Peter Liniker

     Purpose: to access graphics modes not available using the provided
              GRAPH.TPU and to add some functions and procedures not
              included in it. Unfourtunately it cannot be used together
              with GRAPH because of the integrated way in which GRAPH
              is written.
}

{--------------------------------------------------------------------------}

Unit GPHTool;

INTERFACE

Procedure ScrollUp;
Procedure ScrollDn;
Procedure PutDot(X,Y,Color : Word);
Procedure GetDot(X,Y : Word);
Procedure GetColor(X,Y : Word);
Procedure PutChar(X,Y : Word;Color : Byte;s : String);
Procedure PutString(X,Y,Color : Word;OutPt : String);
Procedure Rectangle(X1,Y1,X2,Y2,Color:Word);
Procedure SetGraphMode(Mode : Word);

IMPLEMENTATION

Uses Dos;
Var
     Color : Byte;
     Xmin,Ymin,Xmax,Ymax : Word;

Procedure ScrollUp;
Var
     regs                 : registers;
Begin
     regs.AH := $07;
     regs.BH := $00;
     regs.BL := $00;
     regs.AL := $01;
     intr($10,regs);
End;

Procedure ScrollDn;
Var
     regs                 : registers;
Begin
     regs.AH := $06;
     regs.BL := $00;
     regs.BH := $00;
     regs.AL := $01;
     intr($10,regs);
End;

{------------------------------------------------------------------------}

Procedure PutDot(X,Y,Color : Word);
Var  regs               : registers;
Begin
     regs.ah := $02;
     regs.dx := y;
     regs.cx := x;
     intr($10,regs);
     regs.ah := $0c;
     regs.al := Color;
     intr($10,regs);
End;

Procedure GetColor(X,Y :Word);
Var regs                : registers;
Begin
     regs.ah := $02;
     regs.dx := x;
     regs.cx := y;
     intr($10,regs);
     regs.ah := $0d;
     intr($10,regs);
     Color := regs.al;
End;

Procedure GetDot(X,Y :Word);
Var regs                : registers;
Begin
     regs.ah := $02;
     regs.dx := x;
     regs.cx := y;
     intr($10,regs);
     regs.ah := $0d;
     intr($10,regs);
     Color := regs.bx;
End;

Procedure A(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure B(X,Y,Color : Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+3,COlor);
End;

Procedure C(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure D(X,Y,Color : Word);
Begin
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
End;

Procedure E(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure F(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X,Y+4,Color);
End;

Procedure G(X,Y,Color : Word);
Begin
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
End;

Procedure H(X,Y,Color :Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
     PutDot(X+3,Y,Color);
End;

Procedure I(X,Y,Color : Word);
Begin
     PUTdot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+2,Y+4,Color);
End;

Procedure J(X,Y,Color : Word);
Begin
     PUTdot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X,Y+4,Color);
End;

Procedure K(X,Y,Color : Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X+3,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+3,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure L(X,Y,Color : Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure M(X,Y,Color : Word);
Begin
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
End;

Procedure N(X,Y,Color : Word);
Begin
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
End;

Procedure O(X,Y,Color : Word);
Begin
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
End;

Procedure P(X,Y,Color :Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
End;

Procedure Q(X,Y,Color : Word);
Begin
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
End;

Procedure R(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+2,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+1,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+3,Y+4,Color);
End;

Procedure SA(X,Y,Color : Word);
Begin
     PutDot(X,Y+1,Color);
     PutDot(X,Y+4,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+3,Y,Color);
     PutDot(x+1,Y+2,Color);
     PutDot(X+2,Y+2,Color);
     PutDot(X+3,Y+3,Color);
     PutDot(X+1,Y+4,Color);
     PutDot(X+2,Y+4,Color);
End;

Procedure T(X,Y,Color : Word);
Begin
     PUTdot(X,Y,Color);
     PutDot(X+1,Y,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+1,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X+1,Y+4,Color);
End;

Procedure U(X,Y,Color : Word);
Begin
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
End;

Procedure V(X,Y,Color :Word);
Begin
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
End;

Procedure W(X,Y,Color : Word);
Begin
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
End;

Procedure XA(X,Y,Color : Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y+3,Color);
     PutDot(X+2,Y+4,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X,Y+3,Color);
     PutDot(X,Y+4,Color);
End;

Procedure YA(X,Y,Color : Word);
Begin
     PutDot(X,Y,Color);
     PutDot(X,Y+1,Color);
     PutDot(X+1,Y+2,Color);
     PutDot(X+2,Y,Color);
     PutDot(X+2,Y+1,Color);
     PutDot(X+1,Y+3,Color);
     PutDot(X,Y+4,Color);
End;

Procedure Z(X,Y,Color : Word);
Begin
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
End;

Procedure Comma(X,Y,Color : Word);
Begin
     PutDot(X,Y+4,COlor);
     PutDot(X,Y+5,Color);
     PutDot(X-1,Y+6,Color);
End;

Procedure FStop(X,Y,Color : Word);
Begin
     PutDot(X,Y+4,Color);
End;

Procedure Space;
Begin
End;


Procedure PutChar(X,Y : Word;Color : Byte;s : String);
Var   regs             : registers;
Begin
     If s = 'A' then A(X,Y,Color);
     If s = 'B' then B(X,Y,Color);
     If s = 'C' then C(X,Y,Color);
     If s = 'D' then D(X,Y,Color);
     If s = 'E' then E(X,Y,COlor);
     If s = 'F' then F(X,Y,Color);
     If s = 'G' then G(X,Y,Color);
     If s = 'H' then H(X,Y,Color);
     If s = 'I' then I(X,Y,Color);
     If s = 'J' then J(X,Y,Color);
     If s = 'K' then K(X,Y,Color);
     If s = 'L' then L(X,Y,Color);
     If s = 'M' then M(X,Y,COlor);
     If s = 'N' then N(X,Y,Color);
     If s = 'O' then O(X,Y,Color);
     If s = 'P' then P(X,Y,Color);
     If s = 'Q' then Q(X,Y,Color);
     If s = 'R' then R(X,Y,Color);
     If s = 'S' then SA(X,Y,Color);
     If s = 'T' then T(X,Y,Color);
     If s = 'U' then U(X,Y,Color);
     If s = 'V' then V(X,Y,Color);
     If s = 'W' then W(X,Y,Color);
     If s = 'X' then XA(X,Y,Color);
     If s = 'Y' then YA(X,Y,Color);
     If s = 'Z' then Z(X,Y,COlor);
     If s = ',' then Comma(X,Y,COlor);
     If s = '.' then FStop(X,Y,Color);
     If s = ' ' then Space;
End;

Procedure PutString(X,Y,Color:Word;Outpt:String);
Var  ctr          : Integer;
     s            : String;
     Xq           : Word;
Begin
     Ctr := 1;
     Xq := X;
     repeat
           s := Copy(Outpt,Ctr,1);
           PutChar(Xq,Y,Color,S);
           inc(Xq,5);
           inc(ctr);
     Until Ctr = length(outpt)+1;
End;

Procedure Rectangle(X1,Y1,X2,Y2,Color : Word);
Var  Ctr1,Ctr2          : Word;
Begin
     Ctr1 := X1;
     Ctr2 := Y1;
     repeat
           PutDot(Ctr1,Ctr2,Color);
           Inc(Ctr1);
     Until Ctr1 = X2;
     Ctr1 := X1;
     Ctr2 := Y2;
     repeat
           PutDot(Ctr1,Ctr2,Color);
           Inc(Ctr1);
     Until Ctr1 = X2;
     Ctr1 := X1;
     Ctr2 := Y1;
     repeat
           PutDot(Ctr1,Ctr2,Color);
           Inc(Ctr2);
     Until Ctr2 = Y2;
     Ctr1 := X2;
     Ctr2 := Y1;
     repeat
           PutDot(Ctr1,Ctr2,Color);
           Inc(Ctr2);
     Until Ctr2 = Y2;
End;

Procedure SetGraphMode(Mode : Word);
Var  regs               : registers;
Begin
     regs.ah := $4f;
     regs.al := $02;
     regs.bx := Mode;

     intr($10,regs);
End;

END.