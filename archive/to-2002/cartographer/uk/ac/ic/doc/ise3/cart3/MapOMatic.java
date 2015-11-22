
/*
 * PL and dk. updated by dk.
 */


package uk.ac.ic.doc.ise3.cart3;


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.Hashtable;




public class MapOMatic extends JFrame {


    private JList debug;
    private DefaultListModel debugModel;

    MapGrid canvas;
    JPanel canvasPane;
    JPanel sliderPane1, sliderPane2, slidersPane;
    JScrollPane debugScrollPane;
    JSplitPane mainSplit; // banana-split :P
    JSplitPane ultraSplit; // the whole thing.(vertical)
    JPanel bottom, leftControls, rightControls;
    JTabbedPane daTabs;
    static String file = "peteNeutral.jpg";
    JLabel picture;
    JFrame moodframe;
    statPane stats;



    public MapOMatic(MapGrid c) {





        super("Map-O-Matic");

		canvas = c;


		// ##### THE DEBUG LIST #####

        debugModel = new DefaultListModel();
        debugModel.addElement("---------------------------------------");
        debugModel.addElement(" This program was brought to you by..." );
        debugModel.addElement("         Dimosthenis Kaponis"		   );
        debugModel.addElement("            Peter Liniker"			   );
		debugModel.addElement("             Anthony Ng"				   );
		debugModel.addElement("---------------------------------------");


        //Create the debug and put it in a scroll pane
        debug = new JList(debugModel);
        debug.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        debug.setSelectedIndex(0);
		debugScrollPane = new JScrollPane(debug);
		debugScrollPane.setPreferredSize(new Dimension(300, 500));
		debugScrollPane.setMinimumSize(new Dimension(300,200));


		// ########## SLIDER STUFF ##########
		// ##################################


		//   >>  interSellingTime

		Hashtable labelTable = new Hashtable();
		labelTable.put( new Integer( 0 ), new JLabel("0") );
		labelTable.put( new Integer( 10000 ), new JLabel("10") );
		labelTable.put( new Integer( 20000 ), new JLabel("20") );

		sliderModel inmodel1 = new sliderModel(1000,20000,5000);
		sliderPack  sellSlider  = new sliderPack(Const.interSellT,"Selling Delay Time",1000,inmodel1,2000,0,true,labelTable,true,true);

		//   >>  ProfitThreshold

		sliderModel thresmodel = new sliderModel(0,2000,10);
		sliderPack  thresSlider  = new sliderPack(Const.ProfitThreshold,"Profit Threshold",1,thresmodel,500,0,false,null,true,true);


		//   >>  ContractSpewOutRate
		Hashtable labelTable2 = new Hashtable();
		labelTable2.put( new Integer( 0 ), new JLabel("0") );
		labelTable2.put( new Integer( 10000 ), new JLabel("10") );
		labelTable2.put( new Integer( 20000 ), new JLabel("20") );
		labelTable2.put( new Integer( 30000 ), new JLabel("30") );
		labelTable2.put( new Integer( 40000 ), new JLabel("40") );
		labelTable2.put( new Integer( 50000 ), new JLabel("50") );
		labelTable2.put( new Integer( 60000 ), new JLabel("60") );

		sliderModel inmodel3 = new sliderModel(0,60000,5000);
		sliderPack  contrRateSlider= new sliderPack(Const.interContractT,"Inter-Contract Delay (sec)",1000,inmodel3,10000,0,true,labelTable2,true,true);

		//   >>  MaxConcurrentContracts
		sliderModel inmodel5 = new sliderModel(0,3,3);
		sliderPack  maxcontractSlider = new sliderPack(Const.maxContracts,"Maximum Concurrent Contracts",1,inmodel5,1,0,false,null,true,true);
		maxcontractSlider.snapTicks(true);


		//   >>  learningRate
		Hashtable labelTable3 = new Hashtable();
		labelTable3.put( new Integer( 0 ), new JLabel("0") );
		labelTable3.put( new Integer( 20 ), new JLabel("0.2") );
		labelTable3.put( new Integer( 40 ), new JLabel("0.4") );
		labelTable3.put( new Integer( 60 ), new JLabel("0.6") );
		labelTable3.put( new Integer( 80 ), new JLabel("0.8") );
		labelTable3.put( new Integer( 100 ), new JLabel("1.0") );

		sliderModel inmodel6 = new sliderModel(0,100,100);
		sliderPack  learningRateSlider= new sliderPack(Const.learningRate,"Learning rate",100,inmodel6,20,10,true,labelTable3,true,true);


		//   >>  scalingFactor
		sliderModel inmodel7 = new sliderModel(0,100,100);
		sliderPack  scalingFacSlider = new sliderPack(Const.scalingFactor,"Scaling Factor",100,inmodel7,20,10,true,labelTable3,true,true);


		/// ######### NO MORE SLIDERS AFTER THIS POINT ###########
		//  ######################################################


		// ****************** map canvas *****************************

		canvasPane = new JPanel();
		canvas.setSize(500, 500);
		canvasPane.setMinimumSize(new Dimension(0,0));
		canvasPane.setPreferredSize(new Dimension(500,500));
		canvasPane.add(canvas, BorderLayout.CENTER);
		canvasPane.setToolTipText("The environment map as we have it at this moment");



		// **************** making the split and setting it up. *****

		mainSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,canvasPane,debugScrollPane);
		mainSplit.setDividerLocation(0.5);
		mainSplit.setBounds(0,0,800,800);
		mainSplit.setOneTouchExpandable(true);
		mainSplit.setContinuousLayout(false);

