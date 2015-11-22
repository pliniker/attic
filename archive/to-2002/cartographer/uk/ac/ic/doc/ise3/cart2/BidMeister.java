/*	BidMeister. The friendly control module.
 *
 *
 *      by Batman.
 */

package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;



class BidMeister {

    public BidMeister(){
		System.out.println("BidMeister.BidMeister(): constructed");
    }


	//
	//
	//
    public Lotbid newAuction(Lot incoming) throws java.rmi.RemoteException, InvalidAccountNoException {

		Region r = incoming.getRegion();

		int	inc_accuracy = incoming.getAccuracy();
		int imp_our = CartGroup2.mapman.importance(Const.ourId, incoming.getRegion(), inc_accuracy);

		Lotbid amt = CartGroup2.creditman.DoAuction(imp_our, incoming);

		CartGroup2.gui.println("BidMeister: bidding " + amt.price + " for lot " + 
							   incoming.getLotNo() + " (" + r.x1 + "," + r.y1 + "," + r.x2 + "," + r.y2 + ")");
		
		return amt;
    }


}


