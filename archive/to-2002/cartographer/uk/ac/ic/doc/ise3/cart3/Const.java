//
// useful numbers which shouldn't change more than once
//

package uk.ac.ic.doc.ise3.cart3;


public class Const {

    public static int ourId;
    public static int accountNo;

    public static int xdim = 10;
    public static int ydim = 10;

    public static int maxarea = 10;

    // reinforcement data file extension
    public static String fileExt;

	// variable objects
	public static Var scalingFactor		 = new Var(1.0f);
	public static Var learningRate		 = new Var(1.0f);
	public static Var interSellT		 = new Var(5000.0f);
	public static Var maxContracts		 = new Var(3.0f);
	public static Var interContractT	 = new Var(5000.0f);
	public static Var ProfitThreshold	 = new Var(1.0f);
	public static Var TopUpValue		 = new Var(10.0f);

	// stats section

	public static Var avBlkPrice         = new Var(100.0f);
	public static Var maxOpp			 = new Var(0.0f);
	public static Var ourProgress 		 = new Var(0.0f);
	public static Var profit			 = new Var(0.0f);

	public static long startTime = 0;

	public static boolean doContracting = true;
	public static boolean doSelling     = true;
}
