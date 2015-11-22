
package uk.ac.ic.doc.ise3.cart2;


import java.util.*;
import java.lang.Math.*;
import uk.ac.ic.doc.ise3.sp.*;

/* ContractMeister By Anthony

   using the satisatical information provided by the MapMeister and CreditMeister to
   make a decision in constructing a contract which will send to the robot

   Negotiate between, uncovered map or accuracy ? strategy ?? to make the whole map first
   or making it as accuarate as possible ? Need to ask for clarification of penality in
   accuracy and uncertainly
   of the map.

   Method calling the MapMaster
   CartGroup2.mapman.getRegionToContract( );
   Method calling the CreditMaster
   CartGroup2.creditman.DoContract();
*/

public class ContractMeister extends Thread {
    int maxBlockCanGet;
    Area submitRegion;
    Lotbid myLotBid;
	public static int numContracts;

	//
	// up and down
	//
	public synchronized void up() {
		numContracts++;
	}
	
	public synchronized void down() {
		numContracts--;
	}


    public ContractMeister( ){
		numContracts = 0;
		System.out.println("ContractMeister.ContractMeister(): constructed");
    }

    public void run() {
		try {
			while(true) {

				//how often do we make a contract ? as long as we have money ?
				//then we are making lots of contract within a short time ?
				//need another thread which response to the reply from the server
				//choose the best choice of bid if possible

				while (numContracts < Const.maxContracts) {

					try {
						myLotBid = CartGroup2.creditman.DoContract();
					} catch (Exception e) {
						System.out.println("ContractMeister.run(): exception " + e);
						e.printStackTrace();
						System.exit(1);
					}

					// this initial value needs to be changed later on the time
					// the contact price is only part of the overall balance
					// since we need to keep some money for some other purposes
					
					if (myLotBid.price >= 100) {

						maxBlockCanGet = (int) myLotBid.price / myLotBid.av_block_price;
						if (maxBlockCanGet != 0) {
							submitRegion = CartGroup2.mapman.getRegionToContract(maxBlockCanGet);
							
							// need to lock this area, since we may need to make another contract while
							// the previous is in process.
							
							// do we need to remember which region has been locked ?
							// and differentiate the real region that we have bought and
							// the locked region ?
							CartGroup2.mapman.lockArea(submitRegion);
							
							//consturcting a contract to send to the comms master
							try {
								CartGroup2.commsman.contractRegion(convertReg(submitRegion));
							} catch (Exception e) {
								System.out.println("ContractMeister.run(): exception " + e);
								System.exit(1);
							}
							
							CartGroup2.gui.println("ContractMeister: requesting ("
												   +submitRegion.x1+","+submitRegion.y1+","
												   +submitRegion.x2+","+submitRegion.y2+")");
							
							up();
						} else {
							CartGroup2.gui.println("ContractMeister: nothing left to contract!");
						}

					} else {
						CartGroup2.gui.println("ContractMeister: not contracting");
					}

					// just fixed a contstant 5s where the group make a new contract every
					// five seconds ???
					Thread.sleep(Const.contractSpewOutRate);
				}
			}

		}
		catch (InterruptedException e){
			System.out.println("ContractMeister.run(): OK so tell me you didn't really forget to create a CreditMeister?");
		}
    }

    private Region convertReg(Area in){
		Region out= new Region( );
		out.x1 = in.x1;
		out.x2 = in.x2;
		out.y1 = in.y1;
		out.y2 = in.y2;
		return  out;
    }


    /* reply from Service Provider via
       the commsMesiter about the submitted contract.
       This will be a Vector of quote.
       Need to choose the best quote
       by comparing the cheapest price, accuracy, time and
       reputation of each robot
    */

    //call by the commsmaster in response to the return quote
    public synchronized void receiveFromSP(Vector quote) throws java.rmi.RemoteException, InvalidAccountNoException {
		int accuracy = 0;
		Quote bestQuote;
		//sleep for 5 seconds and check the current balance again ??
		int timeOut = 5000;
		int noOfBlocks = 0;
		int avBlockPrice = 0;

		//do the contract master need to save that everytime a contract is submitted ?

		Region thisRegion;

		/* do we need to remember what is our estimated price of that quote before ?
		   otherwise we need to store the history of the estimated price
		   but the average_block prices keeps changing so even the same number of blocks
		   the estimated price will be different ?
		*/


		if (quote.size() == 0) {
			return;
		}


		if ((bestQuote = FindPrice(quote)) != null){
			// before accepting the quote need to check the credit again.
			// do we have enought money ?
			
			int waitingTime = 10;
			myLotBid = CartGroup2.creditman.DoContract();
			while (((myLotBid.price < bestQuote.getPrice())
					&& (waitingTime > 0))) {
				
				try {
					Thread.sleep(Const.contractSpewOutRate);
					myLotBid = CartGroup2.creditman.DoContract();
					waitingTime--;
				} catch (Exception e) {
					System.out.println("ContractMeister.receiveFromSP(): exception " + e);
				}
				
			}

			if (waitingTime > 0) {
				CartGroup2.commsman.acceptContract(bestQuote);
				CartGroup2.gui.println("ContractMeister: accepting quote "+bestQuote);
			}
		}
		else {
			/* if no quote is accepted due to unaccpetable accuracy then
			   don't reply to SP and leave it as time out and unlock
			   the area.

			   Need to tell the map master to unlock the region, since the contract is not success.
			*/

			thisRegion = ((Quote) quote.elementAt(0)).getRegion( );
			Area a = new Area(thisRegion);
			CartGroup2.mapman.unlockArea(a);
		}
    }

