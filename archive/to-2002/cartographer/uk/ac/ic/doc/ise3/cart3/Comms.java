//
// Comms Class
//
// rmi
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;

import java.util.*;
import java.rmi.*;
import java.rmi.server.*;




class Comms implements Cartographer {



	//
	// callback
	//
    public int getBid(Lot inlot) {
		int price = 0;
		try {
			price = run.bidman.newAuction(inlot);
		}
		catch (Exception e2) {
			System.out.println("Comms.getBid(): exception " + e2);
			e2.printStackTrace();
		}
		return price;
    }


	//
	// callback
	//
    public void auctionResult(Lot lot, int reason) {
		try {
			run.salesman.result(lot, reason);
		}
		catch (Exception e) {
			System.out.println("Comms.auctionResult(): exception " + e);
			e.printStackTrace();
		}
	}


	//
	// callback
	//
    public void youWon(Lot lot) {
		try {
			run.bidman.win(lot);

		}
		catch (Exception e) {
			System.out.println("Comms.youWon(): exception " + e);
			e.printStackTrace();
		}
    }


	//
	// callback
	//
    public void youLost(Lot lot) {
		try {
			run.bidman.lose(lot);
		}
		catch (Exception e) {
			System.out.println("Comms.youLost(): exception " + e);
			e.printStackTrace();
		}
    }



	//
	// callback
	//
    public void quotesReady(Vector quotes) {
		//is called by the SP when the stuff are ready.
		//pass them on to the contractmeister.
		try {
			run.contractman.pickQuote(quotes);
		}
		catch (Exception e) {
			System.out.println("Comms.quotesReady(): contractor got a bit screwed " + e);
			e.printStackTrace();
		}
    }


	//
	// callback
	//
    public void submitRegion(Region r, int accuracy) {
		try {
			run.contractman.deliver(r, accuracy);
		}
		catch (Exception e) {
			System.out.println("Comms.submitRegion(): delivering messed up " + e);
			e.printStackTrace();
		}

    }


	//
	// callback
	//
    public void contractMade(int cartID, int robotID, Quote quote) {
		try {
			run.contractman.contractMade(cartID, robotID, quote);
		}
		catch (Exception e) {
			System.out.println("Comms.contractMade(): exception " + e);
			e.printStackTrace();
		}
    }


    /************************************
     **                                 **
     **      Internal commsmeister      **
     **         implementation          **
     **                                 **
     **                                 **
     *************************************/


    //
	// accept the contract
	//
    public void acceptContract(Quote inquote) throws java.rmi.RemoteException, InvalidAccountNoException {
		try {
			run.sp_auction.acceptQuote(inquote, Const.accountNo);
		}
		catch (Exception e) {
			System.out.println("Comms.acceptContract(): exception " + e);
			e.printStackTrace();
		}
    }


	//
	// put a map up for auction
	//
    public void sellMap(int reserve, Region reg) throws java.rmi.RemoteException,InvalidAccountNoException   {
		try {
			run.sp_auction.submitRegion(reg,reserve,Const.accountNo);
		}
		catch (Exception e) {
			System.out.println("Comms.sellMap(): exception " + e);
			e.printStackTrace();
		}
    }


	//
	// request a region to be contracted
	//
    public void contractRegion(Region outRegion) throws InvalidAccountNoException, java.rmi.RemoteException {
		try {
			run.sp_auction.submitJob(outRegion, Const.accountNo);
		}
		catch (Exception e3) {
			System.out.println("Comms.contractRegion(): Exception " + e3);
			e3.printStackTrace();
		}
    }



    /************************************
     **                                 **
     **      Banking stuff (etc)        **
     **                                 **
     **                                 **
     **                                 **
     *************************************/

	//
	// get our own balance
	//
    public int getBalance() throws java.rmi.RemoteException, InvalidAccountNoException {
		int balance = 0;
		try {
			balance = run.bankserv.getBalance(Const.accountNo) - Bank.OVERDRAFTLIMIT;
		}
		catch (Exception e1) {
			System.out.println("Comms.getBalance(): exception " + e1);
			e1.printStackTrace();
		}
		return balance;
    }


	//
	// get robots time reputation
	//
    public long getTimeRep(int robot_acc_no) throws java.rmi.RemoteException, InvalidAccountNoException  {
		int tr = 0;
		try {
			tr = (int)run.bankserv.getTimeReputation(robot_acc_no);
		}
		catch (Exception e) {
			System.out.println("Comms.getTimeRep(): execption " + e);
			e.printStackTrace();
		}
		return tr;
    }


	//
	// get robots accuracy reputation
	//
    public int getAccRep(int robot_acc_no) throws java.rmi.RemoteException, InvalidAccountNoException  {
		int ar = 0;
		try {
			ar = run.bankserv.getAccuracyReputation(robot_acc_no);
		}
		catch (Exception e) {
			System.out.println("Comms.getTimeRep(): execption " + e);
			e.printStackTrace();
		}
		return ar;
    }


	//
	//
	//
	public long getTime() throws java.rmi.RemoteException {
		long time = 0;
		try {
			time = run.time.getTime();
		}
		catch (Exception e) {
			System.out.println("Comms.getTime(): exception " + e);
			e.printStackTrace();
		}
		return time;
	}


	//
	// finished
	//
	public void finish(Region r) throws java.rmi.RemoteException, InvalidAccountNoException {
	    try {
			run.client.submitFinalMap(r, Const.accountNo);
			run.sp_auction.imFinished(Const.accountNo);
	    }
	    catch (Exception e) {
			System.out.println("Comms.finish(): exception " + e);
			e.printStackTrace();
	    }
	}


	//
	// open account, register callbacks
	//
    public Comms(String desc) { 
		super();

		try {
			UnicastRemoteObject.exportObject(this);
		}
		catch(RemoteException re){
			re.printStackTrace();
		}

		System.out.println("Comms.Comms(): Opening account");

		try {
			Const.accountNo = run.bankserv.openAccount(desc, Bank.CARTOGRAPHER, Const.ourId);
		}
		catch (Exception e){
			System.out.println("Comms: exception " + e);
		}

		System.out.println("Comms.Comms(): Registering callbacks");

		try {
			run.sp_auction.register(this, Const.accountNo);
		} catch (Exception e4){
			System.out.println("Comms.Comms(): exception " + e4);
			System.exit(-1);
		}

		System.out.println("Comms.Comms(): constructed");

		System.out.println("Comms.Comms(): bank account no " + Const.accountNo);
    }

}
