//
// class SellMonkey
//
// sells bananas
//

package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.Vector;



public class SellMonkey extends Thread {


    //
    // called by Comms on sale/non-sale of Lot
    //
    public synchronized void result(Lot lot, int outcome) {
		switch (outcome) {

		// in the case that we sold the lot
		case AuctionServer.YOU_SOLD_LOT:
			// update the winner's info
			Bid b = Misc.getwinner_bid(lot);
			run.cart.add(b.getID());
			run.cart.get(b.getID()).balance().dec(Misc.getwinner_amt(lot));
			run.cart.get(b.getID()).accmap().update(lot.getRegion(), lot.getAccuracy());

		case AuctionServer.BELOW_RESERVE:
			// update all predictors
// 			Vector bids = lot.getAllBids();
// 			for (int i = 0; i < bids.size(); i++) {

// 				b = (Bid)bids.get(i);
// 				run.cart.add(b.getID());

// 				// normalised payment
// 				float updt = (float)b.getAmount() *
// 					(1000.0f / (float)lot.getAccuracy()) *
// 					((float)(Const.xdim * Const.ydim) / (float)Misc.area(lot.getRegion()));

// 				run.cart.get(b.getID()).pred().update((int)updt);
// 			}

			break;

		// unsold lot
		default:

		}
    }


    //
    // sell things thread
    //
    public void run() {
		
		try {
			Thread.sleep( (long)Const.interSellT.get() / 2 );
		} 
		catch (Exception e) {
			System.out.println("nonsense exception");
		}

		while (Const.doSelling) {

				// general idea:
				// if the region to auction is more accurate than at least
				// one other cartographer's map then it's worth trying
				// to sell it.
				int importance = 0;

				try {
					Thread.yield();

					if (run.cart.count() > 0){

						Region r = run.cart.getRegionToAuction(Const.maxarea);
						int racc = run.cart.self().accmap().regionAcc(r);
						int ourimp = run.cart.self().accmap().importance(r, racc);

						int maximp = 0;
						int reserve = 0;
						for (int i = 0; i < run.cart.count(); i++) {
							int bal = run.cart.getByIndex(i).balance().get();
							int imp = run.cart.getByIndex(i).accmap().importance(r, racc);
							int price = run.cart.getByIndex(i).pred().get(Predictor.normaliseImportance(ourimp), 
																		  Predictor.normaliseImportance(imp),
																		  Predictor.normaliseCurrency(bal));

							if (imp > maximp) {
								maximp = imp;
								reserve = price;
							}
						}


						reserve = (int)( (float)reserve *
							       ((float)racc / 1000.0f) *
							       ((float)(Misc.area(r) / (float)(Const.xdim * Const.ydim))) );
						
						
						if ((maximp <= ourimp) && (reserve > 0)) {
							run.com.sellMap(reserve, run.cart.self().wallmap().getWallMap(r));
							run.gui.println("Sell: reserve price " + reserve);
						}
					}
					Thread.sleep( (long)Const.interSellT.get() );
				}
				catch (Exception e) {
						System.out.println("SellMonkey.run(): exception " + e);
						e.printStackTrace();
				}

		}

	}



    //
    //
    //
    public SellMonkey() {
    }

}
