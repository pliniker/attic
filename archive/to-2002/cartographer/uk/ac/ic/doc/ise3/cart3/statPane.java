package uk.ac.ic.doc.ise3.cart3;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;



public class statPane extends JPanel {

	ProgressCombo maxOpp = new ProgressCombo("Maximum Opponent Progress",0,1000);
	ProgressCombo ourProgress = new ProgressCombo("Own Progress",0,1000);

	public statPane() {
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

        //this.setPreferredSize(new Dimension(640,480));
		//this.setSize(500,200);
		//this.setMinimumSize(new Dimension(500,200));

		this.add(maxOpp);
		this.add(ourProgress);

    }


	class ProgressCombo extends JPanel{
		JProgressBar myBar;
		JLabel	myLabel;

		public ProgressCombo(String txtLabel, int min, int max){
			myBar = new JProgressBar(JProgressBar.HORIZONTAL,min,max);
			myLabel = new JLabel(txtLabel,JLabel.LEFT);

			//this.setPreferredSize(300,100);
			//this.setMinimumSize(400,200);
			this.add(myLabel);
			this.add(myBar);
		}

		public void updateProgress(int Value){
			myBar.setValue(Value);
			this.repaint();
		}
	}

	public void paint(Graphics g){
		System.out.println("DEBUG: statPane.paint()");
		maxOpp.updateProgress((int)Const.maxOpp.get());
		ourProgress.updateProgress((int)Const.ourProgress.get());
	}
}