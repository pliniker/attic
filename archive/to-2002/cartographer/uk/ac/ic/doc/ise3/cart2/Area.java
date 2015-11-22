

/*
 * PL
 */

package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;



public class Area {

    int x1, y1, x2, y2;

    public int area() {
	return (x2 - x1 + 1) * (y2 - y1 + 1);
    }

    public Area(int a, int b, int c, int d) {
	x1 = a;
	x2 = c;
	y1 = b;
	y2 = d;
    }

    public Area(Region r) {
	x1 = r.x1;
	x2 = r.x2;
	y1 = r.y1;
	y2 = r.y2;
    }
}
