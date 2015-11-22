

#include <boost/signals.hpp> 
// boost/signals.hpp must be included first
// whereever relevant or freaky things happen!
#include <qaction.h>
#include <qtimer.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <StringList.h>
#include <Brainlib.h>
#include <NodeLayer.h>
#include <EdgeLayer.h>
#include <BadObject.h>
#include <ControlPoint.h>
#include "NavFrame.h"



NavFrame::NavFrame( QWidget* parent, char const* name ) :
    QGLWidget( parent, name ) 
{
    newObjectCreator = new NewObject( this );
    objectBrowser = new ObjectBrowser( this );
    
    navigator = new Navigator();
    
    newNodeAction = new QAction( this, "newNodeAction" );
    newEdgeAction = new QAction( this, "newEdgeAction" );
    deleteObjectAction = new QAction( this, "deleteObjectAction" );
    recallObjectAction = new QAction( this, "recallObjectAction" );
    trainObjectAction = new QAction( this, "trainObjectAction" );
    checkObjectAction = new QAction( this, "checkObjectAction" );
    browseObjectAction = new QAction( this, "browseObjectAction" );
    copyImageAction = new QAction( this, "copyImageAction" );
    pasteObjectAction = new QAction( this, "pasteObjectAction" );
    pasteImageAction = new QAction( this, "pasteImageAction" );
    blankImageAction = new QAction( this, "blankImageAction" );
    disconnectObjectAction = new QAction( this, "disconnectObjectAction" );
    
    objectMenu = new QPopupMenu( this, "objectMenu" );
    
    pasteMenu = new QPopupMenu( objectMenu, "pasteMenu" );
    pasteObjectAction->addTo( pasteMenu );
    pasteImageAction->addTo( pasteMenu );
    
    newNodeAction->addTo( objectMenu );
    newEdgeAction->addTo( objectMenu );
    deleteObjectAction->addTo( objectMenu );
    // recallObjectAction->addTo( objectMenu );
    // trainObjectAction->addTo( objectMenu );
    // checkObjectAction->addTo( objectMenu );
    browseObjectAction->addTo( objectMenu );
    disconnectObjectAction->addTo( objectMenu );
    copyImageAction->addTo( objectMenu );
    objectMenu->insertItem( tr( "Paste" ), pasteMenu );
    blankImageAction->addTo( objectMenu );
    
    languageChange();
    
    connect( newNodeAction, SIGNAL( activated() ),
             this, SLOT( newNodeSlot() ) );
    connect( newEdgeAction, SIGNAL( activated() ),
             this, SLOT( newEdgeSlot() ) );
    connect( deleteObjectAction, SIGNAL( activated() ),
             this, SLOT( deleteObjectSlot() ) );
    connect( browseObjectAction, SIGNAL( activated() ),
             this, SLOT( browseObjectSlot() ) );
    connect( recallObjectAction, SIGNAL( activated() ),
             this, SLOT( recallSlot() ) );
    connect( trainObjectAction, SIGNAL( activated() ),
             this, SLOT( trainSlot() ) );
    connect( checkObjectAction, SIGNAL( activated() ),
             this, SLOT( checkSlot() ) );
    connect( copyImageAction, SIGNAL( activated() ),
             this, SLOT( copyImageSlot() ) );
    connect( pasteObjectAction, SIGNAL( activated() ),
             this, SLOT( pasteObjectSlot() ) );
    connect( pasteImageAction, SIGNAL( activated() ),
             this, SLOT( pasteImageSlot() ) );
    connect( disconnectObjectAction, SIGNAL( activated() ),
             this, SLOT( disconnectSlot() ) );
    connect( blankImageAction, SIGNAL( activated() ),
             this, SLOT( blankImageSlot() ) );
             
    continueExecution = false;
    executionInterval = 250;
    
    nodeAttributes = nnet::ImagePtr( new nnet::Image( "nodeAttributes" ) );
    edgeAttributes = nnet::UintPtr( new nnet::Uint( "edgeAttributes" ) );
    edgeAttributes->set( 16 );
    
    mouseWheelFactor = 120; // * 3.0;

    leftPressed = false;
    rightPressed = false;
}


