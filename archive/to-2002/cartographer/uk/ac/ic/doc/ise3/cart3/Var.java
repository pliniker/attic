//
// class Var
//
// numbers which WILL change
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;



public class Var {

	float var;


	public void set(float val) {
		var = val;
	}

	public float get() {
		return var;
	}


	public Var(float initial) {
		var = initial;
	}
}
