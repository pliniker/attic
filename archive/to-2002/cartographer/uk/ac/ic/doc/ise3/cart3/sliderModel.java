/**
 * sliderModel. Class on which the sliders on the main gui are based.
 * The class is based on the ConverterBoundedRange (java website) which
 * is again based on the DefaultBoundedRangeModel.
 *
 * DK 22/06/01
 */

package uk.ac.ic.doc.ise3.cart3;

import javax.swing.*;
import javax.swing.event.*;

/**
 * Based on the source code for DefaultBoundedRangeModel,
 * this class stores its value as a double, rather than
 * an int.  The minimum value and extent are always 0.
 **/
public class sliderModel implements BoundedRangeModel {
    protected ChangeEvent changeEvent = null;
    protected EventListenerList listenerList = new EventListenerList();

    protected int maximum = 10000;
    protected int minimum = 0;
    protected int extent = 0;
    protected double value = 0.0;
    protected boolean isAdjusting = false;

    public sliderModel(int min, int max, double val) {
		minimum = min;
		maximum = max;
		value   = val;
    }

    public int getMaximum() {
    	return maximum;
    }

    public void setMaximum(int newMaximum) {
        //setRangeProperties(value, extent, minimum, newMaximum, isAdjusting);
    }

    public int getMinimum() {
        return (int)minimum;
    }

    public void setMinimum(int newMinimum) {
        //Do nothing.
    }

    public int getValue() {
        return (int)getDoubleValue();
    }

    public void setValue(int newValue) {
        setDoubleValue((double)newValue);
    }

    public double getDoubleValue() {
        return value;
    }

    public void setDoubleValue(double newValue) {
        setRangeProperties(newValue, extent, minimum, maximum, isAdjusting);
    }

    public int getExtent() {
        return 0;
    }

    public void setExtent(int newExtent) {
        //Do nothing.
    }

    public boolean getValueIsAdjusting() {
        return isAdjusting;
    }

    public void setValueIsAdjusting(boolean b) {
        setRangeProperties(value, extent, minimum, maximum, b);
    }

    public void setRangeProperties(int newValue,
                                   int newExtent,
                                   int newMin,
                                   int newMax,
                                   boolean newAdjusting) {
        System.out.println("In ConverterRangeModel setRangeProperties");
        setRangeProperties((double)newValue,
                           newExtent,
                           newMin,
                           newMax,
                           newAdjusting);
    }

    public void setRangeProperties(double newValue,
                                   int unusedExtent,
                                   int unusedMin,
                                   int newMax,
                                   boolean newAdjusting) {
        if (newMax <= minimum) {
            newMax = minimum + 1;
            System.out.println("sliderModel -> maximum raised by 1 to " + newMax);
        }
        if (Math.round(newValue) > newMax) { //allow some rounding error
            newValue = newMax;
            System.out.println("sliderModel -> value lowered to " + newMax);
        }

        if (Math.round(newValue) < minimum){
			newValue = minimum;
		}

        boolean changeOccurred = false;
        if (newValue != value) {
            System.out.println("value set to " + newValue);
            value = newValue;
            changeOccurred = true;
        }
        if (newMax != maximum) {
            System.out.println("maximum set to " + newMax);
            maximum = newMax;
            changeOccurred = true;
        }
        if (newAdjusting != isAdjusting) {
            maximum = newMax;
            isAdjusting = newAdjusting;
            changeOccurred = true;
        }

        if (changeOccurred) {
            fireStateChanged();
        }
    }

    /*
     * The rest of this is event handling code copied from
     * DefaultBoundedRangeModel.
     */
    public void addChangeListener(ChangeListener l) {
        listenerList.add(ChangeListener.class, l);
    }

    public void removeChangeListener(ChangeListener l) {
        listenerList.remove(ChangeListener.class, l);
    }

    protected void fireStateChanged() {
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length - 2; i >= 0; i -=2 ) {
            if (listeners[i] == ChangeListener.class) {
                if (changeEvent == null) {
                    changeEvent = new ChangeEvent(this);
                }
                ((ChangeListener)listeners[i+1]).stateChanged(changeEvent);
            }
        }
    }
}
