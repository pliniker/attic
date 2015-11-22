

#include "ObjectBrowser.h"

#include <qvariant.h>
#include <qsplitter.h>
#include <qheader.h>
#include <qlistview.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include <ErrorInterface.h>

#include "StringList.h"
#include "ObjectView.h"
#include "BooleanView.h"
#include "CStringView.h"
#include "IntegerView.h"
#include "FloatingView.h"
#include "OptionListView.h"
#include "ImageView.h"
#include "Vector3dView.h"
#include "FileChooserView.h"
#include "ObjectMakerView.h"


ObjectBrowser::ObjectBrowser(   QWidget*    parent,
                                const char* name,
                                bool        modal,
                                WFlags      fl ) :
    QDialog( parent, name, modal, fl )
{

    if( !name )
    {
        setName( "ObjectBrowser" );
    }
        
    setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 3, ( QSizePolicy::SizeType ) 3, 0, 0, sizePolicy().hasHeightForWidth() ) );
    
    setSizeGripEnabled( FALSE );
    ObjectBrowserLayout = new QVBoxLayout( this, 11, 6, "ObjectBrowserLayout" );
    
    splitter = new QSplitter( this, "splitter" );
    splitter->setCursor( QCursor( 0 ) );
    splitter->setFrameShape( QSplitter::NoFrame );
    splitter->setFrameShadow( QSplitter::Plain );
    splitter->setOrientation( QSplitter::Horizontal );
    splitter->setOpaqueResize( TRUE );
    splitter->setHandleWidth( 10 );
    
    mapList = new QListView( splitter, "mapList" );
    mapList->addColumn( tr( "Maps" ) );
    mapList->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 5, ( QSizePolicy::SizeType ) 5, 1, 0, mapList->sizePolicy().hasHeightForWidth() ) );
    mapList->setMinimumSize( QSize( 128, 0 ) );
    mapList->setFrameShadow( QListView::Sunken );
    mapList->setRootIsDecorated( TRUE );
    
    objectView = new QFrame( splitter, "objectView" );
    objectView->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 7, ( QSizePolicy::SizeType ) 7, 2, 0, objectView->sizePolicy().hasHeightForWidth() ) );
    objectView->setMinimumSize( QSize( 128, 64 ) );
    objectView->setFrameShape( QFrame::GroupBoxPanel );
    objectView->setFrameShadow( QFrame::Raised );
    objectView->setMargin( 5 );
    objectView->setMidLineWidth( 1 );
    ObjectBrowserLayout->addWidget( splitter );
    
    objectLayout = new QVBoxLayout( objectView, 3, 3, "objectLayout" );
    
    bottomLayout = new QHBoxLayout( 0, 0, 6, "bottomLayout" );
    spacing = new QSpacerItem( 144, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    bottomLayout->addItem( spacing );
    
    refreshButton = new QPushButton( this, "refreshButton" );
    bottomLayout->addWidget( refreshButton );
    
    applyButton = new QPushButton( this, "applyButton" );
    bottomLayout->addWidget( applyButton );
    
    closeButton = new QPushButton( this, "closeButton" );
    closeButton->setAutoDefault( FALSE );
    closeButton->setDefault( TRUE );
    bottomLayout->addWidget( closeButton );
    ObjectBrowserLayout->addLayout( bottomLayout );
    languageChange();
    resize( QSize( 512, 512 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    // signals and slots connections
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( applyButton, SIGNAL( clicked() ), this, SLOT( applyChanges() ) );
    connect( refreshButton, SIGNAL( clicked() ), this, SLOT( refreshView() ) );
    connect( mapList, SIGNAL( selectionChanged( QListViewItem* ) ),
             this, SLOT( selectMap( QListViewItem* ) ) );
             
    // base type handlers
    Add( "Boolean", BooleanView::Create );
    Add( "CString", CStringView::Create );
    Add( "Char", IntegerView::Create );
    Add( "Uchar", IntegerView::Create );
    Add( "Int", IntegerView::Create );
    Add( "Uint", IntegerView::Create );
    Add( "Float", FloatingView::Create );
    Add( "Double", FloatingView::Create );
    Add( "Image", ImageView::Create );
    Add( "OptionList", OptionListView::Create );
    Add( "ObjectList", OptionListView::Create );
    //Add("Int3d", Vector3dView::Create);
    //Add("Float3d", Vector3dView::Create);
    //Add("Double3d", Vector3dView::Create);
    Add( "FileName", FileChooserView::Create );
    Add( "ObjectMaker", ObjectMakerView::Create );
}


ObjectBrowser::~ObjectBrowser()
{
    // no need to delete child widgets, Qt does it all for us
}


void ObjectBrowser::display( nnet::MapPtr root )
{
    currentRoot = root;
    
    mapList->clear();
    
    QListViewItem* item = new QListViewItem( mapList, root->name().c_str() );
    display( root, item );
    item->setOpen( true );
    
    selectMap( item );
}


void ObjectBrowser::display( nnet::MapPtr root, QListViewItem* item )
{
    nnet::StringListBase* base_types = nnet::string::get_list( "base_types" );
    
    for( nnet::Map::iterator i = root->begin(); i != root->end(); i++ )
    {
        nnet::SerializablePtr object( root->get( i ) );
        // if not a base type and not private, display it
        if( !base_types->exists( object->type() ) && !object->isPrivate() )
        {
            QListViewItem* subitem = new QListViewItem( item, object->name().c_str() );
            subitem->setOpen( true );
            display( object, subitem );
        }
    }
}


void ObjectBrowser::selectMap( QListViewItem* item )
{
    typedef std::list<QListViewItem*> ItemList;
    typedef ItemList::iterator ItemListIt;
    ItemList items;
    
    // clear the frame
    for( ViewListIt i = viewList.begin(); i != viewList.end(); i++ )
    {
        ObjectView* view = *i;
        objectLayout->remove( view );
        delete view;
    }
    viewList.clear();
    
    // rewind back to top level
    QListViewItem* current_item = item;
    while( current_item->parent() )
    {
        items.push_front( current_item );
        current_item = current_item->parent();
    }
    
    // roll forward through Maps to selected map
    nnet::MapPtr current_map = currentRoot;
    for( ItemListIt i = items.begin(); i != items.end(); i++ ) {
        current_item = *i;
        current_map = current_map->get( current_item->text( 0 ).latin1() );
    }
    
    // display the current_map
    for( nnet::Map::iterator i = current_map->begin(); i != current_map->end(); i++ )
    {
        nnet::SerializablePtr object( current_map->get( i ) );
                                
        if( !object->isPrivate() )
        {
            ObjectView * view = Get( object, objectView, object->name().c_str() );
            if( view )
            {
                if( object->isReadonly() )
                {
                    view->setEnabled( false );
                }
                else
                {
                    view->setEnabled( true );
                }
                objectLayout->addWidget( view );
                viewList.push_back( view );
                view->show();
            }
        }
    }
}


void ObjectBrowser::applyChanges()
{
    for( ViewListIt i = viewList.begin(); i != viewList.end(); i++ )
    {
        ObjectView* viewer = *i;
        if( viewer->isEnabled() )
        {
            viewer->applyChanges();
        }
    }
    
    refreshView();
}


void ObjectBrowser::refreshView()
{
    for( ViewListIt i = viewList.begin(); i != viewList.end(); i++ )
    {
        ObjectView* viewer = *i;
        viewer->refreshView();
    }
    
    mapList->clear();
    
    QListViewItem* item = new QListViewItem( mapList, currentRoot->name().c_str() );
    display( currentRoot, item );
    item->setOpen( true );
}


void ObjectBrowser::languageChange()
{
    setCaption( tr( "Object Browser" ) );
    mapList->header() ->setLabel( 0, tr( "Maps" ) );
    applyButton->setText( tr( "Apply Changes" ) );
    closeButton->setText( tr( "&Close" ) );
    closeButton->setAccel( QKeySequence( tr( "Esc, Ctrl+B" ) ) );
    refreshButton->setText( tr( "&Refresh" ) );
    refreshButton->setAccel( QKeySequence( tr( "F5" ) ) );
}


void ObjectBrowser::Add( std::string type, viewer ptr )
{
    viewMap[ type ] = ptr;
}


void ObjectBrowser::Remove( std::string type )
{
    viewMap.erase( type );
}


ObjectView* ObjectBrowser::Get(     nnet::SerializablePtr   object,
                                    QWidget*                parent,
                                    const char*             name )
{
    viewer f = viewMap[ object->type() ];
    if( f )
    {
        return f( object, parent, name );
    }
    else
    {
        return 0;
    }
}


ObjectBrowser::ViewMap ObjectBrowser::viewMap;

