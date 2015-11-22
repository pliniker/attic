    /****************************************************
	 **                									**
	 **				     CreditMeister.					**
	 **   	      Credit Control and planning.			**
	 **													**
	 **                   by Batman. 					**
	 **	12/6/01											**
	 *****************************************************/

	 /* Notes:
	    -------------------------------------------------------
	    The creditmeister class is responsible for credit
	    control, planning and advice on prices. There is an
	    estimated balance the opponent has, and there are several
	    important details that have to be taken into account
	    when estimating the balance:

	    1. The opponent will have an extra cost by contracting
	       (something which is hidden from us)
	    2. The opponent will have an extra income, which can be
	       retrieved by the SP using the published API. The
	       CommsMeister module will automatically request the
	       information for all the auctions on completion of the
	       transaction. In this way, we can estimate the *maximum*
	       amount they can spend and in that sense win :)
	    3. The saving factor will be determined either manually (there can be
	       a manual override control on the gui along with a slider), either
	       automatically with a reinforcement learning algorithm.
	    4. Similarly the incremental factor will be determined by some kind
	       of reinforcement learning algorithm (or overriden by the human controller)
	       with a control on the gui.
	    5. Using map-based expenditure estimation, we can drastically improve on the
	       enemy balance estimation. Moreover, this along with the reinforcement learning
	       algorithms implemented in incF and savF provides the optimum prices for bidding.
	 */


package uk.ac.ic.doc.ise3.cart2;

import uk.ac.ic.doc.ise3.sp.*;
import java.util.*;


class CreditMeister {

    int final_price;
    int incF, savF;		     	   // inrcremental and saving factor. To be determined later for topping up bids.
    boolean lastWasCritical;
    int lastmatch_price, lockedAmt;
    public Reinfo self;
	int lockAmt = 0;
    int av_block_price, total_blocks; //the average block price.
    int bidding_delta, trans_count;


	Hashtable reinfo;
	static Hashtable balances;


	//
	// add a cartographer to the list
	//
	public void addCartographer(int id, int c, int d, int e) {
		reinfo.put(new Integer(id), new Reinfo(c, d, e));
	}

	public Reinfo getCartographer(int id) {
		return (Reinfo)reinfo.get(new Integer(id));
	}


	//
	// balances info
	//
	public void addBalance(int id, int amt) {
		balances.put(new Integer(id), new Integer(amt));
	}

	public int getBalance(int id) {
		return ((Integer)balances.get(new Integer(id))).intValue();
	}

	public void setBalance(int id, int amt) {
		balances.put(new Integer(id), new Integer(amt));
	}


	//
	// params? none!
	//
    public CreditMeister() {
		//constructor. just prints and sets balance to 0.
		//float scaleF, float learnRt, {int myID}, int index1, int index2, int index3


		self = new Reinfo(10, 8, 10);
		reinfo = new Hashtable();

		balances = new Hashtable();

		addBalance(Const.ourId, 0);
		try {
			setBalance(Const.ourId, CartGroup2.commsman.getBalance() - Bank.OVERDRAFTLIMIT);
		}
		catch (Exception e) {
			System.out.println("CreditMeister.CreditMeister(): bank exception " + e);
		}

		System.out.println("CreditMeister.CreditMeister(): constructed");
    }


    /*****************************************************
	 *                                                   *
	 *           Utility/Supporting  Functions           *
	 *                                                   *
	 *****************************************************/


    //
    // returns the available (unlocked) balance
    //
    int availBalance(){
		return getBalance(Const.ourId) - lockedAmt;
    }

    //
    // locks a given amount (if smaller than the balance)
    public synchronized boolean lockAmt(int amount) {
// 		if ((lockedAmt + amount) < (balances[2] + Bank.OVERDRAFTLIMIT )){
// 			lockedAmt += amount;
// 			return true;
// 		} else {
// 			return false;
// 		}
		return true;
    }

    public synchronized void unlockAmt(int Amt){
// 		if (Amt<=lockAmt){
// 			lockAmt -= Amt;
// 		}
    }



    int convertBalance(int curBalance){
		int balanceIndex;

		if (curBalance > 7000) {
			//more than 7K
			balanceIndex = 9;
		} else if (curBalance > 4000){
			//more than 4K
			balanceIndex = 8;
		} else if (curBalance > 2000){
			//more than 2K
			balanceIndex = 7;
		} else if (curBalance > 1000){
			//more than 1K
			balanceIndex = 6;
		} else if (curBalance > 800){
			//more than 800
			balanceIndex = 5;
		} else if (curBalance > 500){
			//more than 500
			balanceIndex = 4;
		} else if (curBalance > 300){
			//more than 300
			balanceIndex = 3;
		} else if (curBalance > 200){
			//more than 200
			balanceIndex = 2;
		} else if (curBalance > 100){
			//more than 100
			balanceIndex = 1;
		} else {
			//between 0 and 100
			balanceIndex = 0;
		}

		return balanceIndex;
    }


