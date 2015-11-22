//
// class Predictor
//
// 3 dimensional pseudo-reinforcement-learning matrix
//


package uk.ac.ic.doc.ise3.cart3;
import uk.ac.ic.doc.ise3.sp.*;
import java.io.*;



public class Predictor {

    int idim, jdim, kdim;
    int[][][] matrix;
    int lasti, lastj, lastk;
    int lastamt;


    //
    // normalisation functions
    //
    public static int normaliseCurrency(int amt) {
		int balanceIndex;

		if (amt > 7000) {
			//more than 7K
			balanceIndex = 9;
		} else if (amt > 4000) {
			//more than 4K
			balanceIndex = 8;
		} else if (amt > 2000) {
			//more than 2K
			balanceIndex = 7;
		} else if (amt > 1000) {
			//more than 1K
			balanceIndex = 6;
		} else if (amt > 800) {
			//more than 800
			balanceIndex = 5;
		} else if (amt > 500) {
			//more than 500
			balanceIndex = 4;
		} else if (amt > 300) {
			//more than 300
			balanceIndex = 3;
		} else if (amt > 200) {
			//more than 200
			balanceIndex = 2;
		} else if (amt > 100) {
			//more than 100
			balanceIndex = 1;
		} else {
			// between neg and 100
			balanceIndex = 0;
		}

		return balanceIndex;
    }

    public static int normaliseImportance(int acc) {
		if (acc >= 100) {
			return 9;
		}
		if (acc < 10) {
			return 0;
		}
		return (acc / 10) - 1;
    }


    //
    // get the predicted value
    //
    public int get(int i, int j, int k) {
		if ((i >= 0) && (i < idim) && 
			(j >= 0) && (j < jdim) &&
			(k >= 0) && (k < kdim)) {

			int amt = matrix[i][j][k];
			return amt;

		} else {
			System.out.println("Predictor.get(): out of bounds " +
							   i + ", " + j + ", " + k);
			return 0;
		}
    }


	//
	// get with the intention of updating later
	//
	public int getForUpdate(int i, int j, int k) {

		int amt = get(i, j, k);
		lasti = i;
		lastj = j;
		lastk = k;
		lastamt = amt;
		return lastamt;
	}


    //
    // update internal values
    //
    public void update(int result) {

		int adjust = (int)((Const.learningRate.get() * Const.scalingFactor.get() *
							(float)(result - lastamt)));
		
		// update active element
		int newpred = lastamt + adjust;
		if (newpred < 0) {
			newpred = 0;
		}
		matrix[lasti][lastj][lastk] = newpred;

		// coax neighbours towards it too
		if (lasti + 1 < idim) {
			int val = matrix[lasti + 1][lastj][lastk];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti + 1][lastj][lastk] = val;
			}
		}

		if (lasti - 1 >= 0) {
			int val = matrix[lasti - 1][lastj][lastk];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti - 1][lastj][lastk] = val;
			}
		}

		if (lastj + 1 < jdim) {
			int val = matrix[lasti][lastj + 1][lastk];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti][lastj + 1][lastk] = val;
			}
		}

		if (lastj - 1 >= 0) {
			int val = matrix[lasti][lastj - 1][lastk];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti][lastj - 1][lastk] = val;
			}
		}

		if (lastk + 1 < kdim) {
			int val = matrix[lasti][lastj][lastk + 1];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti][lastj][lastk + 1] = val;
			}
		}

		if (lastk - 1 >= 0) {
			int val = matrix[lastk][lastj][lastk - 1];
			val += (int)(0.1f * (float)adjust);
			if (val >= 0) {
				matrix[lasti][lastj][lastk - 1] = val;
			}
		}

    }



    //
    // dump array contents to disk
    //
    void store(String filename) throws IOException, FileNotFoundException{
		FileOutputStream f;
		try {
			f = new FileOutputStream(filename);
			DataOutputStream d = new DataOutputStream(f);
			for (int k = 0; k < kdim; k++) {
				for (int j = 0; j < jdim; j++) {
					for (int i = 0; i < idim; i++) {
						d.writeInt(matrix[i][j][k]);
					}
				}
			}
			f.close();
		}
		catch (Exception e) {
			System.out.println("Reinfo.storeData(" + filename
							   + "): exception " + e);
		}
    }


    //
    // load array contents from disk
    //
    void loadData(String filename) throws IOException {
		DataInputStream dat;
		try {
			dat = new DataInputStream(new FileInputStream(filename));
			for (int k = 0; k < kdim; k++) {
				for (int j = 0; j < jdim; j++) {
					for (int i = 0; i < idim; i++) {
						matrix[i][j][k] = dat.readInt();
					}
				}
			}
			dat.close();
		} catch (Exception e1) {
			System.out.println("Reinfo.loadData(): exception " + e1);
		}

    }



    //
    // size of each dimension
    //
    public Predictor(int isize, int jsize, int ksize) {
		idim = isize;
		jdim = jsize;
		kdim = ksize;

		matrix = new int[idim][jdim][kdim];

		for (int i = 0; i < idim; i++) {
			for (int j = 0; j < jdim; j++) {
				for (int k = 0; k < kdim; k++) {
					matrix[i][j][k] = 0;
				}
			}
		}
    }
	
}