    private Quote FindPrice(Vector quote){
		// when no robot accept the contract then vector = null
		// Should also take into account the reputation of each
		// robot

		int totalTime = 0;
		Quote thisQuote;
		int NoOfRob = 0;
		int noOfBlocks = 0;
		int maxRobotTime = 0;
		float avBlockPrice = 0;
		int totalPrice = 0;
        double thePrice =0;
		double priceRatio;
		Quote bestQuote = null;
		float averageTime = 0;
		float averagePrice = 0;
		float averageAccuracy = 0;
		int totalAcc = 0;


		noOfBlocks = (((Quote) quote.elementAt(0)).getRegion( )).mapdata.size( );
		// assuming it takes 2 secs for a robot to travel 50cm, so speed = 25cm/sec
		// each wall is 30cm, so approximately, it takes maximum of
		// 8 sec to go through a block where minimum time is (30*4)/25 = 4.8 sec

		// OR other method to estimate the max time of each block
		// since we are finding 10*10 = 100 blocks of map within 900 sec(more than enought to
		// scan through each block of the map at least once)
		// so the maximum time to scan through each block would be 900/100 = 9 sec

		// This should be less than 9 sec to scan through each block, so I take 8 sec in the program
		// as the maximum scanning time for each block.

		// I also find out the average price, average time and average accuracy from those 3 quotes
		// (may be used as an reference,don't know yet)
		// should we make our decision based on the average value or the our estimated value ?

		maxRobotTime = noOfBlocks * Const.robotTimePerBlock;

		for (int i=0;i<quote.size( );i++){
			thisQuote = (Quote) quote.elementAt(i);
			/* Don't accept the quote when the basic requirement is not meet
			   1. their estimated accuracy have to be above 400(??) out of 1000,
			   since it is waste of time and money to get an inaccurate map ?
			   2. their scanning time have to below the maximum estimated time
			*/

			if ((thisQuote.getTime( ) < maxRobotTime) || (thisQuote.getAccuracy( ) > 500)) {
				totalTime +=thisQuote.getTime( );
				totalPrice += thisQuote.getPrice( );
				NoOfRob++;
			}
		}

		// since no robot is accpeting our contract or non of them
		// can satisfy our basic requirement.

		if ((NoOfRob == 0) || (quote.size( ) == 0)) return null;

		//may need those information, don't know yet

		averageTime = totalTime/NoOfRob;
		averagePrice = totalPrice/NoOfRob;
		averageAccuracy = totalAcc/NoOfRob;

		priceRatio = 999999;

		for (int j=0;j<quote.size( );j++){
			thisQuote = (Quote) quote.elementAt(j);

			int avg_acc = 0;
			try {
				avg_acc = CartGroup2.commsman.getAccRep(thisQuote.getRobotID());
			} catch (Exception e) {
				System.out.println("ContractMeister.FindPrice(): exception " + e);
				System.exit(1);
			}
			if (avg_acc < 500) avg_acc = thisQuote.getAccuracy();

			long time_rep = 0;
			try {
				time_rep = CartGroup2.commsman.getTimeRep(thisQuote.getRobotID());
			} catch (Exception e) {
				System.out.println("ContractMeister.FindPrice(): exception " + e);
				System.exit(1);
			}

			thePrice = 
				(findA(avg_acc))
				*(findT(thisQuote.getTime( )
						+ time_rep
						* noOfBlocks,
						maxRobotTime))
				*100000;

			//choose the largest price ratio, which will be our best quote

			if (priceRatio > (thePrice/thisQuote.getPrice( ))){
				priceRatio = thePrice/thisQuote.getPrice( );
				bestQuote = thisQuote;
			}

		}
		return bestQuote;
    }

    /*	Pricing formula:
		Price = T * A * 100000
        time<180     T=1.0
        time>900     T=0.1
        time is directly proportional to the T within this range:
        180<= time <= 900

        accuracy<250     A=0.1
        accuracy>=250    A= 0.1 + (0.9 *
        (sqrt((accuracy-250)/100)/sqrt((1000-250)/100)))*/

    private double findA(int acc){
		return 0.1 + 0.9 * ((Math.sqrt(acc - 250)/100)/2.74);
    }

    private double findT(long t,int estimatedTime){
		// 180/900 = 0.2,so 1/5
		if (t < (estimatedTime/5)) {
			return 1.0;
		}
		else if (t >= (estimatedTime*0.9)) {
			return 0.1;
		}
		else {  //directly proportional
			return estimatedTime/t;
		}
    }
}




