/*
 * 1.2 version. dk and pl. updated by dk.
 */

package uk.ac.ic.doc.ise3.cart3;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.*;
import java.util.Vector;



public class MapGrid extends Canvas {

    // walls data
    WallMap wall;
    AccMap acc;

    // colour palette
    Color[] palette;
	Color inaccessible;


    //
    // needs params from MapMaster
    //
    public MapGrid(WallMap s, AccMap t) {
		wall = s;
		acc = t;

		palette = new Color[11];
		palette[0] = new Color(127, 127, 127);
		for (int i = 1; i < 5; i++) {
			palette[i] = new Color(250, 50*i, 0);
			palette[5 + i] = new Color(250 - 50*i, 250, 0);
		}

		inaccessible = Color.black;

		System.out.println("MapGrid.MapGrid(): constructed");
    }


	//
	// force a screen update
	//
	public void forceUpdate() {
		repaint();
	}


    //
    // paint function - draws grid, accuracy, walls
    //
    public void paint(Graphics g) {
        Graphics2D g2 = (Graphics2D) g;

        Dimension d = getSize();
        int gridWidth = d.width/10;
        int gridHeight = d.height/10;

		// draw accuracy as shade of red-yellow-green(?)
		// inaccessible regions black.
		for (int j = 0; j < acc.getYDim(); j++) {
			for (int i = 0; i < acc.getXDim(); i++) {
				if (wall.getHWall(i, j) && wall.getHWall(i, j + 1) &&
					wall.getVWall(i, j) && wall.getVWall(i + 1, j)) {
					g2.setColor(inaccessible);
				} else {
					g2.setColor(palette[acc.get(i, j) / 101]);
				}

				g2.fillRect(i * gridWidth + 1,
							j * gridHeight + 1,
							gridWidth - 1,
							gridHeight - 1);
			}
		}

		g2.setColor(Color.black);

		// draw horizontal walls
		for (int j = 0; j < wall.getYDim(); j++) {
			for (int i = 0; i < wall.getXDim(); i++) {
				if (wall.getHWall(i, j)) {
					g2.fillRect(i * gridWidth,
								j * gridHeight - 1,
								gridWidth,
								4);
				}
			}
		}

		// draw vertical walls
		for (int j = 0; j < wall.getYDim(); j++) {
			for (int i = 0; i < wall.getXDim(); i++) {
				if (wall.getVWall(i, j)) {
					g2.fillRect(i * gridWidth - 1,
								j * gridHeight,
								4,
								gridHeight);
				}
			}
		}

    }


}