NavFrame::~NavFrame() 
{
    delete navigator;
}


void NavFrame::reset() 
{
    // TODO what's going on here?
    navigator->reset();
    updateGL();
}


void NavFrame::newNodeSlot() 
{
    newObjectCreator->clear();
    newObjectCreator->populateWith( nnet::string::get_list( "node_layers" ) );
    
    if( newObjectCreator->exec() == QDialog::Accepted ) 
    {
        std::string type = newObjectCreator->typeSelected().latin1();
        
        nnet::NodeLayerPtr node;
        
        try 
        {
            node = nnet::new_node( type );
        } 
        catch( std::exception const& e ) 
        {
            LOG_EXCEPTION_E( e );
            nnet::error::alert( nnet::error::NewObjectFailed );
        }
    }
    
    updateGL();
}


void NavFrame::newEdgeSlot() 
{
    newObjectCreator->clear();
    newObjectCreator->populateWith( nnet::string::get_list( "edge_layers" ) );
    
    if( newObjectCreator->exec() == QDialog::Accepted )
    {
        std::string type = newObjectCreator->typeSelected().latin1();
        
        nnet::EdgeLayerPtr edge;
        
        try 
        {
            edge = nnet::new_edge( type );
        } 
        catch( std::exception const& e ) 
        {
            LOG_EXCEPTION_E( e );
            nnet::error::alert( nnet::error::NewObjectFailed );
        }
    }
    
    updateGL();
}


void NavFrame::deleteObjectSlot() 
{
    nnet::LayerPtr object = navigator->state().layer_selected;

    try
    {
        if( navigator->state().layer_is_edge ) 
        {
            nnet::erase_edge( object );
        } 
        else if( navigator->state().layer_is_node )
        {
            nnet::erase_node( object );
        }
    } 
    catch( std::exception const& e ) 
    {
        LOG_EXCEPTION_E( e );
        nnet::error::std_exception( object->name(), e.what() );
        nnet::error::alert( nnet::error::DeleteObjectFailed );
    }
    
    updateGL();
}


void NavFrame::browseObjectSlot() 
{
    try 
    {
        if( navigator->state().point_selected )
        {
            nnet::MapPtr map( navigator->state().layer_selected );
            objectBrowser->display( map );
            objectBrowser->exec();
        } 
        else 
        {
            objectBrowser->display( nnet::root_map() );
            objectBrowser->exec();
        }
    } 
    catch ( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        nnet::error::std_exception( "Object Browser", e.what() );
    }
}


void NavFrame::copyImageSlot() 
{
    if( navigator->state().point_selected && 
        navigator->state().layer_is_node )
    {
        nnet::LayerPtr layer( navigator->state().layer_selected );

        nnet::ImagePtr state( layer->get( "state" ) );

        nodeAttributes->create( state->xPixels(), 
                                state->y(),
                                state->encoding() );

        for( int i = 0; i < state->size(); i++ ) 
        {
            nodeAttributes->at( i ) = state->at( i );
        }
    }
    else
    {
        nnet::error::log( "no node layer selected" );
    }
}


void NavFrame::pasteObjectSlot() 
{
    if( navigator->state().point_selected &&
        navigator->state().layer_is_node )
    {
        nnet::LayerPtr layer( navigator->state().layer_selected );

        nnet::ImagePtr state( layer->get( "state" ) );

        state->create(  nodeAttributes->xPixels(),
                        nodeAttributes->y(),
                        nodeAttributes->encoding() );
    }
    else
    {
        nnet::error::log( "no node layer selected" );
    }
}


