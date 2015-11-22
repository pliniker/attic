

#include "BooleanView.h"
#include "ObjectBrowser.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


BooleanView::BooleanView(   nnet::SerializablePtr   object,
                            QWidget*                parent,
                            const char*             name,
                            WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    boolean( object )
{
    if( !name )
    {
        setName( "BooleanView" );
    }
    
    BooleanViewLayout = new QHBoxLayout( this, 11, 6, "BooleanViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    BooleanViewLayout->addWidget( label );
    
    check = new QCheckBox( this, "check" );
    BooleanViewLayout->addWidget( check );
    languageChange();
    resize( QSize( 141, 48 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    label->setText( boolean->name().c_str() );
    refreshView();
}


BooleanView::~BooleanView()
{
    // no need to delete child widgets, Qt does it all for us
}


void BooleanView::refreshView()
{
    check->setChecked( boolean->get() );
}


void BooleanView::applyChanges()
{
    boolean->set( check->isChecked() );
}


void BooleanView::languageChange()
{
    setCaption( tr( "BooleanView" ) );
    label->setText( tr( "label" ) );
    check->setText( tr( "True/False" ) );
}


ObjectView* BooleanView::Create( nnet::SerializablePtr  object,
                                 QWidget*               parent,
                                 const char*            name )
{
    return new BooleanView( object, parent, name );
}

