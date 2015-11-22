/*
 * 1.2 version. dk and pl. updated by dk.
 */

package uk.ac.ic.doc.ise3.cart2;

import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.*;
import java.util.Vector;



public class MapGrid extends Canvas {

    // walls data
    WholeMap wall;
    CartMap acc;

    // colour palette
    Color[] palette;


    //
    // needs params from MapMaster
    //
    public MapGrid(WholeMap s, CartMap t) {
		wall = s;
		acc = t;

		palette = new Color[256];
		palette[0] = new Color(127, 127, 127);
		for (int i = 1; i < 128; i++) {
			palette[i] = new Color(255, 2*i, 0);
			palette[127 + i] = new Color(255 - 2*i, 255, 0);
		}
		
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
		for (int j = 0; j < acc.getYdim(); j++) {
			for (int i = 0; i < acc.getXdim(); i++) {
				g2.setPaint(palette[acc.getMapping(i, j) / 4]);
				g2.setColor(palette[acc.getMapping(i, j) / 4]);
				g2.fillRect(i * gridWidth + 1,
							j * gridHeight + 1,
							gridWidth - 1,
							gridHeight - 1);
			}
		}

		g2.setPaint(Color.black);
		g2.setColor(Color.black);

		// draw horizontal walls
		for (int j = 0; j < wall.getYdim(); j++) {
			for (int i = 0; i < wall.getXdim(); i++) {
				if (wall.getHWall(i, j)) {
					g2.fillRect(i * gridWidth,
								j * gridHeight - 1,
								gridWidth,
								4);
				}
			}
		}

		// draw vertical walls
		for (int j = 0; j < wall.getYdim(); j++) {
			for (int i = 0; i < wall.getXdim(); i++) {
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

