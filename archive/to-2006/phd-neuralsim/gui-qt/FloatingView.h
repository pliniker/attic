#ifndef FLOATINGVIEW_H
#define FLOATINGVIEW_H


#include <qspinbox.h>
#include <Floating.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;



/// Because QT does not support templated QWidgets, we have to
/// bodge it.
class FloatContainerBase
{
public:
    virtual ~FloatContainerBase();
    virtual void set( double n ) = 0;
    virtual double get() = 0;
    virtual double min() = 0;
    virtual double max() = 0;
    virtual double stepping() = 0;
};



template <typename T>
class FloatContainer : public FloatContainerBase
{
public:
    FloatContainer( nnet::SerializablePtr ob );
    void set( double n );
    double get();
    double min();
    double max();
    double stepping();
    
private:
    nnet::RefCountPtr< nnet::Floating<T> > object;
};



class FloatSpinBox : public QSpinBox
{
    Q_OBJECT
    
public:
    FloatSpinBox(   double      scalefactor,
                    QWidget*    parent = 0,
                    const char* name = 0 );
                  
    QString mapValueToText( int value );
    int mapTextToValue( bool *ok );
    
    
private:
    double factor;
};



class FloatingView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    FloatingView(   FloatContainerBase*     floatcont,
                    nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0,
                    WFlags                  fl = 0 );
    ~FloatingView();
    
    QLabel*         label;
    FloatSpinBox*   spinBox;
    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
    
protected:
    QHBoxLayout*    FloatingViewLayout;
    
protected slots:
    virtual void    languageChange();
    
private:
    FloatContainerBase* floating;
    double          scalefactor;
    
    /// Class methods
public:
    static ObjectView* Create(  nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};




// Implementation

template <typename T>
FloatContainer<T>::FloatContainer( nnet::SerializablePtr ob ) :
    object( ob )
{
}


template <typename T>
double FloatContainer<T>::get()
{
    return (double)object->get();
}


template <typename T>
void FloatContainer<T>::set( double n )
{
    object->set( (T)n );
}


template <typename T>
double FloatContainer<T>::min()
{
    return (double)object->getMin();
}


template <typename T>
double FloatContainer<T>::max()
{
    return (double)object->getMax();
}


template <typename T>
double FloatContainer<T>::stepping()
{
    return (double)object->getStep();
}



#endif // FLOATINGVIEW_H