void NavFrame::pasteImageSlot() 
{
    if( navigator->state().point_selected &&
        navigator->state().layer_is_node )
    {
        nnet::LayerPtr layer( navigator->state().layer_selected );

        nnet::ImagePtr state( layer->get( "state" ) );

        if( state->x() == nodeAttributes->x() &&
            state->y() == nodeAttributes->y() )
        {
            for( int i = 0; i < state->size(); i++ )
            {
                state->at( i ) = nodeAttributes->at( i );
            }        
        }
        else
        {
            nnet::error::log( "state attributes do not match clipboard image" );
        }
    }
    else
    {
        nnet::error::log( "no node layer selected" );
    }
}


void NavFrame::blankImageSlot() 
{
    if( navigator->state().point_selected &&
        navigator->state().layer_is_node )
    {
        nnet::LayerPtr layer( navigator->state().layer_selected );
        nnet::blankState( layer );
    }
    else
    {
        nnet::error::log( "no node layer selected" );
    }
}


void NavFrame::recallSlot()
{
    try
    {
        nnet::error::clear();
        nnet::error::log( "--- recalling ---" );
        nnet::recall();
        nnet::error::log( "<<< done >>>" );
    }
    catch ( nnet::BadObject& e )
    {
        LOG_EXCEPTION_E( e );
        nnet::error::log( e.what() );
        nnet::error::alert( nnet::error::RecallObjectFailed );
        return ;
    }
    
    updateGL();
    
    if( continueExecution )
    {
        QTimer::singleShot( executionInterval, this, SLOT( recallSlot() ) );
    }
}


void NavFrame::trainSlot()
{
    try
    {
        nnet::error::clear();
        nnet::error::log( "--- training ---" );
        nnet::learn();
        nnet::error::log( "<<< done >>>" );
    }
    catch ( nnet::BadObject& e )
    {
        LOG_EXCEPTION_E( e );
        nnet::error::log( e.what() );
        nnet::error::alert( nnet::error::LearnObjectFailed );
        return ;
    }
    
    updateGL();
    
    if( continueExecution )
    {
        QTimer::singleShot( executionInterval, this, SLOT( trainSlot() ) );
    }
}


void NavFrame::recallTrainSlot()
{
    try
    {
        nnet::error::clear();
        nnet::error::log( "--- recalling ---" );
        nnet::recall();
        nnet::error::log( "--- training ---" );
        nnet::learn();
        nnet::error::log( "<<< done >>>" );
    }
    catch ( nnet::BadObject& e )
    {
        LOG_EXCEPTION_E( e );
        nnet::error::log( e.what() );
        nnet::error::alert( nnet::error::LearnObjectFailed );
        return ;
    }
    
    updateGL();
}


void NavFrame::recallContSlot()
{
    nnet::error::clear();
    continueExecution = true;
    recallSlot();
}


void NavFrame::trainContSlot()
{
    nnet::error::clear();
    continueExecution = true;
    trainSlot();
}


void NavFrame::stopSlot()
{
    continueExecution = false;
}


void NavFrame::checkSlot()
{
    nnet::error::clear();

    try
    {
        nnet::check();
    }
    catch ( nnet::BadObject& e )
    {
        LOG_EXCEPTION_E( e );
        nnet::error::log( e.what() );
        nnet::error::alert( nnet::error::CheckObjectFailed );
    }

    updateGL();
}


void NavFrame::resetSlot()
{
    if( QMessageBox::question(
             this,
             "Brain Builder",
             "Resetting the network will cause all training to be lost!",
             "No don't reset", "Yes do reset" ) )
    {
        try
        {
            nnet::reset();
        }
        catch ( nnet::BadObject& e )
        {
            LOG_EXCEPTION_E( e );
            nnet::error::log( e.what() );
            nnet::error::log( nnet::error::ResetObjectFailed );
        }
    }
}


