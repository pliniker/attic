package uk.ac.ic.doc.ise3.cart3;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;


public class lblStats extends JPanel {

	JLabel lblAvBlkP;
	JLabel ourProfit;

	public lblStats() {
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));


		JLabel lblAvBlkP =  new JLabel("Average Block Price: "+ new Float(Const.profit.get()).toString(),JLabel.LEFT);
		JLabel ourProfit =  new JLabel("Profit to date: "+ new Float(Const.avBlkPrice.get()).toString(),JLabel.LEFT);

		this.add(lblAvBlkP);
		this.add(ourProfit);
	}


	public void paint(){
		ourProfit.setText("Profit to date: " + new Float(Const.profit.get()).toString());
		lblAvBlkP.setText("Average Block Price :" + new Float(Const.avBlkPrice.get()).toString());
		this.repaint();
	}
}
