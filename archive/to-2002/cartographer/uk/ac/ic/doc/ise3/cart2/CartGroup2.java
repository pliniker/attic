/******************************************************
 **                 CartGroup2.java                  **
 **    Mainline code to start off the objects and    **
 **                     threads                      **
 **                                                  **
 ** batman, 10/3/1789                                **
 ******************************************************/

package uk.ac.ic.doc.ise3.cart2;



import java.rmi.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import uk.ac.ic.doc.ise3.sp.*;



public class CartGroup2 {

    public static AuctionServer sp_auction;
    public static Bank  bankserv;
    public static CreditMeister creditman;
    public static CommsMeister commsman;
    public static BidMeister bidman;
    public static SellMeister salesman;
    public static ContractMeister contractman;
    public static MapMaster mapman;
    public static MapGrid mapdrawer;
    public static MapOMatic gui;

	public static String fileExt;


	
	public static void storeData() {
		try {
			for (int i = 0; i < Const.numCarts(); i++) {
				int id = Const.getCart(i);
				Reinfo r = creditman.getCartographer(id);
				r.storeData(Const.fileExt + ".cart" + id);
			}
			creditman.self.storeData(Const.fileExt + ".self");
		} 
		catch (java.io.IOException e1) {
			System.out.println("CartGroup2.storeData("+Const.fileExt+"): failed");
		}
	}

	public static void loadData() {
		// load reinforcement data
		try {
			CartGroup2.creditman.self.loadData(Const.fileExt + ".self");
		} 
		catch (java.io.IOException e1) {
			System.out.println("CartGroup2.loadData("+Const.fileExt+"): failed");
		}
	}


	//
	// two args - 1: RMI server name 2: Reinfo data filename version
	//
    public static void main(String[] args) throws InterruptedException {

		if (args.length == 3) {
			Const.ourId = (java.lang.Integer.parseInt(args[2]));
		}

		System.out.println("CartGroup2.main(): Using ID " + args[2]);


		System.out.println("CartGroup2.main(): Looking up auction server");
		while (sp_auction==null){
			try {
				sp_auction = (AuctionServer)Naming.lookup("rmi://"+args[0]+"/Auction");
			} catch (NotBoundException e1){
				System.out.println("CartGroup2.main(): server not found");
			} catch (Exception e2){
				System.out.println("CartGroup2.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("CartGroup2.main(): located server");
		}
		
		System.out.println("CartGroup2.main(): Looking up banking server");
		while (bankserv==null){
			try {
				bankserv = (Bank)Naming.lookup("rmi://"+args[0]+"/Bank");
			} catch (NotBoundException e1){
				System.out.println("CartGroup2.main(): server not found");
			} catch (Exception e2){
				System.out.println("CartGroup2.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("CartGroup2.main(): located server");
		}


		// ---
        System.out.println("CartGroup2.main(): Initializing Objects");


		mapman = new MapMaster(Const.xdim, Const.ydim);
		mapdrawer = new MapGrid(mapman.getWallMap(), mapman.getAccMap());
		gui = new MapOMatic(mapdrawer);

		commsman = new CommsMeister("Bob Violence " + Const.ourId);
		creditman = new CreditMeister();
		bidman = new BidMeister();
		salesman = new SellMeister();
		contractman = new ContractMeister();

		// ick code
		gui.addWindowListener(new WindowAdapter() {
				public void windowClosing(WindowEvent e) {
					// store reinforcement data
					CartGroup2.storeData();
					System.exit(0);
				}
			});
		gui.pack();
		gui.setVisible(true);
		// ---


		Const.fileExt = args[1];
		loadData();

		System.out.println("CartGroup2.main(): Starting SalesMeister");
		salesman.start();
		System.out.println("CartGroup2.main(): Starting ContractMeister");
		contractman.start();

		//Region r = new Region();
		//r = r.getSampleRegion(4);
		//mapman.updateMap(Const.ourId, r, 600);
    }

}
