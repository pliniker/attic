

#include "ImageView.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfiledialog.h>
#include <qmessagebox.h>


using namespace nnet;


ImageView::ImageView(   SerializablePtr object, 
                        QWidget*        parent,
                        const char*     name, 
                        WFlags          fl ) :
ObjectView( object, parent, name, fl ),
image( object ) 
{
    if( !name )
    {
        setName( "ImageView" );
    }
        
    ImageViewLayout = new QGridLayout( this, 1, 1, 1, 6, "ImageViewLayout" );
    
    frame = new QFrame( this, "frame" );
    frame->setFrameShape( QFrame::GroupBoxPanel );
    frame->setFrameShadow( QFrame::Sunken );
    frameLayout = new QGridLayout( frame, 1, 1, 11, 6, "frameLayout" );
    
    enc = new QComboBox( frame, "enc" );
    QFont enc_font( enc->font() );
    enc_font.setFamily( "Helvetica" );
    enc_font.setPointSize( 10 );
    enc->setFont( enc_font );
    
    frameLayout->addWidget( enc, 1, 1 );
    
    encLabel = new QLabel( frame, "encLabel" );
    encLabel->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 1, ( QSizePolicy::SizeType ) 1, 0, 0, encLabel->sizePolicy().hasHeightForWidth() ) );
    QFont encLabel_font( encLabel->font() );
    encLabel_font.setFamily( "Helvetica" );
    encLabel->setFont( encLabel_font );
    encLabel->setAlignment( int( QLabel::AlignVCenter ) );
    
    frameLayout->addWidget( encLabel, 1, 0 );
    
    xres = new QSpinBox( frame, "xres" );
    QFont xres_font( xres->font() );
    xres_font.setFamily( "Helvetica" );
    xres_font.setPointSize( 10 );
    xres->setFont( xres_font );
    xres->setMaxValue( 1024 );
    xres->setMinValue( 1 );
    
    frameLayout->addWidget( xres, 2, 1 );
    
    xresLabel = new QLabel( frame, "xresLabel" );
    xresLabel->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 1, ( QSizePolicy::SizeType ) 1, 0, 0, xresLabel->sizePolicy().hasHeightForWidth() ) );
    QFont xresLabel_font( xresLabel->font() );
    xresLabel_font.setFamily( "Helvetica" );
    xresLabel->setFont( xresLabel_font );
    xresLabel->setAlignment( int( QLabel::AlignVCenter ) );
    
    frameLayout->addWidget( xresLabel, 2, 0 );
    
    yres = new QSpinBox( frame, "yres" );
    QFont yres_font( yres->font() );
    yres_font.setFamily( "Helvetica" );
    yres_font.setPointSize( 10 );
    yres->setFont( yres_font );
    yres->setMaxValue( 1024 );
    yres->setMinValue( 1 );
    yres->setValue( 1 );
    
    frameLayout->addWidget( yres, 3, 1 );
    
    yresLabel = new QLabel( frame, "yresLabel" );
    yresLabel->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 1, ( QSizePolicy::SizeType ) 1, 0, 0, yresLabel->sizePolicy().hasHeightForWidth() ) );
    QFont yresLabel_font( yresLabel->font() );
    yresLabel_font.setFamily( "Helvetica" );
    yresLabel->setFont( yresLabel_font );
    yresLabel->setAlignment( int( QLabel::AlignVCenter ) );
    
    frameLayout->addWidget( yresLabel, 3, 0 );
    
    loadButton = new QToolButton( frame, "loadButton" );
    
    frameLayout->addWidget( loadButton, 4, 0 );
    
    saveButton = new QToolButton( frame, "saveButton" );
    
    frameLayout->addWidget( saveButton, 4, 1 );
    
    label = new QLabel( frame, "label" );
    label->setSizePolicy( QSizePolicy( ( QSizePolicy::SizeType ) 1, ( QSizePolicy::SizeType ) 1, 0, 0, label->sizePolicy().hasHeightForWidth() ) );
    QFont label_font( label->font() );
    label_font.setFamily( "Helvetica" );
    label->setFont( label_font );
    
    frameLayout->addWidget( label, 0, 0 );
    
    ImageViewLayout->addWidget( frame, 0, 0 );
    languageChange();
    resize( QSize( 225, 175 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    // signals and slots connections
    connect( loadButton, SIGNAL( clicked() ), this, SLOT( loadSlot() ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveSlot() ) );
    
    label->setText( image->name().c_str() );
    refreshView();
}


ImageView::~ImageView() 
{
    // no need to delete child widgets, Qt does it all for us
}


void ImageView::loadSlot() 
{
    QString path = QFileDialog::getOpenFileName( 
                        QString::null, 
                        "Images (*.png *.bmp *.jpg)",
                        this, 
                        "Load Image", 
                        "Choose an image file" );
                   
    if( path != QString::null ) 
    {
        QImage imageFile( path );
        QImage image32bpp = imageFile.convertDepth( 32 );
        
        if( image->xPixels() != image32bpp.width() ||
             image->y() != image32bpp.height() )
        {
             
            if( !QMessageBox::question( 
                                this, 
                                "Brain Builder",
                                "The image and layer sizes are different",
                                "Keep layer size", 
                                "Use new image size" ) ) 
            {
                                         
                QImage imageScaled = imageFile.scale(   image->xPixels(), 
                                                        image->y() );
                image->convertFromRGBA( imageScaled.bits() );
            } 
            else 
            {
                image->create(  image32bpp.width(), 
                                image32bpp.height(), 
                                image->encoding() );
                image->convertFromRGBA( imageFile.bits() );
            }
            
        } 
        else 
        {
            image->convertFromRGBA( imageFile.bits() );
        }
    }
    
    refreshView();
}


void ImageView::saveSlot() 
{
    QString path = QFileDialog::getSaveFileName( 
                                    QString::null, 
                                    "Images (*.png *.bmp *.jpg)",
                                    this, 
                                    "Save Image", 
                                    "Create image file name" );
                   
    if( path != QString::null ) 
    {
        path.append( ".png" );
        QImage imageFile( image->xPixels(), image->y(), 32 );
        
        unsigned char* data = imageFile.bits();
        image->convertToRGBA( data );
        
        for( int i = 0; i < image->xPixels() * image->y(); i++ ) {
            data[ i * 4 + 3 ] = data[ i * 4 + 2 ];
            data[ i * 4 + 2 ] = data[ i * 4 + 1 ];
            data[ i * 4 + 1 ] = data[ i * 4 + 0 ];
            data[ i * 4 + 0 ] = 255;
        }
        
        imageFile.save( path, "PNG" );
    }
}


void ImageView::refreshView() 
{
    xres->setValue( image->xPixels() );
    yres->setValue( image->y() );
    enc->setCurrentItem( ( int ) image->encoding() );
}


void ImageView::applyChanges() 
{
    // only allocate a new image pointer if there have been real changes
    // otherwise funny side effects might occur in other parts of the program
    if( ( xres->value() != image->xPixels() ) ||
        ( yres->value() != image->y() ) ||
        ( ( Image::ColorEncoding ) enc->currentItem() != image->encoding() ) ) 
    {
        image->create(  xres->value(), 
                        yres->value(), 
                        ( Image::ColorEncoding ) enc->currentItem() );
    }
}


void ImageView::languageChange() 
{
    setCaption( tr( "ImageView" ) );
    xresLabel->setText( tr( "X resolution" ) );
    yresLabel->setText( tr( "Y resolution" ) );
    loadButton->setText( tr( "Load" ) );
    saveButton->setText( tr( "Save" ) );
    label->setText( tr( "label" ) );
    encLabel->setText( tr( "Color encoding" ) );
    
    enc->insertItem( tr( "1  bpp Grayscale" ) );
    enc->insertItem( tr( "8  bpp Grayscale" ) );
    enc->insertItem( tr( "8  bpp Color 3:3:2" ) );
    enc->insertItem( tr( "24 bpp Color" ) );
}


ObjectView* ImageView::Create( SerializablePtr  object,
                               QWidget*         parent,
                               const char*      name ) 
{
    return new ImageView( object, parent, name );
}

