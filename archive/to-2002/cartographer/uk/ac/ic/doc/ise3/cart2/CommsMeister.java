/******************************************************
 **                commsmeister.java                 **
 **    central communications hub and information    **
 **                  distribution                    **
 **                                                  **
 ** batman, 7/3/1492                                 **
 ******************************************************/

package uk.ac.ic.doc.ise3.cart2;


import java.util.*;
import java.rmi.*;
import java.rmi.server.*;
import uk.ac.ic.doc.ise3.sp.*;

class CommsMeister implements Cartographer /*CAUTION: This is the SP cartographer, not PeterL's*/ {



    /************************************
     **                                 **
     **   Cartographer implementation   **
     **                                 **
     **                                 **
     **                                 **
     *************************************/

    public CommsMeister(String desc){
	super();
	
	try {
	    UnicastRemoteObject.exportObject(this);
	}
	catch(RemoteException re){
	    re.printStackTrace();
	}
	
	System.out.println("CommsMeister.CommsMeister(): Opening account");
	
	try {
	    Const.accountId = CartGroup2.bankserv.openAccount(desc, Bank.CARTOGRAPHER, Const.ourId);
	}
	catch (Exception e){
	    System.out.println("CommsMeister: exception " + e);
	}
	
	System.out.println("CommsMeister.CommsMeister(): Registering callbacks");
	
	try {
	    CartGroup2.sp_auction.register(this, Const.accountId);
	} catch (Exception e4){
	    System.out.println("CommsMeister.CommsMeister(): exception " + e4);
	    System.exit(-1);
	}
	
	System.out.println("CommsMeister.CommsMeister(): constructed");
    }
    
    
    
    public int getBid(Lot inlot) {
	Lotbid togo = new Lotbid(-1 , -1);
	try {
	    togo = CartGroup2.bidman.newAuction(inlot);
	}
	catch (RemoteException e) {
	    System.out.println("CommsMeister.getBid(): SP exception " + e);
	    System.exit(-1);
	}
	catch (InvalidAccountNoException e1) {
	    System.out.println("CommsMeister.getBid(): invalid account " + e1);
	    System.exit(-1);
	}
	catch (Exception e2) {
	    System.out.println("CommsMeister.getBid(): exception " + e2);
	    e2.printStackTrace();
	}
	return togo.price;
    }
    
    

    public void auctionResult(Lot lot, int reason) {
	try {
	    switch (reason) {
	    case AuctionServer.YOU_SOLD_LOT: {

		// update Reinfo stuff
		Vector v = lot.getAllBids();
		for (int i = 0; i < v.size(); i++) {
		    Bid n = (Bid)v.get(i);
		    
		    // add any new cartographers to list
		    Const.addCart(n.getID());
		    
		    // update knowledge
		    Reinfo r = CartGroup2.creditman.getCartographer(n.getID());
		    Area a = new Area(lot.getRegion());
		    float updt = (float)n.getAmount() * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
		    r.update((int)updt);
		}
		
		Bid b = Const.getwinner_id(lot);
		CartGroup2.creditman.outcome(lot);
		CartGroup2.creditman.addTransaction(b.getID(), -Const.getwinner_amt(lot));
		CartGroup2.mapman.updateMap(b.getID(), lot.getRegion(), lot.getAccuracy());
		
		CartGroup2.gui.println("CommsMeister: sold lot " + lot.getLotNo());
		break; }
	    default:
		CartGroup2.gui.println("CommsMeister: unsold lot " + lot.getLotNo());
	    }
	}
	catch (Exception e) {
	    System.out.println("CommsMeister.auctionResult(): exception " + e);
	    e.printStackTrace();
	}
    }
    

    public void youWon(Lot lot) {
	try {
	    // if we won and wanted to lose...
	    Bid b = Const.getwinner_id(lot);
	    int amt = Const.getwinner_amt(lot);

	    if (lot.getAccuracy() < Const.accuracyThreshold) {
		Area a = new Area(lot.getRegion());
		float updt = (float)amt * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
		CartGroup2.creditman.self.update((int)updt);
	    }
	    
	    // update Reinfo stuff
	    Vector v = lot.getAllBids();
	    for (int i = 0; i < v.size(); i++) {
		Bid n = (Bid)v.get(i);
		
		if (n.getID() == Const.ourId) { continue; }
		
		// add any new cartographers to list
		Const.addCart(n.getID());
		
		// update knowledge
		Reinfo r = CartGroup2.creditman.getCartographer(n.getID());
		Area a = new Area(lot.getRegion());
		float updt = (float)n.getAmount() * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
		r.update((int)updt);
	    }
	    
	    // newly acquired region.
	    CartGroup2.mapman.updateMap(Const.ourId, lot.getRegion(), lot.getAccuracy());
	    CartGroup2.creditman.outcome(lot);
	    CartGroup2.creditman.addTransaction(Const.ourId, -amt);
	    
	    CartGroup2.gui.println("CommsMeister: won auction " + lot.getLotNo());
	}
	catch (Exception e) {
	    System.out.println("CommsMeister.youWon(): exception " + e);
	    e.printStackTrace();
	}
    }
    

