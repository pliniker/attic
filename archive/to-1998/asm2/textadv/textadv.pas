
{
 This unit should allow text adventures to be created quite easiy
}

unit textadv;

interface

uses crt;

const
     Y = True;
     N = False;
     X = 255;
     U = 254;
     get = 1;
     drop = 2;
     use = 3;
     shoot = 4;
     exam = 5;
     read = 6;
     inv = 7;

type
     locations = record
             description : string[80];
             number : byte;
             north : byte;
             south : byte;
             east : byte;
             west : byte;
             end;

     useobjects = record
             number1 : byte;
             number2 : byte;
             description : string[80];
             location1 : byte;
             location2 : byte;
             north : byte;
             south : byte;
             east : byte;
             west : byte;
             end;

     objects = record
             description : string[40];
             identifier : string[10];
             number : byte;
             location : byte;
             action : array[1..6] of boolean;
             examine : string[160];
             end;

     invent = record
             items : array[1..5] of byte;
             end;

var
     location : array[1..127] of locations;
     item     : array[1..127] of objects;
     useobj   : array[1..127] of useobjects;
     inventory: invent;

     currentlocation : byte;


procedure resetadv;
procedure newuse(num,num1,num2 : byte; description : string;
                 loc1,loc2 : byte; n,s,e,w : byte);
procedure newlocation(description : string;num : byte;
                                  north,south,east,west : byte);
procedure newobject(description : string;num : byte;
                    identifier : string;location : byte;
                    action1,action2,action3,
                    action4,action5,action6 : boolean;
                    examine : string);
procedure viewlocation(num : byte);
procedure actions;

implementation

{
 reset all vars
}
procedure resetadv;
var
   ctr :byte;
begin
     ctr := 1;
     repeat
           inventory.items[ctr] := X;
           inc(ctr);
     until ctr = 6;
     ctr := 1;
     repeat
           location[ctr].description := '                                                                                ';
           location[ctr].number := X;
           location[ctr].north := X;
           location[ctr].south := X;
           location[ctr].east := X;
           location[ctr].west := X;
           item[ctr].description := '                                        ';
           item[ctr].identifier := '          ';
           item[ctr].action[1] := N;
           item[ctr].action[2] := N;
           item[ctr].action[3] := N;
           item[ctr].action[4] := N;
           item[ctr].action[5] := N;
           item[ctr].action[6] := N;
           item[ctr].examine := '                                                                                '
                               +'                                                                                ';
           useobj[ctr].description := '                                                                                ';
           useobj[ctr].number1 := X;
           useobj[ctr].number2 := X;
           useobj[ctr].location1 := U;
           useobj[ctr].location2 := U;
           useobj[ctr].north := X;
           useobj[ctr].south := X;
           useobj[ctr].east := X;
           useobj[ctr].west := X;
           currentlocation := 1;
           inc(ctr);
     until ctr = 127;
end;

{
 New use
}
procedure newuse(num,num1,num2 : byte; description : string;
                 loc1,loc2 : byte; n,s,e,w : byte);
begin
     useobj[num].number1 := num1;
     useobj[num].number2 := num2;
     useobj[num].description := description;
     useobj[num].location1 := loc1;
     useobj[num].location2 := loc2;
     useobj[num].north := n;
     useobj[num].south := s;
     useobj[num].east := e;
     useobj[num].west := w;
end;

{
 Create a new location
}
procedure newlocation(description : string;num : byte;
                      north,south,east,west : byte);
begin
     location[num].description := description;
     location[num].number := num;
     location[num].north := north;
     location[num].south := south;
     location[num].east := east;
     location[num].west := west;
end;

{
 Create a new object
}
procedure newobject(description : string;num : byte;
                    identifier : string;location : byte;
                    action1,action2,action3,
                    action4,action5,action6 : boolean;
                    examine : string);
begin
     item[num].description := description;
     item[num].number := num;
     item[num].identifier := identifier;
     item[num].location := location;
     item[num].action[get] := action1;
     item[num].action[drop] := action2;
     item[num].action[use] := action3;
     item[num].action[shoot] := action4;
     item[num].action[exam] := action5;
     item[num].action[read] := action6;
     item[num].examine := examine;
end;

{
 View the location description and any objects there
}
Procedure viewlocation(num : byte);
var
     ctr : byte;
begin
     writeln( location[num].description );
     writeln('You can go: ');
     if not(location[num].north = X) then write('north ');
     if not(location[num].south = X) then write('south ');
     if not(location[num].east = X) then write('east ');
     if not(location[num].west = X) then write('west ');
     writeln;
     writeln;

     ctr := 1;
     repeat
           if item[ctr].location = currentlocation then
           begin
                writeln(item[ctr].description);
                {writeln(item[ctr].examine);}
           end;
           inc(ctr);
     until ctr = 127;
end;

{
 transfers an object to the inventory
}
procedure objtoinv(num : byte);
var
   ctr : byte;
