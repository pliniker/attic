
/*
 * PL
 */


package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;



public class Const {

    public static int ourId = 2;
    public static int accountId = 666;

	public static int maxContracts = 3;
	public static int contractSpewOutRate = 5000;
	public static int robotTimePerBlock = 8000;

    public static int accuracyThreshold = 800;
	public static int spendLimitPerBlock = 100;

	public static float learningRate = 1.0f;
	public static float scalingFactor = 0.3f;

	public static String fileExt;

    final public static int xdim = 10;
    final public static int ydim = 10;

    final public static float importance_areaNotGot = (float) 80.0;
    final public static float importance_totalArea = (float) 20.0;


	//
	// ah just what the doctor ordered
	//
	private static Vector cartographers = new Vector();


	// is something a cartographer?
	public static boolean isCart(int id) {
		for (int i = 0; i < cartographers.size(); i++) {
			int x = ((Integer)cartographers.get(i)).intValue();
			if (x == id) {
				return true;
			}
		}
		return false;
	}


	//
	// do all the necessary to create a new cartographer
	//
	public static void addCart(int id) {
		for (int i = 0; i < cartographers.size(); i++) {
			int x = ((Integer)cartographers.get(i)).intValue();
			if (x == id) {
				return;
			}
		}
		if (id != ourId) {
			CartGroup2.gui.println("<Cartographer " + id + " has joined the game>");
			cartographers.add(new Integer(id));
			CartGroup2.mapman.addCartographer(id);
			CartGroup2.creditman.addCartographer(id, 10, 8, 8);
			CartGroup2.creditman.addBalance(id, -Bank.OVERDRAFTLIMIT);

			try {
				Reinfo r = CartGroup2.creditman.getCartographer(id);
				r.loadData(fileExt + ".cart" + id);
			}
			catch (Exception e) {
				System.out.println("Const.addCart(): loadData("+fileExt+"): failed");
			}
		}
	}

	// number of unique cartographers
	public static int numCarts() {
		return cartographers.size();
	}


	// get one
	public static int getCart(int index) {
		int x;
		try {
			x = ((Integer)cartographers.get(index)).intValue();
			return x;
		}
		catch (Exception e) {
			System.out.println("Const.getCart(): exception " + e);
			x = -1;
			return x;
		}
	}


    //
    // Returns winner's id
    //
    public static Bid getwinner_id(Lot l){
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
			Bid winner = getwinner_id(l);
			v.remove(winner);
			winner = getwinner_id(l);
			return winner.getAmount();
		}
	}



	//
	// number of blocks in various things
	//
	public static int numberBlocks(Lot l) {
		return numberBlocks(l.getRegion());
	}

	public static int numberBlocks(Region r) {
		return (r.x2 - r.x1 + 1) * (r.y2 - r.y1 + 1);
	}

	public static int numberBlocks(Area a) {
		return (a.x2 - a.x1 + 1) * (a.y2 - a.y1 + 1);
	}

	public static int numberBlocks(Quote q) {
		return numberBlocks(q.getRegion());
	}
}

