
//
// the fish must follow each other, because that's what fish do
// reward is given to minimising the distance between each other
// and minimising the angle between them at large distances
//


import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;
import java.applet.Applet;
import java.awt.*;
import java.lang.Thread;
import java.lang.Math;


class Why { // oh why do I need these? nutscrape and IE don't do them
    
    public static double rad2deg(double i) {
	return i*180.0/3.1415927;
    }

    public static double dec2rad(double i) {
	return i*3.1415927/180.0;
    }
}

////////////////////////////////////////////////////////////////////////
//
// includes the plane drawing and learning routines
//
////////////////////////////////////////////////////////////////////////
class Fish {
	
    // trajectory states
    // 0=straight, 1=anticlockwise, -1=clockwise

    int trajectory;

    int xpos, ypos, angle, width, height;
    Color col;
    
    long distInc, angInc;
    float[][][] table = new float[3][10][10];


    //
    // we need a colour, a start position and angle, and width and height
    // of the world
    //
    public Fish(Color c, int startx, int starty, int starta, int w, int h) {
	xpos = startx;
	ypos = starty;
	angle = starta;
	width = w;
	height = h;
	col = c;
	trajectory = 1;  // straight
	distInc = (long) (Math.sqrt( width*width + height*height ) / 9);
	angInc = 360 / 9;
	
	int i, j, k;
	for (i=0; i<3; i++) {
	    for (j=0; j<10; j++) {
		for (k=0; k<10; k++) {
		    table[i][j][k] = (float) Math.random();
		}
	    }
	}
    }
    
    
    public int getXpos() { return xpos; }
    public int getYpos() { return ypos; }
    public int getAngle() { return angle; }
    
    //
    // splats the fish on the Graphics object
    //
    public void draw(Graphics g) {
	double x = Math.cos(Why.dec2rad((double)angle));
	double y = Math.sin(Why.dec2rad((double)angle));
	
	g.setColor(col);
	g.drawLine(xpos, ypos, 
		   (int)(xpos - 7*x), (int)(ypos - 7*y) );
	
	g.drawLine((int)(xpos - 7*x), (int)((ypos-3) - 7*y),
		   (int)(xpos - 7*x), (int)((ypos+3) - 7*y) );
	g.fillOval((int)(xpos-3 - 3*x), (int)(ypos-3 - 3*y), 
		   (int)(6), (int)(6) );
    }
    
    
    //
    // moves the fish along it's current trajectory
    //
    public void move() {
	
	switch (trajectory) {
	case 0:  // clockwise
	    angle -= 6; 
	    if (angle < -180) { angle += 360; }
	    xpos += 4 * Math.cos(Why.dec2rad((double)angle));
	    ypos += 4 * Math.sin(Why.dec2rad((double)angle));
	    break;
	case 2:  // anticlockwise
	    angle += 6; 
	    if (angle > 180) { angle -= 360; }
	    xpos += 4 * Math.cos(Why.dec2rad((double)angle));
	    ypos += 4 * Math.sin(Why.dec2rad((double)angle));
	    break;
	default:
	    xpos += 3 * Math.cos(Why.dec2rad((double)angle));
	    ypos += 3 * Math.sin(Why.dec2rad((double)angle));
	}
	
	if (xpos > width) { xpos = width-5; angle = 180 - angle; }
	if (xpos < 0) { xpos = 5; angle = 180 - angle; }
	if (ypos > height) { ypos = height-5; angle = -angle; }
	if (ypos < 0) { ypos = 5; angle = -angle; }		
    }
    
    
    
    //
    // RL algorithm
    // no transient learning involved, verry simple algo
    //
    double calcAngle(int x, int y) {
	double ang = 
	    Why.rad2deg(Math.atan2( y-ypos, x-xpos )) - angle;
	if (ang > 180.0) { ang -= 360.0; }
	if (ang < -180.0) { ang += 360.0; }
	return ang;
    }
    
    // a bit of a botched reward function, but it sort of works.
    double reward(int x, int y) {
	double r;
	double dist = Math.sqrt((x-xpos)*(x-xpos) + (y-ypos)*(y-ypos));
	double ang = calcAngle(x, y);
	
	if (dist < 1.0) { dist = 1.0; }
	r = 100/dist  - (Math.abs(ang))/dist;
	if (r > 3.0) { r = 3.0; }
	if (r < 1.0) { r = -1.0; }
	return r;
    }
    
