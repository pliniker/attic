
/*
 * PL and dk. updated by dk.
 */


package uk.ac.ic.doc.ise3.cart2;


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


    public MapOMatic(MapGrid c) {
        super("Map-O-Matic");

		canvas = c;
        debugModel = new DefaultListModel();
        debugModel.addElement("+++++++++++++++++++++++++++++++++++++++");
        debugModel.addElement(" This program was brought to you by...");
        debugModel.addElement("         Dimosthenis Kaponis");
        debugModel.addElement("            Peter Liniker");
		debugModel.addElement("             Anthony Ng");
		debugModel.addElement("+++++++++++++++++++++++++++++++++++++++");


        //Create the debug and put it in a scroll pane
        debug = new JList(debugModel);
        debug.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        debug.setSelectedIndex(0);
		debugScrollPane = new JScrollPane(debug);
		debugScrollPane.setPreferredSize(new Dimension(300, 500));
		debugScrollPane.setMinimumSize(new Dimension(300,200));
		

		// ########## SLIDER STUFF ##########
		// ##################################
		
		
	//accuracyThreshold
	sliderModel inmodel1 = new sliderModel(0,1000,800);
	sliderPack  accslider  = new sliderPack(0,"Accuracy Threshold",1,inmodel1,200,100,false,null,true,true);

	//robotTimePerBlock
	Hashtable labelTable = new Hashtable();
	labelTable.put( new Integer( 0 ), new JLabel("0") );
	//labelTable.put( new Integer( 2000 ), new JLabel("2") );
	//labelTable.put( new Integer( 4000 ), new JLabel("4") );
	//labelTable.put( new Integer( 6000 ), new JLabel("6") );
	//labelTable.put( new Integer( 8000 ), new JLabel("8") );
	labelTable.put( new Integer( 10000 ), new JLabel("10") );
	//labelTable.put( new Integer( 12000 ), new JLabel("12") );
	//labelTable.put( new Integer( 14000 ), new JLabel("14") );
	//labelTable.put( new Integer( 16000 ), new JLabel("16") );
	//labelTable.put( new Integer( 18000 ), new JLabel("18") );
	labelTable.put( new Integer( 20000 ), new JLabel("20") );
	sliderModel inmodel2 = new sliderModel(0,20000,8000);
	sliderPack  roboSlider  = new sliderPack(1,"Est. Robot Time Per Block (sec)",1000,inmodel2,10000,0,true,labelTable,true,true);

	//ContractSpewOutRate
	Hashtable labelTable2 = new Hashtable();
	labelTable2.put( new Integer( 0 ), new JLabel("0") );
	labelTable2.put( new Integer( 10000 ), new JLabel("10") );
	labelTable2.put( new Integer( 20000 ), new JLabel("20") );
	labelTable2.put( new Integer( 30000 ), new JLabel("30") );
	labelTable2.put( new Integer( 40000 ), new JLabel("40") );
	labelTable2.put( new Integer( 50000 ), new JLabel("50") );
	labelTable2.put( new Integer( 60000 ), new JLabel("60") );

	sliderModel inmodel3 = new sliderModel(0,60000,10000);
	sliderPack  contrRateSlider= new sliderPack(2,"Inter-Contract Delay (sec)",1000,inmodel3,10000,0,true,labelTable2,true,true);



	//spendingLimitPerBlock
	sliderModel inmodel4 = new sliderModel(0,20,8);
	sliderPack  spendingLimSlider= new sliderPack(3,"Spending Limit Per Block (£)",1,inmodel4,2,1,false,null,true,true);



	//MaxConcurrentContracts
	sliderModel inmodel5 = new sliderModel(0,3,3);
	sliderPack  maxcontractSlider = new sliderPack(4,"Maximum Concurrent Contracts",1,inmodel5,1,0,false,null,true,true);
	maxcontractSlider.snapTicks(true);


	//learningRate
	Hashtable labelTable3 = new Hashtable();
	labelTable3.put( new Integer( 0 ), new JLabel("0") );
	//labelTable3.put( new Integer( 10 ), new JLabel("0.1") );
	labelTable3.put( new Integer( 20 ), new JLabel("0.2") );
	//labelTable3.put( new Integer( 30 ), new JLabel("0.3") );
	labelTable3.put( new Integer( 40 ), new JLabel("0.4") );
	//labelTable3.put( new Integer( 50 ), new JLabel("0.5") );
	labelTable3.put( new Integer( 60 ), new JLabel("0.6") );
	//labelTable3.put( new Integer( 70 ), new JLabel("0.7") );
	labelTable3.put( new Integer( 80 ), new JLabel("0.8") );
	//labelTable3.put( new Integer( 90 ), new JLabel("0.9") );
	labelTable3.put( new Integer( 100 ), new JLabel("1.0") );


	sliderModel inmodel6 = new sliderModel(0,100,100);
	sliderPack  learningRateSlider= new sliderPack(5,"Learning rate",100,inmodel6,20,10,true,labelTable3,true,true);

	//scalingFactor
	sliderModel inmodel7 = new sliderModel(0,100,100);
	sliderPack  scalingFacSlider = new sliderPack(6,"Scaling Factor",100,inmodel7,20,10,true,labelTable3,true,true);



	// map canvas
	canvasPane = new JPanel();
	//canvasPane.setBorder(BorderFactory.createLineBorder(Color.black));
	canvas.setSize(500, 500);
	canvasPane.setMinimumSize(new Dimension(0,0));
	canvasPane.setPreferredSize(new Dimension(500,500));
	canvasPane.add(canvas, BorderLayout.CENTER);
	canvasPane.setToolTipText("The environment map as we have it at this moment");

	//canvasPane.setBackground(Color.white);

	// making the split and setting it up.

	mainSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,canvasPane,debugScrollPane);
	mainSplit.setDividerLocation(0.5);
	mainSplit.setBounds(0,0,800,800);
	mainSplit.setOneTouchExpandable(true);
	mainSplit.setContinuousLayout(false);

	slidersPane = new JPanel(); //contains the individual  slider panes.
	slidersPane.setMinimumSize(new Dimension(0,0));

	leftControls = new JPanel();
	rightControls = new JPanel();
	//leftControls.setAlignmentX(Component.CENTER_ALIGNMENT);
	leftControls.setSize(new Dimension(1000,1000));

	leftControls.add(spendingLimSlider);
	leftControls.add(accslider);
	leftControls.add(roboSlider);
	leftControls.add(contrRateSlider);
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
	JLabel picture = new JLabel(new ImageIcon("peteNeutral.jpg"),JLabel.CENTER);
	picture.setSize(new Dimension(94,124));
	picture.setBorder(BorderFactory.createCompoundBorder(
							     BorderFactory.createLoweredBevelBorder(),
							     BorderFactory.createEmptyBorder(5,5,5,5)));
	//controls.add(setButton,BorderLayout.WEST);


	bottom.add(picture);
	bottom.add(slidersPane);
	bottom.setLayout(new BoxLayout(bottom,BoxLayout.X_AXIS));



	// make the ultra split
	ultraSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT,mainSplit,bottom);
	ultraSplit.setDividerLocation(0.5);
	ultraSplit.setBounds(0,0,800,800);
	ultraSplit.setOneTouchExpandable(true);
	ultraSplit.setContinuousLayout(false);
	//ultraSplit.setSize(new Dimension(1000,1000));

	///////////// END NEW CODE ////////////////////



		Container contentPane = getContentPane();
		//contentPane.add(debugScrollPane, BorderLayout.EAST);
		//contentPane.add(canvasPane, BorderLayout.CENTER);
		//contentPane.add(mainSplit, BorderLayout.CENTER);
		//contentPane.add(slidersPane,BorderLayout.SOUTH);
		contentPane.add(ultraSplit);
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
		canvas.setSize(d.width, d.height);
		canvas.setLocation(0, 0);
		canvasPane.repaint();
		debugScrollPane.repaint();
		slidersPane.repaint();
		//sliderPane1.repaint();
		//sliderPane2.repaint();
		mainSplit.repaint();
		ultraSplit.repaint();



    }

    /** Listens to the sliders. */
    class SliderListener1 implements ChangeListener {

		public void stateChanged(ChangeEvent e) {
			if (!((JSlider)e.getSource()).getValueIsAdjusting()){
				println("Learning Rate modified!");
			}
	    }
    }

	class SliderListener2 implements ChangeListener {

		public void stateChanged(ChangeEvent e) {
			if (!((JSlider)e.getSource()).getValueIsAdjusting()){
				println("Accuracy threshold modified!");
			}
		}
    }
}
