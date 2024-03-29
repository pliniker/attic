/**
 *  The Monitoring Thread.
 *  DK. 24/6/01
 *
 *
 **/

package uk.ac.ic.doc.ise3.cart3;

import java.util.LinkedList;
import uk.ac.ic.doc.ise3.sp.*;


public class Monitor  extends Thread {


    LinkedList  prvProfitDeltas;
    int vecsize, totalProfit, pendContracts;


    //
    // constructor
    public Monitor(int initVecSize) {
		prvProfitDeltas = new LinkedList();
		vecsize = initVecSize;
		totalProfit = 0;
		prvProfitDeltas.add(new Float(0.0f)); //adding initial 0 profit
    }


    //
    // updateProfit
    public void updateProfit() {

		int ourMapAcc = run.cart.self().accmap().totalAcc();
		float profit;
		long time = 0;

		try {
			time = run.com.getTime();
		} catch (java.rmi.RemoteException e1) {
			System.out.println("Monitor.updateProfit(): Could not get time (RemoteException)");
		}

		//using the client-payment function to see how much we'd get if
		//we bailed out now.

		profit = clientPays(time - Const.startTime, ourMapAcc);

		prvProfitDeltas.addLast(new Float(profit - totalProfit));
		
		//truncating size...
		while (prvProfitDeltas.size() > vecsize){
			prvProfitDeltas.removeFirst();
		}

		totalProfit = (int)profit;
    }

    //
    // clientpays applies the client-payment function and returns the money the client will pay.
    //
    float clientPays(long t, int accuracy){

		final int MIN_TIME = 180;
		final int MAX_TIME = 900;
		final int MIN_ACCURACY = 250;
		float T , A;

		long time = t / 1000;

		if (time < MIN_TIME) T = 1;
		else if(time>MAX_TIME) T = 0.1f;
		else T = 1.0f - (0.9f * ((float)(time - MIN_TIME) / (float)(MAX_TIME - MIN_TIME)));

		if(accuracy < 250) A = 0.1f;
		else A = 0.1f + 
				 (float)(0.9f * (Math.sqrt((float)(accuracy - MIN_ACCURACY) / 100.f) / 
								 Math.sqrt((float)(1000 - MIN_ACCURACY) / 100.f)));

		return  A * T * 10000.0f ;
    }


    //
    // contInc() . increment pending contracts.
    public void contInc(){
		pendContracts++;
    }

    //
    // contDec() . decrement pending contracts.
    public void contDec(){
		pendContracts--;
    }

    //
    // getTotalProfit()
    //
    public int getTotalProfit(){
		return totalProfit;
    }


    // getNetProfit(). Returns the difference in the profit if the transaction
    // with the following parameters takes place.
    //
    public int getNetProfit(Region r, int a, long duration, int price){
		AccMap copy  = run.cart.self().accmap().mapCopy();
		copy.update(r,a);
		int accuracy  = copy.totalAcc();
		int potProfit = 0;
		try {
			potProfit = (int)clientPays((run.com.getTime() + 50 + duration),accuracy); // note: 50ms = time to getTime()
		} catch (java.rmi.RemoteException e2){
			System.out.println("Monitor.getNetProfit(): Error while getting time! (Remote Exception)");
			potProfit = totalProfit - price - 100; // note: 100ms delay of doing all this.
		}
		//updateProfit();
		return (potProfit - totalProfit - price);
    }



    //
    // the monitor's run method. this:
    // 1. updates 'peter's mood'
    // 2. checks whether it is time to finish.
    // 3. stops bidding if no money available.
    public void run() {

		while (true) {
			int highacc = 0 ;
			int enemyacc, ouracc = 0;
			
			// ******** UPDATE THE TOTALPROFIT  **********
			
			if (run.cart.self().balance().get() < 0){ vecsize = 2; }
			
			updateProfit();

			//
			// *** check for 10 negative profits
			//
			//checking previous n entries for negative values.
			int noOfNegs = 0;
			for (int x = 0; x < prvProfitDeltas.size(); x++) {
				if (((Float)prvProfitDeltas.get(x)).floatValue() < 0.0f) {
					noOfNegs++;
				}
			}

			if (noOfNegs > 0) {
				System.out.println("Monitor.run(): making a loss for " + noOfNegs + " transactions");
			} else {
				try {
					long time = run.com.getTime();
					System.out.println("<<< RUNNING TIME + " + ((time - Const.startTime) / 1000) + " >>>");
				}
				catch (Exception e) {
				}
			}

			//
			// bail out if we're making a loss
			//
			
			try {
				if (noOfNegs == vecsize){
					Region map = new Region();
					map.x1 = 0;
					map.x2 = Const.xdim - 1;
					map.y1 = 0;
					map.y2 = Const.ydim - 1;
					Const.doContracting = false;
					Const.doSelling = false;
					run.com.finish(run.cart.self().wallmap().getWallMap(map));
					run.gui.println("<<< SYSTEM HAS FINISHED >>>");
					long time = run.com.getTime();
					run.gui.println("<<< RUNNING TIME + " + ((time - Const.startTime) / 1000) + " >>>");
					run.gui.println("<<< BALANCE = " + run.com.getBalance() + " >>>");
					while (true) {
						Thread.sleep(1000);
					}
				}
			}
			catch (Exception e) {
				System.out.println("OWWW! Could not finish!");
				System.exit(1);
			}
			
			// ******** UPDATE THE LINIKER FACE **********
			//getting the highest enemy acc.
			for (int x = 0; x < run.cart.count(); x++){
				enemyacc = run.cart.getByIndex(x).accmap().totalAcc();
				if (enemyacc > highacc) { highacc = enemyacc; }
			}

			ouracc = run.cart.self().accmap().totalAcc();
			Const.maxOpp.set((float)highacc);
			Const.ourProgress.set((float)ouracc);
			Const.profit.set((float)totalProfit);

			//updating THE FACE.
			ouracc = run.cart.self().accmap().totalAcc();
			if (highacc > ouracc + 200) { run.gui.updatePic("PeteLose.jpg"); }
			else if (highacc + 200 > ouracc) { run.gui.updatePic("PeteWin.jpg"); }
			else { run.gui.updatePic("PeteWin.jpg"); }
			
			try {
				Thread.sleep(1000);
			}
			catch (Exception e) {
				System.out.println("Monitor.run(): thread exception " + e);
			}
		}
	}
}