    public void think(float alpha, int x, int y) {
	
	int dist = (int)(Math.sqrt( (x-xpos)*(x-xpos) + (y-ypos)*(y-ypos) ) / 
			 (distInc+1) );
	
	int ang = (int)((calcAngle(x, y) + 180)/(angInc+1));
	
	int i;
	float score = (float) -1e8;
	
	for (i=0; i<3; i++) {
	    if (table[i][dist][ang] > score) {
		trajectory = i;
		score = table[i][dist][ang];
	    }
	}
	
	move();

	table[trajectory][dist][ang] += (float) alpha * reward(x,y);
    }
    
}


////////////////////////////////////////////////////////////////////////
//
// Main SmallShoal class
// 
////////////////////////////////////////////////////////////////////////
public class SmallShoal extends Applet 
    implements Runnable, MouseListener, MouseMotionListener {

    int numfish;

    Image offImage;
    Graphics offGraphics;
    private Thread shoal = null;
    int width, height;
    Fish[] fishies;
    int cursx, cursy;
    boolean cursena = false;
    float alpha = 2;
    
    //
    // create a thread in this class instance
    //
    public void init() {
	int i;
	
	numfish = Integer.decode(getParameter("hullo")).intValue();
	fishies = new Fish[numfish];
	width = 240;
	height = 240;
	
	offImage = createImage(width, height);
	offGraphics = offImage.getGraphics();
	
	for (i=0; i<numfish; i++) {
	    fishies[i] = new Fish(Color.orange, 
				  (int)(Math.random() * width), 
				  (int)(Math.random() * height), 
				  (int)(i * 360/numfish), 
				  width, height);
	}
	
	if (shoal == null) {
	    shoal = new Thread(this, "Fishies");
	    shoal.start();
	}
	
	addMouseMotionListener(this);
	addMouseListener(this);
    }
    
    

    //    
    // draw the fish, double buffering employed
    //
    public void update(Graphics g) {
	int i;
	
	offGraphics.setColor(Color.blue);
	offGraphics.fillRect(0, 0, width, height);
	
	for (i=0; i<numfish; i++) {
	    fishies[i].draw(offGraphics);
	}
	
	g.drawImage(offImage, 0, 0, this);
    }
    
    public void paint(Graphics g) {
	update(g);
    }
    
    
    //
    // find the fish with the lowest angle*distance
    //
    Fish nearestFish(Fish f) {
	Fish nearest = f;
	
	int i;
	double temp;
	double dist = Math.sqrt(width*width + height*height) * 180;
	
	for (i=0; i<numfish; i++) {
	    temp = ( (f.getXpos() - fishies[i].getXpos()) *
		     (f.getXpos() - fishies[i].getXpos()) +
		     (f.getYpos() - fishies[i].getYpos()) *
		     (f.getYpos() - fishies[i].getYpos()) 
		     );
	    
	    double ang = f.getAngle() 
		- Math.abs(
			   Why.rad2deg(
				       Math.atan2(
						  fishies[i].getYpos() -
						  f.getYpos(),
						  fishies[i].getXpos() -
						  f.getXpos()
						  )
				       )
			   );
	    
	    if (ang > 180) { ang -= 360; }
	    if (ang < -180) { ang += 360; }
	    //temp = temp * Math.abs(ang);
	    
	    if ((temp < dist) && (f != fishies[i])) {
		nearest = fishies[i];
		dist = temp;
	    }
	}
	
	return nearest;
    }
    
    //
    // main thread function
    //
    public void run() {
	int i;
	Fish f;
	
	for (;;) {
	    try {
		Thread.sleep(10);
	    }
	    catch (InterruptedException e) { 
	    }
	    
	    for (i=0; i<numfish; i++) {
		f = nearestFish(fishies[i]);
		if ( (cursena) && (
				   ((f.getXpos() - fishies[i].getXpos()) *
				    (f.getXpos() - fishies[i].getXpos()) +
				    (f.getYpos() - fishies[i].getYpos()) *
				    (f.getYpos() - fishies[i].getYpos())) >
				   ((f.getXpos() - cursx) *
				    (f.getXpos() - cursx) +
				    (f.getYpos() - cursy) *
				    (f.getYpos() - cursy)) ) ) {
		    fishies[i].think(alpha, cursx, cursy);
		} else {
		    fishies[i].think(alpha, f.getXpos(), f.getYpos());
		}
	    }

	    alpha *= 0.99;
	    
	    repaint();
	}
    }
    
    
    //
    // we'll leave the applet running even when it isn't focused
    // yes it's naughty I know.
    //
    public void stop() {
    }
    
    
    //
    // get mouse movement
    //
    public void mousePressed(MouseEvent e) {
	cursena = true;
    }
    public void mouseDragged(MouseEvent e) {
	cursx = e.getX();
	cursy = e.getY();
    }
    public void mouseReleased(MouseEvent e) {
	cursena = false;
    }
    public void mouseMoved(MouseEvent e) {}
    public void mouseClicked(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
}
