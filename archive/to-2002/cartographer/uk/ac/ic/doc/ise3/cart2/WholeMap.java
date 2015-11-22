// Anthony Ng
// Class: WholeMap
// Represent our whole map information
// The 10*10 blocks are represenated by
// a vertical wall and horizontal wall
// array
//
// * updated 16/06 PL *
//


package uk.ac.ic.doc.ise3.cart2;
import uk.ac.ic.doc.ise3.sp.*;
import java.util.Vector;


public class WholeMap {

    boolean vWalls[ ][ ];
    boolean hWalls[ ][ ];
    int lengX;
    int lengY;


    //
    // util functions
    //
    public int getXdim() {
		return lengX + 1;
    }

    public int getYdim() {
		return lengY + 1;
    }

    public boolean getHWall(int x,int y){
		return hWalls[x][y];
    }

    public boolean getVWall(int x,int y){
		return vWalls[x][y];
    }


    //
    // another constructor for making part of the whole map
    // initialise a empty with no walls
    //
    public WholeMap(int lengX, int lengY){
		this.lengX = lengX;
		this.lengY = lengY;

		vWalls = new boolean[lengX + 1][lengY + 1];
		hWalls = new boolean[lengX + 1][lengY + 1];

		for (int i=0;i<=lengX;i++){
			for (int j=0;j<=lengY;j++){
				vWalls[i][j] = false;
				hWalls[i][j] = false;
			}
		}

		for (int i = 0; i <= lengX; i++) {
			hWalls[i][0] = true;
			hWalls[i][lengY] = true;
		}
		for (int i = 0; i <= lengY; i++) {
			vWalls[0][i] = true;
			vWalls[lengX][i] = true;
		}

		System.out.println("WholeMap.WholeMap(): constructed");
    }


    //
    // for each block information, add to vertical
    // and horizontal wall accordingly
    // Use "mask" for selective updating, null if not needed
    //
    public void updateBlock(Block c, Block mask, int x ,int y) {

		if (mask == null) {
			hWalls[x][y] = c.north;
			hWalls[x][y + 1] = c.south;
			vWalls[x][y] = c.west;
			vWalls[x + 1][y] = c.east;
		} else {
			if (mask.north) { hWalls[x][y] = c.north; }
			if (mask.south) { hWalls[x][y + 1] = c.south; }
			if (mask.west) { vWalls[x][y] = c.west; }
			if (mask.east) { vWalls[x + 1][y] = c.east; }
		}
    }




    //
    // debug dump
    //
    public void printMap( ){

    	//print the horizontal walls
		for(int y=0;y<=lengY;y++){
			for(int x=0;x<lengX;x++){
				System.out.println(hWalls[x][y]);
			}
		}

		// print the vertical walls
		for(int y=0;y<lengY;y++){ //add one in Y direction
			for(int x=0;x<=lengX;x++){
				System.out.println(vWalls[x][y]);
			}
		}

    }


    //
    // someone may ask for part of the map from us
    //
    public Vector getMap(int startX, int startY, int endX, int endY){

		Vector scan = new Vector();

		for (int j = startY; j <= endY; j++) {
			for (int i = startX; i <= endX; i++) {
				Block b = new Block(hWalls[i][j], vWalls[i + 1][j],
									hWalls[i][j + 1], vWalls[i][j]);
				scan.add(b);
			}
		}

		return scan;
    }


}


























