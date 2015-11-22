program show_exe_data;

uses dos,crt;

var
     fname : string;
     fhandle : file of word;

     number : word;

begin
     if paramcount = 1 then
     begin

     fname := paramstr(1);

     assign(fhandle,fname);
{$I-}
     reset(fhandle);
{$I+}
     if ioresult = 0 then
     begin

     {clrscr;}
     textcolor(0);
     textbackground(7);
     writeln;
     writeln('     S H O W    E X E C U T A B L E     ');
     writeln;

     begin
          writeln('File name: ',fname);

          read(fhandle,number);
          writeln('Header: ',chr(lo(number)),chr(hi(number)));

          read(fhandle,number);
          writeln('Bytes in last page: ',number);

          read(fhandle,number);
          writeln('File size including header: ',number*512);

          read(fhandle,number);
          writeln('Relocation entries: ',number);

          read(fhandle,number);
          writeln('Header size: ',number*16);

          read(fhandle,number);
          writeln('Minimum bytes mem required above loaded program: ',number*16);

          read(fhandle,number);
          writeln('Maximum bytes required above loaded program: ',number*16);

          read(fhandle,number);
          writeln('Initial stack segment value before relocation: ',number);

          read(fhandle,number);
          writeln('Initial stack pointer value: ',number);

          read(fhandle,number);
          writeln('Negative sum of words in the file: ',number);

          read(fhandle,number);
          writeln('Initial instruction pointer value: ',number);

          read(fhandle,number);
          writeln('Initial code segment value before relocation: ',number);

          read(fhandle,number);
          writeln('Byte offset from beginning of file to relocation table: ',number);

          read(fhandle,number);
          writeln('Overlay number: ',number);
     end;

     close(fhandle);

     end;

     end;
end.