//
// class Balance
//
// just handles balances
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;


public class Balance {

    int amount, lockamt;
    
    //
    // general access functions
    //
    public int get() {
		return amount;
    }
    
    public synchronized void set(int amt) {
		amount = amt;
    }

    public synchronized void inc(int amt) {
		amount += amt;
    }

    public synchronized void dec(int amt) {
		amount -= amt;
    }

    public synchronized void lock(int amt) {
		lockamt += amt;
    }

    public synchronized void unlock(int amt) {
		lockamt -= amt;
		if (lockamt < 0) {
			lockamt = 0;
		}
    }

	public synchronized void update() throws java.rmi.RemoteException, InvalidAccountNoException {
		amount = run.com.getBalance();
	}


    //
    // just the starting balance
    //
    public Balance(int initialAmt) {
		amount = initialAmt;
		lockamt = 0;
    }

}