		slidersPane = new JPanel(); //contains the individual  slider panes.
		slidersPane.setMinimumSize(new Dimension(0,0));

		leftControls = new JPanel();
		rightControls = new JPanel();
		leftControls.setSize(new Dimension(1000,1000));

		leftControls.add(sellSlider);
		leftControls.add(contrRateSlider);
		leftControls.add(thresSlider);
		rightControls.add(maxcontractSlider);
		rightControls.add(learningRateSlider);
		rightControls.add(scalingFacSlider);
		rightControls.setSize(new Dimension(1000,1000));

		slidersPane.add(leftControls,BorderLayout.NORTH);
		slidersPane.add(rightControls,BorderLayout.SOUTH);
		slidersPane.setPreferredSize(new Dimension(900,200));


		// ########## BOTTOM STUFF ##########
		// ##################################


		bottom = new JPanel();
		picture = new JLabel(new ImageIcon("peteNeutral.jpg"),JLabel.CENTER);
		picture.setSize(new Dimension(94,124));
		picture.setBorder(BorderFactory.createCompoundBorder(
								     BorderFactory.createLoweredBevelBorder(),
								     BorderFactory.createEmptyBorder(5,5,5,5)));

		moodframe = new JFrame("Pete's mood");
		Container picContPane = moodframe.getContentPane();
		picContPane.add(picture);
		moodframe.setResizable(false);
		moodframe.setVisible(true);
		moodframe.pack();



		//bottom.add(picture);
		bottom.add(slidersPane);
		bottom.setLayout(new BoxLayout(bottom,BoxLayout.X_AXIS));

		// make the ultra split
		ultraSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT,mainSplit,bottom);
		ultraSplit.setDividerLocation(0.5);
		ultraSplit.setBounds(0,0,800,800);
		ultraSplit.setOneTouchExpandable(true);
		ultraSplit.setContinuousLayout(false);


		// ######## SETTING UP TABBED PANE HERE ########

		daTabs = new JTabbedPane(JTabbedPane.TOP);

		Component AboutTab = null;

		daTabs.addTab("Main View",null,mainSplit,"The Main view which contains the map as well as info about current transactions");
		daTabs.addTab("Settings",null,slidersPane,"The Settings view which contains controls for many of the programs parameters");

		// optional stuff.

		stats= new statPane();
		lblStats mylbls = new lblStats();

		JPanel statmain = new JPanel();
		statmain.add(stats,BorderLayout.NORTH);
		statmain.add(mylbls,BorderLayout.SOUTH);



		daTabs.addTab("Statistics",null,statmain,"The Statistics tab which contains real-time statistics about the system and the opponents");
		daTabs.repaint();
		//daTabs.addTab("About",null,AboutTab,"About this programs and the people behind it");

		// ######## END OF TABBED PANE STUFF ##########



		Container contentPane = getContentPane();
		contentPane.add(daTabs);
		mainSplit.resetToPreferredSizes();

		System.out.println("MapOMatic.MapOMatic(): constructed");
	}


	//
	// output to the scrolling text list
	//
	public void println(String s) {
		debug.setSelectedIndex(0);
		debugModel.insertElementAt(s, 0);
		if (debugModel.getSize() > 1000) {
			debugModel.remove(1000);
		}
	}


	//
	// repaint all because Java is silly
	//
    public void paint(Graphics g) {
		Dimension d = canvasPane.getSize();
		debugScrollPane.repaint();
		canvas.setSize(d.width, d.height);
		//canvas.setLocation(0, 0);
		//canvasPane.repaint();
		//slidersPane.repaint();
		//sliderPane1.repaint();
		//sliderPane2.repaint();
		mainSplit.repaint();
		daTabs.repaint();
    }

	public void updatePic(String filename){

		if (file == filename) { return; }
		else {
			picture.setIcon(new ImageIcon(filename));
			file = filename;
			moodframe.repaint();
		}
	}
}

