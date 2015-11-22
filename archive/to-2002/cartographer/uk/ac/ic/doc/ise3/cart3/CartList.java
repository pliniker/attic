//
// class CartList
//
// wrapper for all cartographers
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;



public class CartList {

    Vector ids; // vector of id's
    Hashtable list; // id/CartInfo pairs
    CartInfoSelf selfinfo;


    //
    // general access functions
    //
    public int count() {
		return ids.size();
    }

    public CartInfo get(int id) {
		return (CartInfo)list.get(new Integer(id));
    }


    public CartInfo getByIndex(int index) {
		Integer i = (Integer)ids.get(index);
		return (CartInfo)list.get(i);
    }

    public void add(int id) {
		// if don't already have this cartographer
		// and it isn't self then add it
		Integer i = new Integer(id);
		if ((list.get(i) == null) && (id != Const.ourId)) {
			CartInfo c = new CartInfo(id, Const.xdim, Const.ydim,
									  10, 10, 10, -Bank.OVERDRAFTLIMIT);

			list.put(i, c);
			ids.add(i);
			// reload previous info about cartN
			try {
				c.pred().loadData(Const.fileExt + ".cart" + id);
			} catch (java.io.IOException e1){
				System.out.println("CartList.add(): ERROR LOADING DATA FROM FILE");
			}

			run.gui.println("<Cart " + id + " has joined the game>");
		}
    }

    public CartInfoSelf self() {
		return selfinfo;
    }


    //
    // get a region to sell to other cartographers
    //
    public Region getRegionToAuction(int maxarea) {

		AccMap s = new AccMap(Const.xdim, Const.ydim);
		AccMap t;
		Region one, two;

		// calculate the total area all others have
		for (int i = 0; i < count(); i++) {
			AccMap c = getByIndex(i).accmap();
			s = s.mapUnion(c);
		}

		// what do we have which they ALL don't have (them minus us)
		t = s.mapDiff(selfinfo.accmap());
		one = t.unchartedRegion(maxarea);

		// calculate what we have what each of them doesn't
		two = new Region();
		two.x1 = 0;  two.x2 = 0;  two.y1 = 0;  two.y2 = 0;
		for (int i = 0; i < count(); i++) {
			AccMap c = (getByIndex(i)).accmap();
			t = c.mapDiff(selfinfo.accmap());

			Region temp = t.unchartedRegion(maxarea);
			if (Misc.area(temp) > Misc.area(two)) {
				two = temp;
			}
		}

		// which is smaller
		if (Misc.area(one) < Misc.area(two)) {
			return one;
		} else {
			return two;
		}
    }




    public CartList() throws InvalidAccountNoException {
		list = new Hashtable();
		ids = new Vector();

		System.out.println("CartList.CartList(): Creating our own info class");
		selfinfo = new CartInfoSelf(Const.xdim, Const.ydim, -Bank.OVERDRAFTLIMIT);
    }

}
