

#include "IntegerView.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


using namespace nnet;


IntContainerBase::~IntContainerBase() {}


IntegerView::IntegerView(   IntContainerBase*   intcont, 
                            SerializablePtr     object,
                            QWidget*            parent, 
                            const char*         name, 
                            WFlags              fl ) :
ObjectView( object, parent, name, fl ),
integer( intcont ) 
{
    if( !name )
    {
        setName( "IntegerView" );
    }
        
    IntegerViewLayout = new QHBoxLayout( this, 11, 6, "IntegerViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    label->setAlignment( int( QLabel::AlignVCenter ) );
    IntegerViewLayout->addWidget( label );
    
    spinBox = new QSpinBox( this, "spinBox" );
    QFont spinBox_font( spinBox->font() );
    spinBox_font.setFamily( "Helvetica" );
    spinBox_font.setPointSize( 10 );
    spinBox->setFont( spinBox_font );
    IntegerViewLayout->addWidget( spinBox );
    languageChange();
    resize( QSize( 120, 56 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    label->setText( object->name().c_str() );
    refreshView();
}


IntegerView::~IntegerView()
{
    // no need to delete child widgets, Qt does it all for us
    delete integer;
}


void IntegerView::refreshView() 
{
    spinBox->setValue( integer->get() );
    spinBox->setMinValue( integer->min() );
    spinBox->setMaxValue( integer->max() );
    spinBox->setLineStep( integer->stepping() );
}


void IntegerView::applyChanges() 
{
    integer->set( spinBox->value() );
}


void IntegerView::languageChange() 
{
    setCaption( tr( "IntegerView" ) );
    label->setText( tr( "label" ) );
}


ObjectView* IntegerView::Create( SerializablePtr    object,
                                 QWidget*           parent,
                                 const char*        name ) 
{
    IntContainerBase * container = 0;
    
    if( object->type() == "Char" )
    {
        container = new IntContainer<char>( object );
    }
        
    if( object->type() == "Uchar" )
    {
        container = new IntContainer<unsigned char>( object );
    }
        
    if( object->type() == "Int" )
    {
        container = new IntContainer<int>( object );
    }
        
    if( object->type() == "Uint" )
    {
        container = new IntContainer<unsigned int>( object );
    }
        
    return new IntegerView( container, object, parent, name );
}

