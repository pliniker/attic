#ifndef INTEGERVIEW_H
#define INTEGERVIEW_H


#include <Integer.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;


/// Because QT does not support templated QWidgets, we have to
/// bodge it.
struct IntContainerBase 
{
    virtual ~IntContainerBase();
    virtual void set( int n ) = 0;
    virtual int get() = 0;
    virtual int min() = 0;
    virtual int max() = 0;
    virtual int stepping() = 0;
};


/// IntegerProperty template is read and written by this proxy class.
/// Based on IntContainerBase.
template <typename T>
class IntContainer : public IntContainerBase 
{
public:
    IntContainer( nnet::SerializablePtr ob );
    void set( int n );
    int get();
    int min();
    int max();
    int stepping();
    
private:
    nnet::RefCountPtr< nnet::Integer<T> > object;
};




class IntegerView : public ObjectView {
    Q_OBJECT
    
    /// Object methods
public:
    IntegerView(    IntContainerBase*       intcont, 
                    nnet::SerializablePtr   object,
                    QWidget*                parent = 0, 
                    const char*             name = 0, 
                    WFlags                  fl = 0 );
    ~IntegerView();
    
    QLabel*         label;
    QSpinBox*       spinBox;
    
    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
    
    
protected:
    QHBoxLayout*    IntegerViewLayout;
    
protected slots:
    virtual void    languageChange();
    
private:
    IntContainerBase* integer;
    
    
    /// Class methods
public:
    static ObjectView* Create( nnet::SerializablePtr    object,
                               QWidget*                 parent = 0,
                               const char*              name = 0 );
};




// Implementation

template <typename T>
IntContainer<T>::IntContainer( nnet::SerializablePtr ob ) :
    object( ob )
{
}


template <typename T>
int IntContainer<T>::get()
{
    return (int)object->get();
}


template <typename T>
void IntContainer<T>::set( int n ) 
{
    object->set( (T)n );
}


template <typename T>
int IntContainer<T>::min() 
{
    return (int)object->getMin();
}


template <typename T>
int IntContainer<T>::max() 
{
    return (int)object->getMax();
}


template <typename T>
int IntContainer<T>::stepping() 
{
    return (int)object->getStep();
}



#endif // INTEGERVIEW_H
