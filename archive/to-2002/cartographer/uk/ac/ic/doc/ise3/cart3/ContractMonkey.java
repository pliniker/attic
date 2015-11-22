//
// class ContractMonkey
//
// bananas
//

package uk.ac.ic.doc.ise3.cart3;


import uk.ac.ic.doc.ise3.sp.*;
import java.util.Vector;


public class ContractMonkey extends Thread {

	int numContracts = 0;
	boolean pendingContract = false;
	Region pendingRegion;


	synchronized void up() {
		numContracts++;
	}

	synchronized void down() {
		numContracts--;
	}



    //
    // contract has been delivered
    //
    public synchronized void deliver(Region r, int acc) throws InvalidAccountNoException, java.rmi.RemoteException {
		run.cart.self().updateMap(r, acc);
		run.cart.self().accmap().unlock(r);
		run.cart.self().balance().update();

		// is locking the amount important?
		//run.cart().self().balance().unlock(amt);

		down();
		// notify Monitor of finished contract
		run.mon.contDec();

		run.gui.println("Contract: delivered");
    }


    //
    // choose one from the few
	//
    public synchronized void  pickQuote(Vector quotes) throws InvalidAccountNoException, java.rmi.RemoteException {

		run.gui.println("Contract: " + quotes.size() + " quotes received");
		
		if (quotes.size() == 0) {
			run.cart.self().accmap().unlock(pendingRegion);
		} else {

			int maxprofit = -9999999;
			
			Quote picked = null;
			for (int i = 0; i < quotes.size(); i++) {
				Quote q = (Quote)quotes.get(i);
				int  accuracy = (q.getAccuracy() + run.com.getAccRep(q.getRobotID())) / 2;
				long time = q.getTime() + 
					(Misc.area(q.getRegion()) * 
					 run.com.getTimeRep(q.getRobotID()));
				int profit = run.mon.getNetProfit(q.getRegion(), accuracy, time, q.getPrice());
				
				if (profit > maxprofit) {
					maxprofit = profit;
					picked = q;
				}
			}
			
			// if there was a worthwhile quote, pick it
			if ((picked != null) && (maxprofit > 0)) {
				run.com.acceptContract(picked);
				run.cart.self().balance().dec(picked.getPrice());
				// notify the Monitor of a new contract
				run.mon.contInc();
				up();

				run.gui.println("Contract: picking price " + picked.getPrice());
			}
			
			// is locking the amount important?
			//run.cart().self().balance().lock(picked.getPrice());
		}

		pendingContract = false;
    }


    //
    // send out contracts
    //
    public void run() {
		while (Const.doContracting) {
			try {
				run.gui.println("Contracts: " + numContracts + " pending");
				while ((numContracts < (int)Const.maxContracts.get()) &&
					   (!pendingContract)) {
					
					// if we have a reasonable amount of money
					if (run.cart.self().balance().get() > 1000) {

						Region r = run.cart.self().accmap().unchartedRegion(Const.maxarea);
						run.cart.self().accmap().lock(r);
						run.com.contractRegion(r);
						run.gui.println("Contract: requesting");

						pendingRegion = r;
						pendingContract = true;
					}
					
					Thread.sleep( (long)Const.interContractT.get() );
				}

				Thread.sleep( (long)Const.interContractT.get() );
			}
			catch (Exception e) {
				System.out.println("ContractMonkey.run(): exception " + e);
				e.printStackTrace();
			}
		}
    }


	//
	// a general contract has been made by another cartographer
	//
	public void contractMade(int cartID, int robotID, Quote quote) {
		run.cart.add(cartID);
		run.cart.get(cartID).accmap().update(quote.getRegion(), quote.getAccuracy());
		run.cart.get(cartID).balance().dec(quote.getPrice());
		run.gui.println("Contract: other party contracted at " + quote.getPrice());
	}


    //
    //
    //
    public ContractMonkey() {
		System.out.println("ContractMonkey.ContractMonkey(): constructed");
    }

}
