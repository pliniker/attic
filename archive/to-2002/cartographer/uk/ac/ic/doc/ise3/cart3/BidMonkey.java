//
// class BidMonkey
//
// bids for bananas
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.Vector;



public class BidMonkey {


	static long avBidDuration = 100; //the average bid duration.(msec)

    //
    // called by Comms when we win
    //
    public synchronized void win(Lot lot) {

		int amount = Misc.getwinner_amt(lot);
		run.cart.self().balance().dec(amount);
		run.cart.self().updateMap(lot.getRegion(), lot.getAccuracy());
		run.mon.updateProfit();

		// update all predictors
		Vector bids = lot.getAllBids();
		for (int i = 0; i < bids.size(); i++) {

			Bid b = (Bid)bids.get(i);
			if (b.getID() != Const.ourId) {
				run.cart.add(b.getID());

				// normalised payment
				float updt = (float)b.getAmount() *
					(1000.0f / (float)lot.getAccuracy()) *
					((float)(Const.xdim * Const.ydim) / (float)Misc.area(lot.getRegion()));

				run.cart.get(b.getID()).pred().update((int)updt);
			}
		}

		run.gui.println("Auction: won, paid " + amount);
    }


    //
    // called by Comms when we lose
    //
    public synchronized void lose(Lot lot) {

		int amount = Misc.getwinner_amt(lot);
		Bid w = Misc.getwinner_bid(lot);

		if (w.getAmount() >= lot.getReserve()) {
			if (w.getID() != Const.ourId) {
				run.cart.add(w.getID());
				run.cart.get(w.getID()).balance().dec(Misc.getwinner_amt(lot));
				run.cart.get(w.getID()).accmap().update(lot.getRegion(), lot.getAccuracy());
				run.gui.println("Auction: lost, winner bid " + w.getAmount());
			}
		} else {
			run.gui.println("Auction: lost - all under reserve");
		}

		// update all predictors
		Vector bids = lot.getAllBids();
		for (int i = 0; i < bids.size(); i++) {

			Bid b = (Bid)bids.get(i);
			if (b.getID() != Const.ourId) {
				run.cart.add(b.getID());

				// normalised payment
				float updt = (float)b.getAmount() *
					(1000.0f / (float)lot.getAccuracy()) *
					((float)(Const.xdim * Const.ydim) / (float)Misc.area(lot.getRegion()));

				run.cart.get(b.getID()).pred().update((int)updt);
			}
		}
    }


    //
    // called by Comms on new auction
	// returns price
    //
    public synchronized int newAuction(Lot lot) {

		run.gui.println("Auction: new auction");

		Region reg = lot.getRegion();
		int acc = lot.getAccuracy();
		int price = 0;

		int ourImportance = run.cart.self().accmap().importance(reg,acc);


		if (run.cart.count() > 0) {
		
			//getting highest enemy price.
			int maxPrice = 0;
			for (int i = 0; i < run.cart.count(); i++) {
				int imp = run.cart.getByIndex(i).accmap().importance(reg,acc);
				int their_balance = run.cart.getByIndex(i).balance().get();
				price = run.cart.getByIndex(i).pred().getForUpdate(Predictor.normaliseImportance(ourImportance),
																   Predictor.normaliseImportance(imp),
																   Predictor.normaliseCurrency(their_balance));
				if (price > maxPrice) {
					maxPrice = price;
				}
			}
			
			// un-normalise payment
			price = (int)( (float)price *
						   ((float)lot.getAccuracy() / 1000.0f) *
						   ((float)(Misc.area(lot.getRegion()) / (float)(Const.xdim * Const.ydim))) );
			
			//estimating profit based on the highest enemy price and the client-payment function.
			int profit = run.mon.getNetProfit(reg,acc,avBidDuration, price);
			if (profit >= (int)Const.ProfitThreshold.get()) {
				
				int amount = price + (int)Const.TopUpValue.get();
				run.gui.println("Auction: bidding " + amount);
				return amount;
			} else {
				return price;
			}

		} else {
			return (int)Const.TopUpValue.get();
		}
    }


    //
    //
    //
    public BidMonkey() {

    }
}

