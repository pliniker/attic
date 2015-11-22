

#include "FileChooserView.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qfiledialog.h>


FileChooserView::FileChooserView(   nnet::SerializablePtr   object,
                                    QWidget*                parent,
                                    const char*             name,
                                    WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    cstr( object )
{
    if( !name )
    {
        setName( "FileChooserView" );
    }
        
    FileChooserViewLayout = new QHBoxLayout( this, 11, 6, "FileChooserViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    label->setAlignment( int( QLabel::AlignVCenter ) );
    FileChooserViewLayout->addWidget( label );
    
    edit = new QPushButton( cstr->get
                            ().c_str(), this, "edit" );
    QFont edit_font( edit->font() );
    edit_font.setFamily( "Helvetica" );
    edit_font.setPointSize( 10 );
    edit->setFont( edit_font );
    FileChooserViewLayout->addWidget( edit );
    languageChange();
    resize( QSize( 189, 50 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    connect( edit, SIGNAL( clicked() ), this, SLOT( pickFile() ) );
    
    label->setText( cstr->name().c_str() );
    refreshView();
}


FileChooserView::~FileChooserView()
{
    // no need to delete child widgets, Qt does it all for us
}


void FileChooserView::refreshView()
{
    edit->setText( cstr->get().c_str() );
}


void FileChooserView::applyChanges()
{
    cstr->set( edit->text().latin1() );
}


void FileChooserView::pickFile()
{
    QString path = QFileDialog::getOpenFileName(
                            edit->text(),
                            "*",
                            this,
                            "Choose file",
                            "Choose a file" );
                   
    if( path != QString::null )
    {
        edit->setText( path );
    }
}


void FileChooserView::languageChange()
{
    setCaption( tr( "FileChooserView" ) );
    label->setText( tr( "label" ) );
    edit->setText( tr( "text" ) );
}


ObjectView* FileChooserView::Create(    nnet::SerializablePtr   object,
                                        QWidget*                parent,
                                        const char*             name )
{
    return new FileChooserView( object, parent, name );
}

