
/*
 * other cartographers' info
 *
 * PL. 05/06/1987
 *
 */

package uk.ac.ic.doc.ise3.cart2;

import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;
import java.lang.Math;



public class CartMap {


    // id num
    protected int id;
	
	
    // mapped blocks to date
    protected int xdim, ydim;
    protected int[][] accuracy;
    protected boolean[][] locked; //???? - set if area requested, unset when updated


    //
    // get carto id
    //
    public int getId() {
		return id;
    }
	
	
    //
    // get map dimensions
    //
    public int getXdim() {
		return xdim;
    }
	
    public int getYdim() {
		return ydim;
    }
	
    public int getMapping(int x, int y) {
		if (x < 0) { return 0; }
		if (x >= xdim) { return 0; }
		if (y < 0) { return 0; }
		if (y >= ydim) { return 0; }
		return accuracy[x][y];
    }
	
    public void setMapping(int x, int y, int acc) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y < ydim)) {
			accuracy[x][y] = acc;
		}
    }
	
	
    //
    // return number of blocks in a region not already mapped
    //
    public int blocksNotGot(Area r) {
		int blocks = 0;
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; j++) {
				if (accuracy[i][j] == 0) {
					blocks++;
				}
			}
		}
		return blocks;
    }


    //
    // return number of blocks in a region of accuracy below threshold
    //
    public int blocksBelowThreshold(Area r) {
		int blocks = 0;
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {
				if (accuracy[i][j] < Const.accuracyThreshold) {
					blocks++;
				}
			}
		}
		return blocks;
    }
	

    //
    // average accuracy for area a
    //
    public int averageAccuracy(Area a) {
		int avg_acc = 0;
		for (int j = a.y1; j <= a.y2; j++) {
			for (int i = a.x1; i <= a.x2; i++) {
				avg_acc += accuracy[i][j];
			}
		}
		avg_acc /= a.area();
		return avg_acc;
    }
    

    //
    // check whether anything in an Area is locked or not
    //
    public boolean isLocked(Area a) {
		for (int j = a.y1; j <= a.y2; j++) {
			for (int i = a.x1; i <= a.x2; i++) {
				if (locked[i][j]) return true;
			}
		}
		return false;
    }


    //
    // Lock an area
    //
    public void lock(Area a) {
		for (int j = a.y1; j <= a.y2; j++) {
			for (int i = a.x1; i <= a.x2; i++) {
				locked[i][j] = true;
			}
		}
    }


    //
    // Unlock an area
    //
    public void unlock(Area a) {
		for (int j = a.y1; j <= a.y2; j++) {
			for (int i = a.x1; i <= a.x2; i++) {
				locked[i][j] = false;
			}
		}
    }


    //
    // update known mapped areas
	// Region r, accuracy a
    //
    public void update(Region r, int a) {
		// patch the map
		
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {
				
				// presumably accuracy can only go up??
				if (accuracy[i][j] < a) {
					accuracy[i][j] = a;
				}
				
				locked[i][j] = false;
			}
		}
    }
	

    //
    // calculate an importance factor for region r
    // based on comparative accuracy between r, the region
    // r in our map and the whole map
    //
    public int importance(Region r, int lot_acc) {

		// average accuracy of the region already known
		Area a = new Area(r);
		int avg_acc1 = averageAccuracy(a);

		// difference between accuracy of lot_acc and that
		int acc_diff1 = lot_acc - avg_acc1;

		// incorporate average accuracy of entire map
		a = new Area(0, 0, xdim - 1, ydim - 1);
		int avg_acc2 = averageAccuracy(a);
		int acc_diff2 = avg_acc2 - avg_acc1;

		int acc_diff = (lot_acc / 2) + (acc_diff1 - acc_diff2);
		System.out.println("" + lot_acc + " " + acc_diff1+" "+acc_diff2);

		if (acc_diff1 > 0) {
			acc_diff = (int) (Math.sqrt( (double)(10 * acc_diff) ));
		} else {
			acc_diff = 0;
		}

		// bound between 0 & 100
		if (acc_diff > 100) {
			acc_diff = 100;
		}
		if (acc_diff < 0) {
			acc_diff = 0;
		}

		return acc_diff;
    }


    //
    // find largest, least accurate, least mapped
    // area	which contains no locked blocks
    // Caller should lock(Area) after this has returned
    //
    public Area unchartedRegion(int a) {
		int startWidth, startHeight, maxGot;
		Vector areas = new Vector();
		Area r = new Area(0, 0, 0, 0);
	
	
		if (a < 6) {
			startWidth = 1;
			startHeight = 1;
			maxGot = 0;
		} else {
			startWidth = 2;
			startHeight = 2;
			maxGot = 3;
		}
	
		// find all possible areas of size at least a
		for (int starty = 0; starty < ydim - startHeight; starty++) {
			for (int startx = 0; startx < xdim - startWidth; startx++) {
		
				int width = startWidth;
				int height = startHeight;
				r.x1 = startx;
				r.y1 = starty;
		
				// never go bigger than area a
				while (width * height <= a) {
		    
					r.x2 = startx + width - 1;
					r.y2 = starty + height - 1;


					// try increasing to the right
					if (r.x2 < xdim - 1) {
						r.x2++;
						if ((r.area() - blocksBelowThreshold(r) <= maxGot) &&
							(!isLocked(r))) {
							width++;
						} else {
							r.x2--;
						}
					}
		    
					// try increasing downwards
					if (r.y2 < ydim - 1) {
						r.y2++;
						if ((r.area() - blocksBelowThreshold(r) <= maxGot) &&
							(!isLocked(r))){
							height++;
						} else {
							r.y2--;
						}
					}
		    
					// if cannot increase size, then just quit
					if ((r.x2 == startx + width - 1) && 
						(r.y2 == starty + height - 1)) {
						break;
					}
				}
		
				// add region to the list of possibles
				Area s = new Area(r.x1, r.y1, r.x2, r.y2);
				areas.add(s);
			}
		}
	
	
		// find first least accurate area
		int best = 1000;
		if (areas.size() > 1) {

			for (int i = areas.size() - 1; i >= 0; i--) {
				Area s = (Area)areas.get(i);
				if (averageAccuracy(s) <= best) {
					r = s;
					best = averageAccuracy(s);
				}
			}

		} else {
			r = (Area)areas.get(1);
		}
	
		// the caller should lock the area itself to prevent this function
		// returning the same area twice
		return r;
    }
    

    //
    // returns CartMap with map inverted
    // 1000 - this
    //
    public CartMap mapNot() {
		CartMap c = new CartMap(0, xdim, ydim);

		for (int j = 0; j < ydim; j++) {
			for (int i = 0; i < xdim; i++) {
				c.setMapping(i, j, 1000 - accuracy[i][j]);
			}
		}
		return c;
    }


    //
    // returns CartMap with map (this - g)
    //
    public CartMap mapDiff(CartMap g) {
		if ((g.getXdim() != xdim) && (g.getYdim() != ydim)) {
			return null;
		} else {

			CartMap h = new CartMap(0, xdim, ydim);
			for (int j = 0; j < ydim; j++) {
				for (int i = 0; i < xdim; i++) {

					int a = accuracy[i][j];
					int b = g.getMapping(i, j);

					// (this - g)
					int c = a - b;
					if (c < 0) { c = 0; }
					h.setMapping(i, j, c);
				}
			}
			return h;
		}
    }


    //
    // returns CartMap with map (this | g)
    //
    public CartMap mapUnion(CartMap g) {
		if ((g.getXdim() != xdim) && (g.getYdim() != ydim)) {
			return null;
		} else {

			CartMap h = new CartMap(0, xdim, ydim);
			for (int j = 0; j < ydim; j++) {
				for (int i = 0; i < xdim; i++) {
					
					// max(this,g)
					h.setMapping(i, j, Math.max(accuracy[i][j], g.getMapping(i, j)));
				}
			}
			return h;
		}
    }
	
	
    //
    // just requires an ID
    //
    public CartMap(int cid, int xsize, int ysize) {
		id = cid;
		xdim = xsize;
		ydim = ysize;
		accuracy = new int[xsize][ysize];
		locked = new boolean[xsize][ysize];

		for (int j = 0; j < ysize; j++) {
			for (int i = 0; i < xsize; i++) {
				accuracy[i][j] = 0;
				locked[i][j] = false;
			}
		}

		//System.out.println("CartMap.CartMap(): created with id " + cid);
    }
}
