//
// class CartInfo
//
// package various others together
//

package uk.ac.ic.doc.ise3.cart3; 
import uk.ac.ic.doc.ise3.sp.*; 



public class CartInfo {

	int id;
    Predictor p;
    AccMap a;
    Balance b;
    

    //
    // general access functions
    //
	public int getId() {
		return id;
	}

    public Predictor pred() {
		return p;
    }

    public AccMap accmap() {
		return a;
    }

    public Balance balance() {
		return b;
    }


    //
    // id; map x,y; predictor i,j,k; balance bal
    //
    public CartInfo(int i, int xdim, int ydim, int idim, int jdim, int kdim, int bal) {
		id = i;
		p = new Predictor(idim, jdim, kdim);
		a = new AccMap(xdim, ydim);
		b = new Balance(bal);
    }
}
