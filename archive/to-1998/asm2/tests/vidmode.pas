program Video_mode;

{
 needs svga bios.
 changes the video mode to mode specified in parameter.
}

uses dos,crt;

var
   regs : registers;
   mode : word;

begin
     if paramcount = 0 then
     begin
        writeln('Vid Mode - please give parameter');
        writeln('    $0          T40x25x16');
        writeln('    $1          T40x25x16');
        writeln('    $2          T80x25x16');
        writeln('    $3          T80x25x16');
        writeln('    $7          T80x25x2');
        writeln('    $54         T132x43x16');
        writeln('    $55         T132x25x16');
        writeln('    $109        T132x25x?');
        writeln('    $10A        T132x43x?');
        halt;
     end;

     val(paramstr(1),mode,mode);

     regs.ax := $4F02;
     regs.bx := mode;
     intr($10,regs);

     textcolor(0);
     textbackground(7);
     clrscr;
end.