void NavFrame::disconnectSlot() 
{
    if( navigator->state().point_attached &&
        navigator->state().layer_is_edge )
    {
        navigator->detachLayer();
    }
    updateGL();
}


void NavFrame::initializeGL()
{
    navigator->initialize();
}


void NavFrame::paintGL()
{
    navigator->paint();
}


void NavFrame::resizeGL( int w, int h )
{
    navigator->resize( w, h );
}


void NavFrame::mousePressEvent( QMouseEvent* e )
{
    QPoint at = e->pos();

    // left button event
    if( e->button() == LeftButton )
    {
        leftPressed = true;
    }

    // right button event
    if( e->button() == RightButton )
    {
        rightPressed = true;
    }
    
    navigator->mousePress( at.x(), at.y(), leftPressed, rightPressed );

    updateGL();
}


void NavFrame::mouseReleaseEvent( QMouseEvent* e )
{
    QPoint at = e->pos();

    navigator->mouseRelease( at.x(), at.y(), leftPressed, rightPressed );
    
    if( e->button() == LeftButton ) 
    {
        leftPressed = false;
    }

    if( e->button() == RightButton )
    {
        rightPressed = false;

        // do context menu
        if( navigator->state().point_selected )
        {
            disconnectObjectAction->setEnabled( navigator->state().point_attachable );
            deleteObjectAction->setEnabled( !navigator->state().point_attached );
        }
        else
        {
            disconnectObjectAction->setEnabled( false );
            deleteObjectAction->setEnabled( false );
        }

        objectMenu->exec( mapToGlobal( at ) );
    }
    
    updateGL();
}


void NavFrame::mouseMoveEvent( QMouseEvent* e )
{
    QPoint at = e->pos();
    
    navigator->mouseMoveTo( at.x(), at.y() );

    if( leftPressed || rightPressed )
    {
        updateGL();
    }
}


void NavFrame::wheelEvent( QWheelEvent* e )
{
    QPoint at = e->pos();

    navigator->mouseWheel( at.x(), at.y(), e->delta() / mouseWheelFactor );
    
    updateGL();
    e->accept();
}


void NavFrame::languageChange()
{
    newNodeAction->setText( tr( "New Node" ) );
    newNodeAction->setMenuText( tr( "New Node" ) );
    newNodeAction->setAccel( tr( "Ctrl+N" ) );
    newEdgeAction->setText( tr( "New Edge" ) );
    newEdgeAction->setMenuText( tr( "New Edge" ) );
    newEdgeAction->setAccel( tr( "Ctrl+E" ) );
    deleteObjectAction->setText( tr( "Delete" ) );
    deleteObjectAction->setMenuText( tr( "Delete" ) );
    deleteObjectAction->setAccel( tr( "Delete" ) );
    recallObjectAction->setText( tr( "Recall" ) );
    recallObjectAction->setMenuText( tr( "Recall" ) );
    trainObjectAction->setText( tr( "Train" ) );
    trainObjectAction->setMenuText( tr( "Train" ) );
    checkObjectAction->setText( tr( "Verify" ) );
    checkObjectAction->setMenuText( tr( "Verify" ) );
    browseObjectAction->setText( tr( "Properties" ) );
    browseObjectAction->setMenuText( tr( "Properties" ) );
    browseObjectAction->setAccel( tr( "Ctrl+B" ) );
    copyImageAction->setText( tr( "Copy" ) );
    copyImageAction->setMenuText( tr( "Copy" ) );
    pasteObjectAction->setText( tr( "Attributes" ) );
    pasteObjectAction->setMenuText( tr( "Attributes" ) );
    pasteImageAction->setText( tr( "Image" ) );
    pasteImageAction->setMenuText( tr( "Image" ) );
    blankImageAction->setText( tr( "Blank" ) );
    blankImageAction->setMenuText( tr( "Blank" ) );
    disconnectObjectAction->setText( tr( "Disconnect" ) );
}