    public void youLost(Lot lot) {
	try {
	    // update Reinfo stuff
	    Vector v = lot.getAllBids();
	    for (int i = 0; i < v.size(); i++) {
		Bid n = (Bid)v.get(i);
		
		if (n.getID() == Const.ourId) { continue; }
		
		// add any new cartographers to list
		Const.addCart(n.getID());
		
		// update knowledge
		Reinfo r = CartGroup2.creditman.getCartographer(n.getID());
		Area a = new Area(lot.getRegion());
		float updt = (float)n.getAmount() * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
		r.update((int)updt);
	    }
	    
	    Bid b = Const.getwinner_id(lot);
	    int pay = Const.getwinner_amt(lot);
	    
	    Area a = new Area(lot.getRegion());
	    float updt;
	    if (lot.getAccuracy() >= Const.accuracyThreshold) {
		// if we wanted to win, then that was bad
		updt = (float)b.getAmount() * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
	    } else {
		// otherwise good - adjust so next time we bid what they pay
		int lowerbid = pay;
		updt = (float)lowerbid * (1000.0f / (float)lot.getAccuracy()) * ((float)(Const.xdim * Const.ydim) / (float)a.area());
	    }
	    CartGroup2.creditman.self.update((int)updt);
	    
	    // if all were below reserve...
	    if (b.getID() != Const.ourId) {
		CartGroup2.creditman.outcome(lot);
		CartGroup2.creditman.addTransaction(b.getID(), - pay);
		CartGroup2.mapman.updateMap(b.getID(), lot.getRegion(), lot.getAccuracy());
	    }
	    
	    CartGroup2.gui.println("CommsMeister: lost auction " + lot.getLotNo());
	}
	catch (Exception e) {
	    System.out.println("CommsMeister.youLost(): exception " + e);
	    e.printStackTrace();
	}
    }
    


    public void quotesReady(Vector quotes) {
	//is called by the SP when the stuff are ready.
	//pass them on to the contractmeister.
	try {
	    CartGroup2.contractman.receiveFromSP(quotes);
	} 
	catch (Exception e) {
	    System.out.println("CommsMeister.quotesReady(): contractor got a bit screwed " + e);
	    e.printStackTrace();
	}
    }
    

    public void submitRegion(Region r, int accuracy) {
	CartGroup2.mapman.updateMap(Const.ourId,r, accuracy);
	Area a = new Area(r);
	CartGroup2.mapman.unlockArea(a);
	CartGroup2.contractman.down();
	CartGroup2.gui.println("CommsMeister: received region from robot");
    }

    public void contractMade(int cartID, int robotID, Quote quote){
	//that is the broadcast thing we will be called with.
	CartGroup2.mapman.updateMap(cartID, quote.getRegion(), quote.getAccuracy());
	CartGroup2.creditman.addTransaction(cartID, - quote.getPrice());
    }

    /************************************
     **                                 **
     **      Internal commsmeister      **
     **         implementation          **
     **                                 **
     **                                 **
     *************************************/

    // ******** need to add try-catch stuff here ****************
    
    public void acceptContract(Quote inquote) throws java.rmi.RemoteException, InvalidAccountNoException {
	CartGroup2.sp_auction.acceptQuote(inquote,Const.accountId);
    }
    
    public void sellMap(int reserve, Region reg) throws java.rmi.RemoteException,InvalidAccountNoException   {
	CartGroup2.sp_auction.submitRegion(reg,reserve,Const.accountId);
    }
    
    public void contractRegion(Region outRegion) throws InvalidAccountNoException, java.rmi.RemoteException {
	try {
	    CartGroup2.sp_auction.submitJob(outRegion, Const.accountId);
	} catch (RemoteException e1){
	    System.out.println("CommsMeister.contractRegion(): RemoteException " + e1);
	} catch (Exception e3) {
	    System.out.println("CommsMeister.contractRegion(): Exception " + e3);
	    e3.printStackTrace();
	}
    }

    // **********************************************************

    /************************************
     **                                 **
     **      Banking stuff (etc)        **
     **                                 **
     **                                 **
     **                                 **
     *************************************/

    public int getBalance() throws java.rmi.RemoteException, InvalidAccountNoException {
	return	CartGroup2.bankserv.getBalance(Const.accountId);
    }
    
    public long getTimeRep(int robot_acc_no) throws java.rmi.RemoteException, InvalidAccountNoException  {
	return CartGroup2.bankserv.getTimeReputation(robot_acc_no);
    }
    
    
    
    public int getAccRep(int robot_acc_no) throws java.rmi.RemoteException, InvalidAccountNoException  {
	return	CartGroup2.bankserv.getAccuracyReputation(robot_acc_no);
    }
    
}
