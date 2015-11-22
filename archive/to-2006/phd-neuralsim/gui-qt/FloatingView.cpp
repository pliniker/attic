

#include "FloatingView.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <cmath>


FloatContainerBase::~FloatContainerBase() {}


FloatSpinBox::FloatSpinBox( double scalefactor,
                            QWidget *parent,
                            const char *name ) :
    QSpinBox( parent, name ),
    factor( scalefactor )
{
}


QString FloatSpinBox::mapValueToText( int value )
{
    if( value == -1 )
    {
        return QString( "Auto" );
    }
    return QString( "%1.%2" ).arg( value / ( int ) factor ).arg( value % ( int ) factor );
}


int FloatSpinBox::mapTextToValue( bool* )
{
    if( text() == "Auto" )
    {
        return -1;
    }
    return ( int ) ( factor * text().toDouble() );
}




FloatingView::FloatingView( FloatContainerBase*     floatcont,
                            nnet::SerializablePtr   object,
                            QWidget*                parent,
                            const char*             name,
                            WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    floating( floatcont )
{

    if( !name )
    {
        setName( "FloatingView" );
    }
        
    // allow for correct viewing of a floating point number inside
    // a qspinbox which only understands integers
    int int_scalefactor;
    frexp( floating->stepping(), &int_scalefactor );
    scalefactor = pow( 10, ( double ) int_scalefactor );
    
    FloatingViewLayout = new QHBoxLayout( this, 11, 6, "FloatingViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    FloatingViewLayout->addWidget( label );
    
    spinBox = new FloatSpinBox( scalefactor, this, "spinBox" );
    QFont spinBox_font( spinBox->font() );
    spinBox_font.setFamily( "Helvetica" );
    spinBox_font.setPointSize( 10 );
    spinBox->setFont( spinBox_font );
    FloatingViewLayout->addWidget( spinBox );
    languageChange();
    resize( QSize( 120, 56 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    label->setText( object->name().c_str() );
    refreshView();
}


FloatingView::~FloatingView()
{
    // no need to delete child widgets, Qt does it all for us
    delete floating;
}


void FloatingView::refreshView()
{
    spinBox->setValue( (int)( floating->get() * scalefactor ) );
}


void FloatingView::applyChanges()
{
    int value = spinBox->value();
    floating->set( (double) value / scalefactor );
}


void FloatingView::languageChange()
{
    setCaption( tr( "FloatingView" ) );
    label->setText( tr( "label" ) );
}



ObjectView* FloatingView::Create( nnet::SerializablePtr object,
                                  QWidget*              parent,
                                  const char*           name )
{
    FloatContainerBase * container = 0;
    
    if( object->type() == "Float" )
    {
        container = new FloatContainer<float>( object );
    }
        
    if( object->type() == "Double" )
    {
        container = new FloatContainer<double>( object );
    }
        
    return new FloatingView( container, object, parent, name );
}

