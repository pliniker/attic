/* sellMesiter created by Anthony
   making money by selling our own map to the other cartopgrapher
   try to sell when we don't have enought money 
   
   a thread waiting for credit master calling
   also constantly constructing the map for sale
   call peter to see the what we can sell
   call di about the average block price
   estimate the price which we can sell
   
*/

package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;
import java.lang.Math; 
   
public class SellMeister extends Thread {
    int sellingPrice;
    //int submitRegion;
    Lotbid otherLot;
    int maxPrice = 0;
    float avBlockPrice = 0; 
    int maxBlocks = 0;
    Area sellArea;
    Region submitRegion;
    int regionAccuracy;
    

    public SellMeister( ){
		System.out.println("SellMeister.SellMeister(): constructed");
    }
    

    //when to make a sale, only when our
    // balance below certain limit ?
    public void run( ){
		System.out.println("SellMeister.run(): started");
	
		try{
			while (true){
				// get the highest price which our enemy can afford
				// return from the credit mesiter is a lotbid
				otherLot = CartGroup2.creditman.DoSell(); 

				maxBlocks = (int) ((float)otherLot.price / (float)otherLot.av_block_price);  // avoid div-0 errors
				if (maxBlocks != 0) {
					sellArea = CartGroup2.mapman.getRegionToAuction(maxBlocks);
					if (sellArea.area() > 1) {

						submitRegion = CartGroup2.mapman.getRegion(sellArea); 
						
						// the average block price is based on accuracy = 1000; 
						regionAccuracy = CartGroup2.mapman.areaAccuracy(Const.ourId, sellArea); 
						sellingPrice = (int) (((float)(submitRegion.mapdata.size( ) * otherLot.av_block_price)) * (float)regionAccuracy/1000.0f); 
						
						// only sell the map when the other cartopgrapher's overall
						// accuracy is less than 800
						// or put the price up
						
						CartGroup2.gui.println("SellMeister: selling ("+submitRegion.x1+","+submitRegion.y1+","+submitRegion.x2+","+submitRegion.y2+")");
						
						boolean theyMightWantSomething = true;
						for (int i = 0; i < Const.numCarts(); i++) {
							int id = Const.getCart(i);
							if (CartGroup2.mapman.mapAccuracy(id) >= Const.accuracyThreshold) {
								theyMightWantSomething = false;
							}
						}
						
						if (theyMightWantSomething) {
							// then sell the map 
							// put our map to the aution
							// submitRegion(Region r, int reserve, int accountNo)
							CartGroup2.commsman.sellMap(sellingPrice, submitRegion);
							
						}
						else {
							// put the price up by 10% ?
							CartGroup2.commsman.sellMap((int) ((float)sellingPrice * 1.1f), submitRegion);
						}
					}
				} else {
					CartGroup2.gui.println("SellMeister: not selling");
				}
				
				//try to sell a map every 5 sec ?
				Thread.sleep(5000);
			}
		}
		catch (Exception e){ 
			System.out.println("SellMeister.run(): exception " + e);
			e.printStackTrace();
		}
    }
}

