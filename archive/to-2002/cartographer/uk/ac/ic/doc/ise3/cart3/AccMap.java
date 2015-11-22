//
// class AccMap
//
// deals with accuracy levels
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.lang.Math;
import java.util.*;



public class AccMap {

    int xdim, ydim;
    int[][] acc;
    boolean[][] lck;


    //
    // general access functions
    //
    public int getXDim() {
		return xdim;
    }

    public int getYDim() {
		return ydim;
    }


    // get accuracy at x,y
    public int get(int x, int y) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y < ydim)) {
			return acc[x][y];
		} else {
			//System.out.println("AccMap.get(): out of bounds " + x + ", " + y);
			return 0;
		}
    }

    // set accuracy at x,y
    public void set(int x, int y, int c) {
		if ((x >= 0) && (x < xdim) && (y >= 0) && (y < ydim)) {
			acc[x][y] = c;
		} else {
			System.out.println("AccMap.set(): out of bounds " + x + ", " + y);
		}
    }


    // lock region r
    public void lock(Region r) {
		if ((r.x1 >= 0) && (r.x2 < xdim) && (r.y1 >= 0) && (r.y2 <= ydim)) {

			for (int y = r.y1; y <= r.y2; y++) {
				for (int x = r.x1; x <= r.x2; x++) {
					lck[x][y] = true;
				}
			}
		} else {
			System.out.println("AccMap.lock(): out of bounds " +
							   r.x1 + ", " + r.y1 + ", " +
							   r.x2 + ", " + r.y2);
		}
    }

    // unlock region r
    public void unlock(Region r) {
		if ((r.x1 >= 0) && (r.x2 < xdim) && (r.y1 >= 0) && (r.y2 <= ydim)) {

			for (int y = r.y1; y <= r.y2; y++) {
				for (int x = r.x1; x <= r.x2; x++) {
					lck[x][y] = false;
				}
			}
		} else {
			System.out.println("AccMap.unlock(): out of bounds " +
							   r.x1 + ", " + r.y1 + ", " +
							   r.x2 + ", " + r.y2);
		}
    }

    // unlock region r
    public boolean isLocked(Region r) {
        if ((r.x1 >= 0) && (r.x2 < xdim) && (r.y1 >= 0) && (r.y2 <= ydim)) {

            for (int y = r.y1; y <= r.y2; y++) {
                for (int x = r.x1; x <= r.x2; x++) {
                    if (!lck[x][y]) {
						return false;
					}
                }
            }
			return true;
        } else {
            System.out.println("AccMap.isLocked(): out of bounds " +
                               r.x1 + ", " + r.y1 + ", " +
                               r.x2 + ", " + r.y2);
			return false;
        }
    }


    // get average accuracy of region r
    public int regionAcc(Region r) {
		int avg_acc = 0;
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {
				avg_acc += acc[i][j];
			}
		}
		int area = Misc.area(r);
		if (area == 0) {
			return 0;
		} else {
			return avg_acc / area;
		}
    }

    // get total accuracy of map
    public int totalAcc() {
		int avg_acc = 0;
		for (int j = 0; j < ydim; j++) {
			for (int i = 0; i < xdim; i++) {
				avg_acc += acc[i][j];
			}
		}

		avg_acc /= (xdim * ydim);
		return avg_acc;
    }


    //
    // update accuracy info
    //
    public synchronized void update(Region r, int a) {
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {

				if (a > acc[i][j]) {
					acc[i][j] = a;
				}
			}
		}
    }


    //
    // importance of a Region to the owner of this map
    //
    public int importance(Region r, int lot_acc) {

		// diff between lot_acc and known acc of r
		int avg_acc1 = regionAcc(r);
		int acc_diff1 = lot_acc - avg_acc1;

		// diff between lot_acc and acc of whole map
		int avg_acc2 = totalAcc();
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
	// create a new instance of this class
	// with duplicate data
	//
	public AccMap mapCopy() {
		AccMap c = new AccMap(xdim, ydim);

		for (int j = 0; j < ydim; j++) {
			for (int i = 0; i < xdim; i++) {
				c.set(i, j, acc[i][j]);
			}
		}
		return c;
	}


    //
    // returns AccMap with map inverted
    // 1000 - this
    //
    public AccMap mapNot() {
		AccMap c = new AccMap(xdim, ydim);

		for (int j = 0; j < ydim; j++) {
			for (int i = 0; i < xdim; i++) {
				c.set(i, j, 1000 - acc[i][j]);
			}
		}
		return c;
    }


    //
    // returns AccMap with map (this - g)
    //
    public AccMap mapDiff(AccMap g) {
		if ((g.getXDim() != xdim) && (g.getYDim() != ydim)) {
			System.out.println("AccMap.mapDiff(): attempting to apply to map of different dimensions");
			return null;
		} else {

			AccMap h = new AccMap(xdim, ydim);
			for (int j = 0; j < ydim; j++) {
				for (int i = 0; i < xdim; i++) {
					int a = acc[i][j];
					int b = g.get(i, j);

					// (this - g)
					int c = a - b;
					if (c < 0) { c = 0; }
					h.set(i, j, c);
				}
			}
			return h;
		}
    }


    //
    // returns AccMap with map (this | g)
    //
    public AccMap mapUnion(AccMap g) {
		if ((g.getXDim() != xdim) && (g.getYDim() != ydim)) {
			System.out.println("AccMap.mapUnion(): attempting to apply to map of different dimensions");
			return null;
		} else {

			AccMap h = new AccMap(xdim, ydim);
			for (int j = 0; j < ydim; j++) {
				for (int i = 0; i < xdim; i++) {
					// max(this,g)
					h.set(i, j, Math.max(acc[i][j], g.get(i, j)));
				}
			}
			return h;
		}
    }



    //
    // least accurate
    // area which contains no locked blocks
    // Caller should lock(Area) after this has returned
    //
    public Region unchartedRegion(int a) {
		int startWidth, startHeight, maxGot;
		Vector areas = new Vector();
		Region r = new Region();

		int mapacc = totalAcc();
		startWidth = 1;
		startHeight = 1;


		// find all possible areas of size at least a
		for (int starty = 0; starty <= ydim - startHeight; starty++) {
			for (int startx = 0; startx <= xdim - startWidth; startx++) {

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
						if ((mapacc >= regionAcc(r)) && (!isLocked(r))) {
							width++;
						} else {
							r.x2--;
						}
					}

					// try increasing downwards
					if (r.y2 < ydim - 1) {
						r.y2++;
						if ((mapacc >= regionAcc(r)) && (!isLocked(r))){
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
				Region s = new Region();
				s.x1 = r.x1;  s.x2 = r.x2;  s.y1 = r.y1;  r.y2 = r.y2;
				areas.add(s);
			}
		}


		// find least accurate region
		int best = 1000;
		if (areas.size() > 1) {

			for (int i = areas.size() - 1; i >= 0; i--) {
				Region s = (Region)areas.get(i);
				if (regionAcc(s) <= best) {
					r = s;
					best = regionAcc(s);
				}
			}

		// if not >1 it can be 1 OR 0. 0 case was omitted -> EXCEPTION -> CRASH.
		} else {
			r = (Region)areas.get(1);
		}

		// the caller should lock the area itself to prevent this function
		// returning the same area twice
		return r;
    }




    //
    // dimensions of the map
    //
    public AccMap(int xsize, int ysize) {
		xdim = xsize;
		ydim = ysize;

		acc = new int[xsize][ysize];
		lck = new boolean[xsize][ysize];

		for (int y = 0; y < ysize; y++) {
			for (int x = 0; x < xsize; x++) {
				acc[x][y] = 0;
				lck[x][y] = false;
			}
		}
    }
}
