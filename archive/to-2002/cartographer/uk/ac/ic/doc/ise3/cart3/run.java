//
// class run
//
// run yeah
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;

import java.rmi.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;



public class run {

    public static AuctionServer sp_auction;
    public static Bank  bankserv;
    public static Time time;
    public static Client client;

    public static Comms com;
    public static BidMonkey bidman;
    public static SellMonkey salesman;
    public static ContractMonkey contractman;
    public static CartList cart;
    public static Monitor mon;

    public static MapGrid mapdrawer;
    public static MapOMatic gui;



	//
	// write predictors to disk
	//
	public static void storeData() {
		try {
			for (int i = 0; i < cart.count(); i++) {
				int id = cart.getByIndex(i).getId();
				cart.getByIndex(i).pred().store(Const.fileExt + ".cart" + id);
			}
		}
		catch (Exception e1) {
			System.out.println("run.storeData("+Const.fileExt+"): failed " + e1);
			e1.printStackTrace();
		}
	}



	//
	// three args - 1: RMI server name 2: Reinfo data filename version 3: id
	//
    public static void main(String[] args) throws InterruptedException, InvalidAccountNoException {

		Const.fileExt = args[1];
		Const.ourId = (java.lang.Integer.parseInt(args[2]));
		System.out.println("run.main(): Using ID " + args[2]);


		System.out.println("run.main(): Looking up auction server");
		while (sp_auction==null){
			try {
				sp_auction = (AuctionServer)Naming.lookup("rmi://"+args[0]+"/Auction");
			} catch (NotBoundException e1){
				System.out.println("run.main(): server not found");
			} catch (Exception e2){
				System.out.println("run.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("run.main(): located server");
		}


		System.out.println("run.main(): Looking up banking server");
		while (bankserv==null){
			try {
				bankserv = (Bank)Naming.lookup("rmi://"+args[0]+"/Bank");
			} catch (NotBoundException e1){
				System.out.println("run.main(): server not found");
			} catch (Exception e2){
				System.out.println("run.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("run.main(): located server");
		}


		System.out.println("run.main(): Looking up time server");
		while (time == null){
			try {
				time = (Time)Naming.lookup("rmi://"+args[0]+"/Time");
			} catch (NotBoundException e1){
				System.out.println("run.main(): server not found");
			} catch (Exception e2){
				System.out.println("run.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("run.main(): located server");
			try {
				Const.startTime = time.getTime();
				System.out.println("Start time = " + Const.startTime);
			}
			catch (Exception e) {
				System.out.println("run.main(): time exception " + e);
			}
		}

		System.out.println("run.main(): Looking up client server");
		while (client == null){
		    try {
			    client = (Client)Naming.lookup("rmi://"+args[0]+"/Client");
			} catch (NotBoundException e1){
				System.out.println("run.main(): server not found");
			} catch (Exception e2){
				System.out.println("run.main(): exception " + e2);
				Thread.sleep(2000);
			}
			System.out.println("run.main(): located server");
		}

		// ---
        System.out.println("run.main(): Initializing Objects");

		cart = new CartList();
		mon = new Monitor(10); //10 = history number.
		mapdrawer = new MapGrid(cart.self().wallmap(), cart.self().accmap());
		gui = new MapOMatic(mapdrawer);

		bidman = new BidMonkey();
		com = new Comms("Map-O-Matic " + Const.ourId);
		salesman = new SellMonkey();
		contractman = new ContractMonkey();


		// ick code
		gui.addWindowListener(new WindowAdapter() {
				public void windowClosing(WindowEvent e) {
					// store reinforcement data
					run.storeData();
					System.exit(0);
				}
			});
		gui.pack();
		gui.setVisible(true);
		// ---


		System.out.println("run.main(): Starting SellMonkey");
		salesman.start();
		System.out.println("run.main(): Starting ContractMonkey");
		contractman.start();
		System.out.println("run.main(): Starting monitor");
		mon.start();

	}
}
