//
// our own internal data
//
// class CartInfoSelf
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;



public class CartInfoSelf {

    WallMap w;
	AccMap a;
	Balance b;


    //
    // general access function
    //
    public WallMap wallmap() {
		return w;
    }

    public AccMap accmap() {
		return a;
    }

    public Balance balance() {
		return b;
    }

    //
    // update our own wall and accuracy info
    // the two are inseparable really
    //
    public void updateMap(Region r, int a) {

		int index = 0;
		for (int j = r.y1; j <= r.y2; j++) {
			for (int i = r.x1; i <= r.x2; i++) {

				// make sure we get the data we pay for!
				if (index < r.mapdata.size()) {
					// if the current block has less accuracy than
					// the lot accuracy...

					if (a > accmap().get(i, j)) {
						Block b = (Block)r.mapdata.get(index);

						// update only the walls for which the accuracy is
						// bigger than previously for both ajacent blocks
						if (a > accmap().get(i - 1, j)) {
							wallmap().setVWall(i, j, b.west);
						}

						if (a > accmap().get(i + 1, j)) {
							wallmap().setVWall(i + 1, j, b.east);
						}

						if (a > accmap().get(i, j - 1)) {
							wallmap().setHWall(i, j, b.north);
						}

						if (a > accmap().get(i, j + 1)) {
							wallmap().setHWall(i, j + 1, b.south);
						}
					}
				} else {
					System.out.println("MapMaster.updateMap(): we've been conned!");
				}
				index++;
			}
		}

		// also update accuracy info
		accmap().update(r, a);

		// force gui refresh
		run.mapdrawer.forceUpdate();
    }


    //
    // all as CartInfo
    //
    public CartInfoSelf(int xdim, int ydim, int bal) {
		a = new AccMap(xdim, ydim);
		b = new Balance(bal);
		w = new WallMap(xdim, ydim);
    }
}
