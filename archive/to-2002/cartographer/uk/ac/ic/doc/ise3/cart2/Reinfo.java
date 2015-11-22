/***************************************
 *              Reinfo.java            *
 *  the reinforcement learning class   *
 *                                     *
 *                                     *
 *       By batman and Spiderman       *
 *                                     *
 ***************************************/

package uk.ac.ic.doc.ise3.cart2;
import java.io.*;



public class Reinfo {

    int[][][] matrix; //[our imp][their imp][balance]
    int lastEstimate, lastX, lastY, lastZ, myID;
    int inx1,inx2,inx3;

    public Reinfo(int index1, int index2, int index3){
		inx1=  index1;
		inx2 = index2;
		inx3 = index3;
		initArray();
		System.out.println("Reinfo.Reinfo(): constructed");
    }


    //
    // apply learning algorithm
    //
    public void update(int Result){
		// update the last transaction's coordinates (and neighbouring) based on the result

		// update the actual coordinate.
		int newprice = (int)((Const.learningRate * Const.scalingFactor *
							  (float)( Result - lastEstimate )) + lastEstimate);
		if (newprice < 0) {
		    newprice = 0;
		}
		matrix[lastX][lastY][lastZ] = newprice;

		// update the neibouring
		if (lastX + 1 < inx1) {
		        matrix[lastX+1][lastY][lastZ] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));
			
			if (matrix[lastX+1][lastY][lastZ] < 0) {
			    matrix[lastX+1][lastY][lastZ] = 0;
			}
		}

		if (lastX - 1 >= 0) {
			matrix[lastX-1][lastY][lastZ] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));

			if (matrix[lastX-1][lastY][lastZ] < 0) {
			    matrix[lastX-1][lastY][lastZ] = 0;
			}
		}

		if (lastY + 1 < inx2) {
			matrix[lastX][lastY+1][lastZ] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));

			if (matrix[lastX][lastY+1][lastZ] < 0) {
			    matrix[lastX][lastY+1][lastZ] = 0;
			}
		}

		if (lastY - 1 >= 0) {
			matrix[lastX][lastY-1][lastZ] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));

			if (matrix[lastX][lastY-1][lastZ] < 0) {
			    matrix[lastX][lastY-1][lastZ] = 0;
			}
		}

		if (lastZ + 1 < inx3) {
			matrix[lastX][lastY][lastZ+1] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));

			if (matrix[lastX][lastY][lastZ+1] < 0) {
			    matrix[lastX][lastY][lastZ+1] = 0;
			}
		}

		if (lastZ - 1 >= 0) {
			matrix[lastX][lastY][lastZ-1] +=
				(int)(0.1*(Const.learningRate * Const.scalingFactor * (float)( Result - lastEstimate )));

			if (matrix[lastX][lastY][lastZ-1] < 0) {
			    matrix[lastX][lastY][lastZ-1] = 0;
			}
		}

    }


    //
    // get predicted price
    //
    public int getResult(int imp_our, int imp_emy, int curBalance){
	//return the estimate.

	
       	int balanceIndex = 0;
	/*	imp_our /= 10;
		imp_emy /= 10;

		if (curBalance > 7000) {
			//more than 7K
			balanceIndex = 9;
		} else if (curBalance > 4000){
			//more than 4K
			balanceIndex = 8;
		} else if (curBalance > 2000){
			//more than 2K
			balanceIndex = 7;
		} else if (curBalance > 1000){
			//more than 1K
			balanceIndex = 6;
		} else if (curBalance > 800){
			//more than 800
			balanceIndex = 5;
		} else if (curBalance > 500){
			//more than 500
			balanceIndex = 4;
		} else if (curBalance > 300){
			//more than 300
			balanceIndex = 3;
		} else if (curBalance > 200){
			//more than 200
			balanceIndex = 2;
		} else if (curBalance > 100){
			//more than 100
			balanceIndex = 1;
		} else {
			//between 0 and 100
			balanceIndex = 0;
		}
		*/

		//update 'last' variables and return the lastEstimate.
		lastX = imp_our;
		lastY = imp_emy;
		lastZ = balanceIndex;
		if (lastX < 0) lastX = 0;
		if (lastX > inx1 - 1) lastX = inx1 - 1;
		if (lastY < 0) lastY = 0;
		if (lastY > inx2 - 1) lastY = inx2 - 1;
		if (lastZ < 0) lastZ = 0;
		if (lastZ > inx3 - 1) lastZ = inx3 - 1;
		lastEstimate = matrix[lastX][lastY][lastZ];

		return lastEstimate;
    }


    //
    // variable depending on input
    //
    void initArray() {
		int x,y,z;
		
		matrix = new int[inx1][inx2][inx3];
		
		for (x=0;x<inx1;x++){
			for (y=0;y<inx2;y++){
				for (z=0;z<inx3;z++){
					matrix[x][y][z] = (int)(8000.0f*(float)((float)y / 7.0f));
				}
			}
		}
    }
	

    //
    // dump array contents to disk
    //
    void storeData(String filename) throws IOException, FileNotFoundException{
		FileOutputStream f;
		try {
		    f = new FileOutputStream(filename);
			DataOutputStream d = new DataOutputStream(f);
			for (int k = 0; k < inx3; k++) {
				for (int j = 0; j < inx2; j++) {
					for (int i = 0; i < inx1; i++) {
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
			for (int k = 0; k < inx3; k++) {
				for (int j = 0; j < inx2; j++) {
					for (int i = 0; i < inx1; i++) {
						matrix[i][j][k] = dat.readInt();
					}
				}
			}
			dat.close();
		} catch (Exception e1) {
			System.out.println("Reinfo.loadData(): exception " + e1);
		}

    }

}
