
#include <qlistbox.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include "Console.h"


Console::Console(   QWidget*    parent,
                    const char* name,
                    bool        modal,
                    WFlags      fl ) :
    QDialog( parent, name, modal, fl )
{
    if( !name )
    {
        setName( "Messages" );
    }
        
    consoleLayout = new QVBoxLayout( this, 0, 0, "consoleLayout" );
    
    text = new QListBox( this, "text" );
    consoleLayout->addWidget( text );
    
    resize( QSize( 480, 240 ) );
    
    parentWidget = parent;
    caption = "Brain Builder";
}


Console::~Console() {}


void Console::alert( std::string err )
{
    text->insertItem( err.c_str(), 0 );
    QMessageBox::warning( parentWidget, caption, err.c_str() );
}


void Console::log( std::string err )
{
    text->insertItem( err.c_str(), 0 );
    if ( !isVisible() )
    {
        show();
    }
}


bool Console::ask( std::string err )
{
    text->insertItem( err.c_str(), 0 );
    return ( QMessageBox::question( parentWidget, caption, err.c_str(),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::No | QMessageBox::Escape ) == QMessageBox::Yes );
}


void Console::clear()
{
    text->clear();
}

