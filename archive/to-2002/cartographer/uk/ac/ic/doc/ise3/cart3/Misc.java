//
// misc functions which can't be placed logically anywhere
// because of Service Provider class limits
//
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;


public class Misc {

    //
    // area of a region
    //
    public static int area(Region r) {
		return (r.x2 - r.x1 + 1) * (r.y2 - r.y1 + 1);
    }


    //
    // who won an auction?
    //
    public static Bid getwinner_bid(Lot l){
        Bid winner = new Bid(-1, -1);
		int amt = 0;
		Vector v = l.getAllBids();

		for (int i = 0; i < v.size(); i++) {
			Bid b = (Bid)v.get(i);
			if (b.getAmount() > amt) {
				amt = b.getAmount();
				winner = b;
			}
		}

        return winner;
    }


    //
    // how much did the winner actually pay?
    //
    public static int getwinner_amt(Lot l) {
		Vector v = l.getAllBids();
		if (v.size() == 1) {
			Bid winner = (Bid)v.get(0);
			return l.getReserve();
		} else {
			Bid winner = getwinner_bid(l);
			v.remove(winner);
			winner = getwinner_bid(l);
			v.add(winner);
			return winner.getAmount();
		}
    }

}
