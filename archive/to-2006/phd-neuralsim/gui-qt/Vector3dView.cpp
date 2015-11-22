

#include "Vector3dView.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


Vector3dView::Vector3dView( nnet::SerializablePtr   object,
                            QWidget*                parent,
                            const char*             name,
                            WFlags                  fl ) :
    ObjectView( object, parent, name, fl )
{
    if( !name )
    {
        setName( "Vector3dView" );
    }
        
    Vector3dViewLayout = new QGridLayout( this, 1, 1, 1, 6, "Vector3dViewLayout" );
    
    frame21 = new QFrame( this, "frame21" );
    frame21->setFrameShape( QFrame::GroupBoxPanel );
    frame21->setFrameShadow( QFrame::Sunken );
    frame21Layout = new QGridLayout( frame21, 1, 1, 11, 6, "frame21Layout" );
    
    y = new QSpinBox( frame21, "y" );
    QFont y_font( y->font() );
    y_font.setFamily( "Helvetica" );
    y_font.setPointSize( 10 );
    y->setFont( y_font );
    
    frame21Layout->addWidget( y, 2, 1 );
    
    xLabel = new QLabel( frame21, "xLabel" );
    QFont xLabel_font( xLabel->font() );
    xLabel_font.setFamily( "Helvetica" );
    xLabel->setFont( xLabel_font );
    
    frame21Layout->addWidget( xLabel, 1, 0 );
    
    label = new QLabel( frame21, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    
    frame21Layout->addMultiCellWidget( label, 0, 0, 0, 1 );
    
    yLabel = new QLabel( frame21, "yLabel" );
    QFont yLabel_font( yLabel->font() );
    yLabel_font.setFamily( "Helvetica" );
    yLabel->setFont( yLabel_font );
    
    frame21Layout->addWidget( yLabel, 2, 0 );
    
    z = new QSpinBox( frame21, "z" );
    QFont z_font( z->font() );
    z_font.setFamily( "Helvetica" );
    z_font.setPointSize( 10 );
    z->setFont( z_font );
    
    frame21Layout->addWidget( z, 3, 1 );
    
    zLabel = new QLabel( frame21, "zLabel" );
    QFont zLabel_font( zLabel->font() );
    zLabel_font.setFamily( "Helvetica" );
    zLabel->setFont( zLabel_font );
    
    frame21Layout->addWidget( zLabel, 3, 0 );
    
    x = new QSpinBox( frame21, "x" );
    QFont x_font( x->font() );
    x_font.setFamily( "Helvetica" );
    x_font.setPointSize( 10 );
    x->setFont( x_font );
    
    frame21Layout->addWidget( x, 1, 1 );
    
    Vector3dViewLayout->addWidget( frame21, 0, 0 );
    languageChange();
    resize( QSize( 120, 141 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
}


Vector3dView::~Vector3dView()
{
    // no need to delete child widgets, Qt does it all for us
}


void Vector3dView::refreshView() {}


void Vector3dView::applyChanges() {}


void Vector3dView::languageChange()
{
    setCaption( tr( "Vector3dView" ) );
    xLabel->setText( tr( "X" ) );
    label->setText( tr( "label" ) );
    yLabel->setText( tr( "Y" ) );
    zLabel->setText( tr( "Z" ) );
}


ObjectView* Vector3dView::Create( nnet::SerializablePtr, QWidget*, const char* )
{
    return 0;
}

