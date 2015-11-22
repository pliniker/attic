//
// class WallMap
//
// walls only
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;


public class WallMap {

    int xdim, ydim;
    boolean[][] hWall;
    boolean[][] vWall;
    
    
    //
    // general access functions
    //
    public int getXDim() {
		return xdim;
    }
	
    public int getYDim() {
		return ydim;
    }
	
    public synchronized boolean getHWall(int x, int y) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y <= ydim)) {
			return hWall[x][y];
		} else {
			//System.out.println("WallMap.getHWall(): out of bounds " + x + ", " + y);
			return false;
		}
    }
    
    public synchronized boolean getVWall(int x, int y) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y <= ydim)) {
			return vWall[x][y];
		} else {
			//System.out.println("WallMap.getVWall(): out of bounds " + x + ", " + y);
			return false;
		}
    }
	
    public synchronized void setHWall(int x, int y, boolean c) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y <= ydim)) {
			hWall[x][y] = c;
		} else {
			//System.out.println("WallMap.setHWall(): out of bounds " + x + ", " + y);
		}	    
    }
	
    public synchronized void setVWall(int x, int y, boolean c) {
		if ((x >= 0) && (x <= xdim) && (y >= 0) && (y < ydim)) {
			vWall[x][y] = c;
		} else {
			//System.out.println("WallMap.setHWall(): out of bounds " + x + ", " + y);
		}
    }
	
	
    //
    // fill out Region.mapdata
    //
    public Region getWallMap(Region r) {
		Vector v = new Vector();
		
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {
				v.add(new Block(getHWall(i, j),
								getVWall(i + 1, j),
								getHWall(i, j + 1),
								getVWall(i, j)));
			}
		}
		
		r.mapdata = v;
		return r;
    }
	

    //
    // just the dimensions. Also fill out know boundary walls
    //
    public WallMap(int xdim, int ydim) {
		this.xdim = xdim;
		this.ydim = ydim;
		hWall = new boolean[xdim][ydim + 1];
		vWall = new boolean[xdim + 1][ydim];
		
		for (int i = 0; i < xdim; i++) {
			hWall[i][0] = true;
			hWall[i][ydim] = true;
		}
		for (int i = 0; i < ydim; i++) {
			vWall[0][i] = true;
			vWall[xdim][i] = true;
		}
    }	
	

}

