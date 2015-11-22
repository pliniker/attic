program text_adventure;

uses crt,textadv;

const ret = chr(13)+chr(10);

procedure initadv;
begin
     textcolor(7);
     textbackground(0);
     clrscr;

     newlocation('You are in the ship''s kitchen.',
                  1, X,X,X,X);
     newlocation('You are in a corridor.',
                  2, X,1,12,6);
     newlocation('You are in the ship''s mess. It is a mess.',
                  3, X,2,X,4);
     newlocation('You are in the recreation room.',
                  4, X,X,3,X);
     newlocation('You are in the ship''s brig.',
                  6, X,X,2,X);
     newlocation('You are in a corridor.',
                  7, 6,11,X,X);
     newlocation('You are in the ship''s engine room.',
                  11, 7,X,X,X);
     newlocation('You are in a corridor.',
                  12, X,14,X,2);
     newlocation('You are in a corridor.',
                  14, 12,X,15,X);
     newlocation('You are in the crews quarters.',
                  15, X,X,X,14);
     newlocation('You are in a corridor.',
                  16, 14,19,X,X);
     newlocation('You are in the ship''s shuttle bay.',
                  19, 16,X,X,X);
     currentlocation := 1;

     newobject('There is a strange brown liquid',1,'liquid',1,
               Y,N,Y,N,Y,N,
               'It is a strange brown liquid which looks a bit like coffee.');
     newobject('There is a cup',7,'cup',1,
               Y,N,Y,N,N,N,
               '           ');
     newobject('A pirate is guarding you',2,'pirate',1,
               N,N,Y,N,Y,N,
               'He is a large,scruffy bloke holding a gigawatt plasma blaster.');
     newobject('A robot is blocking the corridor',3,'robot',2,
               N,N,Y,N,Y,N,
               'It is a standard security robot, model C3PoO. It will not let you past.');
     newobject('There is a plate on a table',4,'plate',3,
               Y,N,Y,N,Y,N,
               'Its a plate. What else do you want to know?');
     newobject('There is a floppy disk on the floor',5,'disk',4,
               Y,N,Y,N,Y,N,
               'A floppy disk! These are about three hundred years out of date!');
     newobject('There is an arcade machine in the corner',6,'arcade',4,
               N,N,Y,N,Y,N,
               'It is a state of the art full immersion 3D virtual reality version of the'
               +ret+'classic game Asteroids.');
     newobject('The crew are here',8,'crew',6,
               N,N,Y,N,Y,N,
               'This is the crew of the trade ship ''Roger the Dodger''. Captain Birk is '
               +ret+'trying to bargain with the pirate');
     newobject('A pirate is guarding the crew',9,'pirate',6,
               N,N,Y,N,Y,N,
               'This pirate has a Faster-Blaster-Master 2000, popular with space pirates.');
     newobject('There is a rack of batteries',11,'batteries',11,
               Y,N,Y,N,Y,N,
               'These are standard AA size batteries. They are used to start the engine of the'
               +ret+'ship.');
     newobject('There is yet another robot',12,'robot',14,
               N,N,Y,N,N,N,
               '          ');
     newobject('There is a bunch of wires',14,'wires',15,
               Y,N,Y,N,Y,N,
               'They appear to be wires. Of course you could be wrong.');
     newobject('There is a shuttle craft',15,'shuttle',19,
               N,N,Y,N,Y,N,
               'This is a Gaguar XJ 2200 Shuttle Craft. It is the most luxurious shuttle'
               +ret+'anybody can own. It belongs to Captain Birk. It does not seem to be working');
     newobject('There is an escape pod',16,'pod',19,
               N,N,Y,N,Y,N,
               'This is about the least luxurious craft in the galaxy. It is designed to '
               +ret+'help you escape in.');

     newuse(1, 1,7, 'You pour the liquid into the cup',
            U,1, X,X,X,X);
     newuse(2, 7,2, 'The pirate thanks you and downs the stuff. He collapses looking rather blue.',
            U,U, 2,X,X,X);
     newuse(3, 14,3, 'You enjoy shoving the wires into the robot which promptly melts into a puddle.',
            U,U, 3,X,X,X);
     newuse(4, 4,9, 'You use the plate like a frisbee, knocking the gun out of the pirates hand.',
            U,U, X,7,X,X);
     newuse(5, 11,15, 'The Gaguars batteries are flat. You fix the AA batteries.',
            U,19, X,X,X,X);
     newuse(6, 5,12, 'When you push the disk into the robot, it decides to let you past.',
            U,U, X,19,X,X);
end;

var
     ctr : longint;

begin
     resetadv;
     initadv;

     ctr := 0;
     repeat
           textcolor(7);
           textbackground(0);
           clrscr;
           viewlocation(currentlocation);
           actions;
           inc(ctr);
     until ctr = 0;
end.