begin
     ctr := 1;
     repeat
           if (inventory.items[ctr] = X) then
           begin
              inventory.items[ctr] := num;
              item[num].location := X;
              exit;
           end;
           inc(ctr);
     until ctr = 6;
end;

{
 transfers an object to the ground
}
procedure invtoobj(num : byte);
var
   ctr : byte;
begin
     ctr := 1;
     repeat
           if (inventory.items[ctr] = num) then
           begin
              inventory.items[ctr] := X;
              item[num].location := currentlocation;
              exit;
           end;
           inc(ctr);
     until ctr = 6;
end;

{
 View the result of 'exam'
}
procedure showexam(loc : byte;desc : string);
var
   ctr: byte;
begin
     ctr := 1;
     repeat
           if (item[ctr].identifier = desc) AND (item[ctr].location = loc) then
           begin
                if (item[ctr].action[exam] = Y) then writeln(item[ctr].examine)
                else writeln('It is very usual');
           end;
           inc(ctr);
     until ctr = 127;
end;

{
 Gets an object
}
procedure getobject(loc : byte;desc : string);
var
   ctr: byte;
begin
     ctr := 1;
     repeat
           if (item[ctr].identifier = desc) AND (item[ctr].location = loc) then
           begin
                if (item[ctr].action[get] = Y) then
                begin
                     item[ctr].action[get] := N;
                     item[ctr].action[drop] := Y;
                     objtoinv(ctr);
                end
                else writeln('Cant do that');
           end;
           inc(ctr);
     until ctr = 127;
end;

{
 Drops an object
}
procedure dropobject(loc : byte;desc : string);
var
   ctr: byte;
begin
     ctr := 1;
     repeat
           if (item[ctr].identifier = desc) AND (item[ctr].location = loc) then
           begin
                if (item[ctr].action[drop] = Y) then
                begin
                     item[ctr].action[get] := Y;
                     item[ctr].action[drop] := N;
                     invtoobj(ctr);
                end
                else writeln('Cant do that');
           end;
           inc(ctr);
     until ctr = 127;
end;

{
 use handler
}
procedure usethem(obj1,obj2 : string);
var
   ctr1,ctr2,ctr3,ctr4,ctr5 : byte;

   num1,num2 : byte;
