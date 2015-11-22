

#include "OptionListView.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


OptionListView::OptionListView( nnet::SerializablePtr   object,
                                QWidget*                parent,
                                const char*             name,
                                WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    optionList( object )
{
    if( !name )
    {
        setName( "OptionListView" );
    }
        
    OptionListViewLayout = new QVBoxLayout( this, 11, 6, "OptionListViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    OptionListViewLayout->addWidget( label );
    
    selected = new QListBox( this, "selected" );
    OptionListViewLayout->addWidget( selected );
    languageChange();
    resize( QSize( 243, 118 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    // if ( optionList->max() == 0 ) {
    //  selected->setSelectionMode( QListBox::NoSelection );
    // } else if ( optionList->max() == 1 ) {
    //  selected->setSelectionMode( QListBox::Single );
    // } else {
    selected->setSelectionMode( QListBox::Multi );
    // }
    
    // signals and slots connections
    connect( selected, SIGNAL( rightButtonClicked( QListBoxItem*, const QPoint& ) ), this, SLOT( rightClickedSlot() ) );
    connect( selected, SIGNAL( clicked( QListBoxItem* ) ),
             this, SLOT( newSelection( QListBoxItem* ) ) );
             
    label->setText( optionList->name().c_str() );
    refreshView();
}


OptionListView::~OptionListView()
{
    // no need to delete child widgets, Qt does it all for us
}


void OptionListView::rightClickedSlot() {}


void OptionListView::refreshView()
{
    selected->clear();
    
    nnet::StringListBase* items = optionList->choices();
    for( int i = 0; i < items->size(); i++ )
    {
        selected->insertItem( items->get( i ).c_str(), i );
    }
    
    for( int i = 0; i < optionList->size(); i++ )
    {
        QListBoxItem* item = selected->findItem( optionList->get( i ), Qt::CaseSensitive );
        if( item )
        {
            selected->setSelected( item, true );
        }
    }
}


void OptionListView::applyChanges() {}


void OptionListView::newSelection( QListBoxItem* item )
{
    if( item )
    {
        if( selected->isSelected( item ) )
        {
            selected->setSelected( item, optionList->add( item->text().latin1() ) );
        }
        else
        {
            selected->setSelected( item, optionList->remove( item->text().latin1() ) );
        }
    }
    
    refreshView();
}


void OptionListView::languageChange()
{
    setCaption( tr( "OptionListView" ) );
    label->setText( tr( "label" ) );
}


ObjectView* OptionListView::Create( nnet::SerializablePtr   object,
                                    QWidget*                parent,
                                    const char*             name )
{
    return new OptionListView( object, parent, name );
}

