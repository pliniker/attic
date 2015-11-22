

#include "NewObject.h"

#include <qvariant.h>
#include <qiconview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


NewObject::NewObject(   QWidget*    parent,
                        const char* name,
                        bool        modal,
                        WFlags      fl ) :
    QDialog( parent, name, modal, fl )
{

    if( !name )
    {
        setName( "NewObject" );
    }
        
    setSizeGripEnabled( FALSE );
    NewObjectLayout = new QVBoxLayout( this, 11, 6, "NewObjectLayout" );
    
    iconView = new QIconView( this, "iconView" );
    QFont iconView_font( iconView->font() );
    iconView_font.setFamily( "Lucida Grande" );
    iconView_font.setPointSize( 10 );
    iconView->setFont( iconView_font );
    iconView->setFrameShape( QIconView::GroupBoxPanel );
    iconView->setFrameShadow( QIconView::Sunken );
    iconView->setLineWidth( 5 );
    iconView->setMargin( 5 );
    iconView->setVScrollBarMode( QIconView::AlwaysOn );
    iconView->setHScrollBarMode( QIconView::AlwaysOff );
    iconView->setGridX( -1 );
    iconView->setGridY( -1 );
    iconView->setItemTextPos( QIconView::Right );
    iconView->setWordWrapIconText( false );
    iconView->setMaxItemWidth( 640 );
    NewObjectLayout->addWidget( iconView );
    
    layout2 = new QHBoxLayout( 0, 0, 6, "layout2" );
    spacer = new QSpacerItem( 251, 31, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer );
    
    createButton = new QPushButton( this, "createButton" );
    createButton->setDefault( TRUE );
    createButton->setEnabled( false );
    layout2->addWidget( createButton );
    
    cancelButton = new QPushButton( this, "cancelButton" );
    layout2->addWidget( cancelButton );
    NewObjectLayout->addLayout( layout2 );
    languageChange();
    resize( QSize( 312, 378 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    // signals and slots connections
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( createButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( iconView, SIGNAL( selectionChanged( QIconViewItem* ) ),
             this, SLOT( selectionChangedSlot( QIconViewItem* ) ) );
    connect( iconView, SIGNAL( doubleClicked( QIconViewItem* ) ),
             this, SLOT( selectIconSlot( QIconViewItem* ) ) );
             
    currentItem = 0;
}


NewObject::~NewObject()
{
    // no need to delete child widgets, Qt does it all for us
}


void NewObject::clear()
{
    iconView->clear();
    createButton->setEnabled( false );
    currentItem = 0;
}


void NewObject::populateWith( nnet::StringListBase* types )
{
    if( types )
    {
        for( int i = 0; i < types->size(); i++ )
        {
            ( void ) new QIconViewItem(
                            iconView,
                            ( types->get( i ) ).c_str(),
                            QPixmap::fromMimeSource( "neuron-scaled.png" ) );
        }
    }
}


QString NewObject::typeSelected()
{
    if( currentItem )
    {
        return currentItem->text();
    }
    else
    {
        return QString::null;
    }
}


void NewObject::selectionChangedSlot( QIconViewItem* item )
{
    currentItem = item;
    if ( currentItem )
    {
        createButton->setEnabled( true );
    }
}


void NewObject::selectIconSlot( QIconViewItem* item )
{
    currentItem = item;
    if ( currentItem )
    {
        accept();
    }
}


void NewObject::languageChange()
{
    setCaption( tr( "Object Selector" ) );
    iconView->clear();
    //(void) new QIconViewItem( iconView, tr( "Dummy" ), QPixmap::fromMimeSource( "neuron-scaled.png" ) );
    createButton->setText( tr( "Select" ) );
    createButton->setAccel( QKeySequence( QString::null ) );
    cancelButton->setText( tr( "Cancel" ) );
    cancelButton->setAccel( QKeySequence( tr( "Esc" ) ) );
}

