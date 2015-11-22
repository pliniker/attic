

#include "ObjectMakerView.h"


#include <qvariant.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlineedit.h>
#include <qtoolbutton.h>


NameObject::NameObject(     QWidget*    parent,
                            const char* name,
                            WFlags      fl ) :
    QDialog( parent, name, TRUE, fl )
{
  nameObjectLayout = new QHBoxLayout( this, 11, 6, "nameObjectLayout" );

  edit = new QLineEdit( this, "edit" );
  edit->setText( "YourObjectName" );
  edit->selectAll();

  nameObjectLayout->addWidget( edit );

  connect( edit, SIGNAL( returnPressed() ), this, SLOT( returnPressed() ) );
}


std::string NameObject::name()
{
  return objectName;
}


void NameObject::returnPressed()
{
    objectName = edit->text().latin1();

    if( objectName != "" )
    {
        edit->selectAll();
        accept();
    }
}




ObjectMakerView::ObjectMakerView(   nnet::SerializablePtr   object,
                                    QWidget*                parent,
                                    const char*             name,
                                    WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    optionList( object )
{

    if( !name )
    {
        setName( "ObjectMakerView" );
    }

    ObjectMakerViewLayout = new QVBoxLayout( this, 11, 6, "ObjectMakerViewLayout" );
    ButtonLayout = new QHBoxLayout( this, 1, 1, "ButtonLayout" );

    ObjectMakerViewLayout->addLayout( ButtonLayout );

    addButton = new QToolButton( this, "addButton" );
    addButton->setText( "New object" );
    ButtonLayout->addWidget( addButton );

    delButton = new QToolButton( this, "delButton" );
    delButton->setText( "Delete" );
    delButton->setEnabled( false );
    ButtonLayout->addWidget( delButton );

    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    ObjectMakerViewLayout->addWidget( label );

    objects = new QListBox( this, "selected" );
    ObjectMakerViewLayout->addWidget( objects );
    languageChange();
    resize( QSize( 243, 118 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );

    objects->setSelectionMode( QListBox::Single );

    nameObject = new NameObject( this, "NameObject" );

    // signals and slots connections
    connect( objects, SIGNAL( clicked( QListBoxItem* ) ),
            this, SLOT( newSelection( QListBoxItem* ) ) );

    connect( addButton, SIGNAL( clicked() ),
            this, SLOT( addObject() ) );
    connect( delButton, SIGNAL( clicked() ),
            this, SLOT( delObject() ) );

    label->setText( optionList->name().c_str() );
    refreshView();
}


ObjectMakerView::~ObjectMakerView()
{
    // no need to delete child widgets, Qt does it all for us
}


void ObjectMakerView::refreshView()
{
    objects->clear();

    for( int i = 0; i < optionList->size(); i++ )
    {
        objects->insertItem( optionList->get( i ).c_str(), i );
    }
}


void ObjectMakerView::applyChanges() {}


void ObjectMakerView::newSelection( QListBoxItem* item )
{
    if( item )
    {
        delButton->setEnabled( true );
        currentItem = item->text().latin1();
    }
    else
    {
        delButton->setEnabled( false );
        currentItem = "";
    }
}


void ObjectMakerView::addObject()
{
    nameObject->exec();
    optionList->add( nameObject->name() );

    refreshView();
}


void ObjectMakerView::delObject()
{
    if( currentItem != "" )
    {
        optionList->remove( currentItem );
    }

    refreshView();
}


void ObjectMakerView::languageChange()
{
    setCaption( tr( "ObjectMakerView" ) );
    label->setText( tr( "label" ) );
}


ObjectView* ObjectMakerView::Create(    nnet::SerializablePtr   object,
                                        QWidget*                parent,
                                        const char*             name )
{
    return new ObjectMakerView( object, parent, name );
}