	//
	// get 
	//
	int doReinfo(int emyID, int our_imp, int oth_imp, int otherparam){
		Reinfo curReinfo;
		int balance;

		balance = getBalance(emyID);
		curReinfo = getCartographer(emyID);
		
		if (emyID == Const.ourId) {
			balance = otherparam;
			curReinfo = self;
		}

		return curReinfo.getResult(our_imp, oth_imp, balance);
    }

 
    //
    // this method updates the estimated balance for the relevant party
    //
    public void addTransaction(int party_id, int delta_balance){
		int b = getBalance(party_id);
		b += delta_balance;
		setBalance(party_id, b);
    }


    //
    // indirect balance update.
    //
    public synchronized void blockPriceUpd(int blocks, int price){
		/*this method updates the (moving) average block price, used to
		 *identify the different cartographers' balance*/
		av_block_price = ((av_block_price*total_blocks + price*blocks)/(total_blocks+blocks));
		total_blocks += blocks;
    }


    // ------------------------- NEED WORK > WAITING FOR BID OBJECT --------------------------------

    //
    // updates the bidding_delta and does some housekeeping.
    // parameters are the lot, wanted_to_lose boolean and whether we lost or not
    public void outcome(Lot inlot){
		int winners_bid;
		Bid win = Const.getwinner_id(inlot);
		winners_bid = win.getAmount();
		bidding_delta = (bidding_delta * trans_count + (lastmatch_price - winners_bid))/(trans_count+1);
		trans_count++;
    }


    //
    // this method returns the highest estimated enemy price.
    // it does not take any parameters. it returns an int.
    //
    public int getHiEnemyPrice() {
		int highprice = 0;
		int max_enemy_price;

		for (int x = 0; x < Const.numCarts(); x++){

			//get maximum overdraught price.
			int cid = Const.getCart(x);

			max_enemy_price = getBalance(cid);
			if (max_enemy_price > highprice){
				highprice = max_enemy_price;
			}
		}
		return highprice;
    }



    // --------------- CODE NEEDS TO CHANGE. PENDING API CHANGES -------------------


    //
    // given a lot and an id, returns the id's bid.
    //
    int idFromBid(int id, Lot inlot){
		/*	switch(id) {
			case 0:
			return inlot.getc0bid();
			case 1:
			return inlot.getc1bid();
			default:
			return 0;*/
		return 0;
		
    }


	/*****************************************************
	 *                                                   *
	 *             Publicly accessible function 	     *
	 *                 and main algorithm		    	 *
	 *                                                   *
	 *****************************************************/


	//
	// yeah yeah doing the Auction
 	//
    public Lotbid DoAuction(int our_imp, Lot lot) throws java.rmi.RemoteException, InvalidAccountNoException {

		setBalance(Const.ourId, CartGroup2.commsman.getBalance() - Bank.OVERDRAFTLIMIT);
		CartGroup2.gui.println("CreditMeister: " + getBalance(Const.ourId) + " available");


		int emyID = 0;
		int maxPrice = 0;
		for (int i = 0; i < Const.numCarts(); i++) {
			int id = Const.getCart(i);
			int imp = CartGroup2.mapman.importance(id, lot.getRegion(), lot.getAccuracy());
			int price = doReinfo(id, our_imp, imp, 0);
			if (price > maxPrice) {
				maxPrice = price;
				emyID = id;
			}
		}
		
		int our_reinfo = doReinfo(Const.ourId, getBalance(Const.ourId), our_imp, maxPrice);
		
		final_price = (int) ((float)(our_reinfo * Const.numberBlocks(lot)) / (float)(Const.xdim * Const.ydim));
		final_price = final_price + bidding_delta;

		if (final_price > Const.spendLimitPerBlock) {
			final_price = Const.spendLimitPerBlock;
		}

		Lotbid result = new Lotbid(lot.getLotNo(), final_price);
		return result;
    }


    //
    // Doing the contract.
    //
    public Lotbid DoContract() throws java.rmi.RemoteException, InvalidAccountNoException{

		setBalance(Const.ourId, CartGroup2.commsman.getBalance() - Bank.OVERDRAFTLIMIT);
		CartGroup2.gui.println("CreditMeister: " + getBalance(Const.ourId) + " available");

		Lotbid contractPack;

		if (getBalance(Const.ourId) > 1000) {
			//return the percent of money we can spend on contract.
			//let's say for now 80% of money.
			contractPack = new Lotbid(0, (int) (0.8f * (float)getBalance(Const.ourId)) );
				
			if (av_block_price == 0) {
				contractPack.av_block_price = 100;
			} else {
				contractPack.av_block_price = av_block_price;
			}

		} else {
			contractPack = new Lotbid(0, 0);
			if (av_block_price == 0) {
				contractPack.av_block_price = 100;
			} else {
				contractPack.av_block_price = av_block_price;
			}
		}

		return contractPack;
    }


	//
	// sell something duh
	//
    public Lotbid DoSell(){
		Lotbid sellpack = new Lotbid(0, getHiEnemyPrice());


		if (av_block_price == 0) {
			sellpack.av_block_price = 100;
		} else {
			sellpack.av_block_price = this.av_block_price;
		}
		sellpack.price = getHiEnemyPrice();

		if (sellpack.price == 0) {
			sellpack.price = 1000;
		}

		return sellpack;
    }


}
