/**
 * sliderPack. The class that encapsulates the slider+textfield+label
 * used in the settings and the main gui.
 *
 * DK 22/06/01
 */

package uk.ac.ic.doc.ise3.cart2;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.Hashtable;


public class sliderPack extends JPanel {

	JLabel 		myLbl;
	JTextField	myTxt;
	JSlider		mySlider;
	static String PrevContent = "" ;
	int divid = 1;
	boolean floating = false;
	int vars;


	//
	//contstructor. This has a hoooooorrible thing. the var<->int mapping. for the sake of liniker's health :)

	public sliderPack(int var_id, String lblText, int divider, sliderModel inmodel, int maj_tick, int min_tick,
					  boolean useCustom, Hashtable custom_lbls, boolean ticks, boolean plbls)
	{

		vars = var_id;
		setPreferredSize(new Dimension (200,80));
		divid = divider;
		//
		//the label
		myLbl = new JLabel(lblText, JLabel.CENTER);
		myLbl.setAlignmentX(Component.CENTER_ALIGNMENT);

		//
		//the slider
		mySlider = new JSlider(inmodel);
		mySlider.addChangeListener(new sliderListener());

		//
		//the textfield
		myTxt = new JTextField(4);
		myTxt.addActionListener(new txtListener());

		myTxt.setText(Integer.toString(inmodel.getValue()/divid));

		//
		//Turn on labels at major tick marks.
		mySlider.setMajorTickSpacing(maj_tick);
		mySlider.setMinorTickSpacing(min_tick);
		mySlider.setPaintTicks(ticks);
		mySlider.setPaintLabels(plbls);
        mySlider.setBorder(BorderFactory.createEmptyBorder(0,0,10,0));

        if (useCustom){
			mySlider.setLabelTable(custom_lbls);
		}

        //form the panel
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		this.setSize(500,200);
		this.setMinimumSize(new Dimension(500,200));

		//add the widgets in there
		this.add(myLbl);
		this.add(myTxt);
		this.add(mySlider);

	}

	public void snapTicks(boolean snap){
		mySlider.setSnapToTicks(snap);
	}

	//
	//the textbox listener
	class txtListener implements ActionListener {

		public void actionPerformed(ActionEvent a){

			boolean dotFound = false;

			System.out.println(myTxt.getText());
			char[] inChars = (myTxt.getText()).toCharArray();

 	        for (int i = 0; i < inChars.length; i++) {
				if ((!(Character.isDigit(inChars[i]) || inChars[i] == '.' && !dotFound))){
					myTxt.setText(PrevContent);
					PrevContent = myTxt.getText();
					return ;
				}
				if (inChars[i] == '.'){
					dotFound = true;
				}
			}
			mySlider.setValue((int)(Float.parseFloat(myTxt.getText())*divid));
			System.out.println("DEBUG -> TXT MODIFIED");
		}

	}

	//the slider listener
	class sliderListener implements ChangeListener {

		public void stateChanged(ChangeEvent a){
			if (!((JSlider)a.getSource()).getValueIsAdjusting()){
				System.out.println("sliderListener -> modified!");
				myTxt.setText(Float.toString((float)mySlider.getValue()/divid));
				PrevContent = Integer.toString(mySlider.getValue());

				switch (vars){
					case 0: //accuracyThreshold
						Const.accuracyThreshold = mySlider.getValue();
					case 1: //robot time per block (est)
						Const.robotTimePerBlock = mySlider.getValue();
					case 2: //InterContract (spitting) time.
						Const.contractSpewOutRate = mySlider.getValue();
					case 3: //spending limit per block.
						Const.spendLimitPerBlock = mySlider.getValue();
					case 4: //Max. concurrent contracts. (0-3)
						Const.maxContracts = mySlider.getValue();
					case 5: //Learning rate.
						Const.learningRate = (float)mySlider.getValue();
					case 6: //Scaling Factor
						Const.scalingFactor = (float)mySlider.getValue();
					default:
						//do nothing
					}
			}
		}
	}


}