
/*
 *
 * MapMaster
 *
 * 08/06 PL.
 *
 */

package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;


public class MapMaster {

    Hashtable cartographers;
    CartMap us;
    WholeMap wallmap;

    // shortcut code
    CartMap cartographer(int id) {
		return (CartMap) cartographers.get(new Integer(id));
    }


    //
    // get the Map objects for probably drawing
    //
    WholeMap getWallMap() {
		return wallmap;
    }
    
    CartMap getAccMap() {
		return us;
    }


    //
    // Lock and unlock an area of our map
    //
    public void lockArea(Area a) {
		us.lock(a);
    }

    public void unlockArea(Area a) {
		us.unlock(a);
    }


	//
	// average accuracy of their maps
	//
	public int mapAccuracy(int id) {
		if (id == Const.ourId) {
			Area a = new Area(0, 0, us.getXdim() - 1, us.getYdim() - 1);
			return us.averageAccuracy(a);
		} else {
			CartMap c = cartographer(id);
			Area a = new Area(0, 0, c.getXdim() - 1, c.getYdim() - 1);
			return c.averageAccuracy(a);
		}
	}


	//
	// average accuracy of Area a
	//
	public int areaAccuracy(int id, Area a) {
		if (id == Const.ourId) {
			return us.averageAccuracy(a);
		} else {
			CartMap c = cartographer(id);
			return c.averageAccuracy(a);
		}
	}


	//
	// get a Region with wall data
	//
	public Region getRegion(Area a) {
		Region r = new Region();
		r.x1 = a.x1;
		r.x2 = a.x2;
		r.y1 = a.y1;
		r.y2 = a.y2;
		r.mapdata = wallmap.getMap(r.x1, r.y1, r.x2, r.y2);
		return r;
	}

	
    //
    // call when a cartographer has bought a map,
    // including for updating our own map
	// Region r, accuracy a
    //
    public void updateMap(int id, Region r, int a) {
		System.out.println("" + id + " == " + Const.ourId);
		if (id == Const.ourId) {

			// update the walls info

			int index = 0;
			for (int j = r.y1; j <= r.y2; j++) {
				for (int i = r.x1; i <= r.x2; i++) {

					// if the current block has less accuracy than
					// the lot accuracy...
					if (index < r.mapdata.size()) {
						if (a > us.getMapping(i, j)) {
							Block b = (Block)r.mapdata.get(index);
							Block mask = new Block(false, false, false, false);
							
							// update only the walls for which the accuracy is
							// bigger than previously for both ajacent blocks
							if (a > us.getMapping(i - 1, j)) {
								mask.west = true;
							}
							
							if (a > us.getMapping(i + 1, j)) {
								mask.east = true;
							}
							
							if (a > us.getMapping(i, j - 1)) {
								mask.north = true;
							}
							
							if (a > us.getMapping(i, j + 1)) {
								mask.south = true;
							}
							
							wallmap.updateBlock(b, mask, i, j);
						}
					} else {
						System.out.println("MapMaster.updateMap(): we've been conned!");
					}
					index++;
				}
			}

			// also update accuracy info
			us.update(r, a);

			CartGroup2.mapdrawer.forceUpdate();
			CartGroup2.gui.println("MapMaster.update(" +
								   r.x1 + ", " + r.y1 + ", " +
								   r.x2 + ", " + r.y2 + ")");

		} else {
			CartMap c = cartographer(id);
			c.update(r, a);
		}
    }


    //
    // get the importance factor (%) for a region 
	// with accuracy a
    //
    public int importance(int id, Region r, int a) {
		if (id == Const.ourId) {
			return us.importance(r, a);
		} else {
			CartMap c = cartographer(id);
			return c.importance(r, a);
		}
    }


    //
    // find an region to submit for contract
    //
    public Area getRegionToContract(int maxarea) {
		return us.unchartedRegion(maxarea);
    }


    //
    // get a region to sell to other cartographers
    //
    public Area getRegionToAuction(int maxarea) {

		CartMap s = new CartMap(0, us.getXdim(), us.getYdim());
		CartMap t;
		Area one, two;

		// calculate the total area all others have
		for (Enumeration e = cartographers.elements(); e.hasMoreElements() ;) {
			CartMap c = (CartMap) e.nextElement();
			s = s.mapUnion(c);
		}
		
		// what do we have which they ALL don't have (them minus us)
		t = s.mapDiff(us);
		one = t.unchartedRegion(maxarea);


		// calculate what we have what each of them doesn't
		two = new Area(0,0,0,0);
		for (Enumeration e = cartographers.elements(); e.hasMoreElements() ;) {
			CartMap c = (CartMap) e.nextElement();
			t = c.mapDiff(us);
			Area temp = t.unchartedRegion(maxarea);
			if (temp.area() > two.area()) {
				two = temp;
			}
		}


		// which is bigger (smaller?)
		if (one.area() < two.area()) {
			return one;
		} else {
			return two;
		}
    }


	//
	// add a cartographer to the bunch
	//
	public void addCartographer(int id) {
		cartographers.put(new Integer(id), 
						  new CartMap(id, us.getXdim(), us.getYdim()));
	}


    //
    // give array of cartographer id's
    // don't include self in the list.
    //
    public MapMaster(int xdim, int ydim) {

		// make hashtable of cartographer and contract maps
		cartographers = new Hashtable();

		us = new CartMap(Const.ourId, xdim, ydim);
		wallmap = new WholeMap(xdim, ydim);

		System.out.println("MapMaster.MapMaster(): constructed");
    }

}
