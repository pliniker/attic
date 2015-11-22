

#include "CStringView.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>


CStringView::CStringView(   nnet::SerializablePtr   object,
                            QWidget*                parent,
                            const char*             name,
                            WFlags                  fl ) :
    ObjectView( object, parent, name, fl ),
    cstr( object )
{
    if( !name )
    {
        setName( "CStringView" );
    }
        
    CStringViewLayout = new QHBoxLayout( this, 11, 6, "CStringViewLayout" );
    
    label = new QLabel( this, "label" );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    label->setAlignment( int( QLabel::AlignVCenter ) );
    CStringViewLayout->addWidget( label );
    
    edit = new QLineEdit( this, "edit" );
    QFont edit_font( edit->font() );
    edit_font.setFamily( "Helvetica" );
    edit_font.setPointSize( 10 );
    edit->setFont( edit_font );
    edit->setFrameShape( QLineEdit::LineEditPanel );
    edit->setFrameShadow( QLineEdit::Sunken );
    CStringViewLayout->addWidget( edit );
    languageChange();
    resize( QSize( 189, 50 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    label->setText( cstr->name().c_str() );
    refreshView();
}


CStringView::~CStringView()
{
    // no need to delete child widgets, Qt does it all for us
}


void CStringView::refreshView()
{
    edit->setText( cstr->get().c_str() );
}


void CStringView::applyChanges()
{
    cstr->set( edit->text().latin1() );
}


void CStringView::languageChange()
{
    setCaption( tr( "CStringView" ) );
    label->setText( tr( "label" ) );
    edit->setText( tr( "text" ) );
}


ObjectView* CStringView::Create(    nnet::SerializablePtr   object,
                                    QWidget*                parent,
                                    const char*             name )
{
    return new CStringView( object, parent, name );
}