begin
     ctr1 := 1;
     repeat
           if (item[inventory.items[ctr1]].number = inventory.items[ctr1]) AND
              (item[inventory.items[ctr1]].identifier = obj1) then

           begin
                num1 := inventory.items[ctr1];
                ctr2 := 1;
                repeat
                      if (item[inventory.items[ctr2]].number = inventory.items[ctr2]) AND
                         (item[inventory.items[ctr2]].identifier = obj2) then

                      begin
                           num2 := inventory.items[ctr2];
                           ctr3 := 1;
                           repeat
                                 if ((useobj[ctr3].number1 = num1) AND
                                    (useobj[ctr3].number2 = num2)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num1].location := useobj[ctr3].location1;
                                         item[num2].location := useobj[ctr3].location2;
                                         inventory.items[ctr1] := X;
                                         inventory.items[ctr2] := X;
                                         item[num1].action[get] := Y;
                                         item[num2].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;
                                 if ((useobj[ctr3].number1 = num2) AND
                                    (useobj[ctr3].number2 = num1)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num2].location := useobj[ctr3].location1;
                                         item[num1].location := useobj[ctr3].location2;
                                         inventory.items[ctr2] := X;
                                         inventory.items[ctr1] := X;
                                         item[num2].action[get] := Y;
                                         item[num1].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;

                                 inc(ctr3);
                           until ctr3 = 127;
                      end;

                      inc(ctr2);
                until ctr2 = 6;
           end
           else
           begin
                num1 := inventory.items[ctr1];
                ctr2 := 1;
                repeat
                      if (item[ctr2].location = currentlocation) AND
                         (item[ctr2].identifier = obj2) then

                      begin
                           num2 := item[ctr2].number;
                           ctr3 := 1;
                           repeat
                                 if ((useobj[ctr3].number1 = num1) AND
                                    (useobj[ctr3].number2 = num2)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num1].location := useobj[ctr3].location1;
                                         item[num2].location := useobj[ctr3].location2;
                                         inventory.items[ctr1] := X;
                                         inventory.items[ctr2] := X;
                                         item[num1].action[get] := Y;
                                         item[num2].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;
                                 if ((useobj[ctr3].number1 = num2) AND
                                    (useobj[ctr3].number2 = num1)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num2].location := useobj[ctr3].location1;
                                         item[num1].location := useobj[ctr3].location2;
                                         inventory.items[ctr2] := X;
                                         inventory.items[ctr1] := X;
                                         item[num2].action[get] := Y;
                                         item[num1].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;

                                 inc(ctr3);
                           until ctr3 = 127;
                      end;

                      inc(ctr2);
                until ctr2 = 127;
           end;

           inc(ctr1);
     until ctr1 = 6;

     ctr1 := 1;
     repeat
           if (item[ctr1].number = currentlocation) AND
              (item[ctr1].identifier = obj1) then

           begin
                num1 := item[ctr1].number;
                ctr2 := 1;
                repeat
                      if (item[inventory.items[ctr2]].number = inventory.items[ctr2]) AND
                         (item[inventory.items[ctr2]].identifier = obj2) then

                      begin
                           num2 := inventory.items[ctr2];
                           ctr3 := 1;
                           repeat
                                 if ((useobj[ctr3].number1 = num1) AND
                                    (useobj[ctr3].number2 = num2)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num1].location := useobj[ctr3].location1;
                                         item[num2].location := useobj[ctr3].location2;
                                         inventory.items[ctr1] := X;
                                         inventory.items[ctr2] := X;
                                         item[num1].action[get] := Y;
                                         item[num2].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;
                                 if ((useobj[ctr3].number1 = num2) AND
                                    (useobj[ctr3].number2 = num1)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num2].location := useobj[ctr3].location1;
                                         item[num1].location := useobj[ctr3].location2;
                                         inventory.items[ctr2] := X;
                                         inventory.items[ctr1] := X;
                                         item[num2].action[get] := Y;
                                         item[num1].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;

                                 inc(ctr3);
                           until ctr3 = 127;
                      end;

                      inc(ctr2);
                until ctr2 = 6;
           end
           else
           begin
                num1 := inventory.items[ctr1];
                ctr2 := 1;
                repeat
                      if (item[ctr2].location = currentlocation) AND
                         (item[ctr2].identifier = obj2) then

                      begin
                           num2 := item[ctr2].number;
                           ctr3 := 1;
                           repeat
                                 if ((useobj[ctr3].number1 = num1) AND
                                    (useobj[ctr3].number2 = num2)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num1].location := useobj[ctr3].location1;
                                         item[num2].location := useobj[ctr3].location2;
                                         inventory.items[ctr1] := X;
                                         inventory.items[ctr2] := X;
                                         item[num1].action[get] := Y;
                                         item[num2].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;
                                 if ((useobj[ctr3].number1 = num2) AND
                                    (useobj[ctr3].number2 = num1)) then
                                    begin
                                         if not(useobj[ctr3].north = X)
                                            then location[currentlocation].north := useobj[ctr3].north;
                                         if not(useobj[ctr3].south = X)
                                            then location[currentlocation].south := useobj[ctr3].south;
                                         if not(useobj[ctr3].east = X)
                                            then location[currentlocation].east := useobj[ctr3].east;
                                         if not(useobj[ctr3].west = X)
                                            then location[currentlocation].west := useobj[ctr3].west;
                                         item[num2].location := useobj[ctr3].location1;
                                         item[num1].location := useobj[ctr3].location2;
                                         inventory.items[ctr2] := X;
                                         inventory.items[ctr1] := X;
                                         item[num2].action[get] := Y;
                                         item[num1].action[get] := Y;
                                         writeln(useobj[ctr3].description);
                                         exit;
                                    end;

                                 inc(ctr3);
                           until ctr3 = 127;
                      end;

                      inc(ctr2);
                until ctr2 = 127;
           end;

           inc(ctr1);
     until ctr1 = 127;
     writeln('Cant do that');
end;

{
 Actions handler
}
procedure actions;
var
     action : string;
     obj1 : string;
     obj2 : string;
     ctr : byte;
begin
     gotoxy(1,15);
     write(':');
     readln(action);

     if (action = 'n') AND not(location[currentlocation].north = X) then
        currentlocation := location[currentlocation].north;
     if (action = 's') AND not(location[currentlocation].south = X) then
        currentlocation := location[currentlocation].south;
     if (action = 'e') AND not(location[currentlocation].east = X) then
        currentlocation := location[currentlocation].east;
     if (action = 'w') AND not(location[currentlocation].west = X) then
        currentlocation := location[currentlocation].west;

     if action = 'quit' then
        begin
             write('Are you sure you want to get out of here? ');
             action := readkey;
             if action = 'y' then halt;
        end;

     if action = 'exam' then
        begin
             write('examine what: ');
             readln(action);
             showexam(currentlocation,action);
             readln;
        end;

     if action = 'get' then
        begin
             write('get what: ');
             readln(action);
             getobject(currentlocation,action);
             delay(2000);
        end;

     if action = 'drop' then
        begin
             write('drop what: ');
             readln(action);
             dropobject(X,action);
             delay(2000);
        end;

     if action = 'inv' then
        begin
             ctr := 1;
             repeat
                   if not(inventory.items[ctr] = X) then
                   writeln(item[inventory.items[ctr]].identifier);
                   inc(ctr);
             until ctr = 6;
             readln;
        end;

     if action = 'use' then
        begin
             write('use what: ');
             readln(obj1);
             write('with what: ');
             readln(obj2);
             usethem(obj1,obj2);
             readln;
        end;

     if action = 'help' then
        begin
             writeln('Commands: get, drop, use, exam, read, inv, quit.');
             readln;
        end;
end;

end.