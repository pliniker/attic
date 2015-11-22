

#include <time.h>
#include <sstream>
#include <iostream>

#include <Brainlib.h>
#include <NodeLayer.h>
#include <EdgeLayer.h>
#include <Slot.h>

#include "Navigator.h"


using namespace nnet;


char const*     navigator_name = "navigator_name";



Navigator::Navigator()
{
    m_rendState.point_selected = false;
    m_rendState.point_attachable = false;
    m_rendState.point_attached = false;
    m_rendState.layer_is_node = false;
    m_rendState.layer_is_edge = false;

    ControlPoint::Reset();

    Serializable::AddFactory( "ControlPoint", ControlPoint::Create );
    Serializable::AddFactory( "NodeView", NodeView::Create );
    Serializable::AddFactory( "EdgeView", EdgeView::Create );

    string::get_list( navigator_name )->add( "ControlPoint" );
    string::get_list( navigator_name )->add( "NodeView" );
    string::get_list( navigator_name )->add( "EdgeView" );

    MapPtr nav_map( new Map( navigator_name ) );
    root_map()->add( nav_map );

    m_currentPoint  = 0;
    m_busy          = false;

    // set up signals
    global::new_signal.connect(
            boost::bind( &Navigator::reset, this ) );

    layer::new_node_signal.connect(
            boost::bind( &Navigator::newNode, this, _1 ) );

    layer::new_edge_signal.connect(
            boost::bind( &Navigator::newEdge, this, _1 ) );

    layer::erase_node_signal.connect(
            boost::bind( &Navigator::eraseNode, this, _1 ) );

    layer::erase_edge_signal.connect(
            boost::bind( &Navigator::eraseEdge, this, _1 ) );

    layer::attach_signal.connect(
            boost::bind( &Navigator::attachLayers, this, _1, _2 ) );

    layer::detach_signal.connect(
            boost::bind( &Navigator::detachLayers, this, _1, _2 ) );
}


Navigator::~Navigator()
{
    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->clear();
}


void Navigator::reset()
{
    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->clear();
    m_objects.clear();
}


void Navigator::initialize()
{
    glEnable( GL_TEXTURE_2D );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );

    glInitNames();
}


void Navigator::paint()
{
    setupView();
    updateLists();
    draw();
}


void Navigator::resize( int w, int h )
{
    m_viewPoint.resize( w, h );
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    m_viewPoint.frustum();
    glMatrixMode( GL_MODELVIEW );
}


void Navigator::newNode( NodeLayerPtr n )
{
    NodeViewPtr renderer( new NodeView( n ) );

    GLVector m_viewPointPos = m_viewPoint.position();
    m_viewPointPos.x = -m_viewPointPos.x;
    m_viewPointPos.y = -m_viewPointPos.y;
    m_viewPointPos.z = 0;
    renderer->position( m_viewPointPos );

    renderer->makeList();

    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->add( renderer );
}


void Navigator::newEdge( EdgeLayerPtr e )
{
    EdgeViewPtr renderer( new EdgeView( e ) );

    GLVector m_viewPointPos = m_viewPoint.position();
    m_viewPointPos.x = -m_viewPointPos.x;
    m_viewPointPos.y = -m_viewPointPos.y;
    m_viewPointPos.z = 0;
    renderer->position( m_viewPointPos );

    renderer->makeList();

    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->add( renderer );
}


void Navigator::eraseNode( NodeLayerPtr n )
{
    deselectPoint( m_currentPoint );
    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->erase( n->name() );
}


void Navigator::eraseEdge( EdgeLayerPtr e )
{
    deselectPoint( m_currentPoint );
    MapPtr nav_map( root_map()->get( navigator_name ) );
    nav_map->erase( e->name() );
}


//
//      Node      ->    Edge      ->    Node
//      Node:from -> to:Edge:from -> to:Node
//
void Navigator::attachLayers( LayerPtr from, LayerPtr to )
{
    MapPtr nav_map( root_map()->get( navigator_name ) );

    RenderObjectPtr from_renderer( nav_map->get( from->name() ) );
    RenderObjectPtr to_renderer( nav_map->get( to->name() ) );

    if ( from_renderer.get() && to_renderer.get() )
    {
        from_renderer->fromPoint()->attachTo(
                to_renderer->toPoint() );
    }
}


void Navigator::detachLayers( LayerPtr from, LayerPtr to )
{
    MapPtr nav_map( root_map()->get( navigator_name ) );

    RenderObjectPtr from_renderer( nav_map->get( from->name() ) );
    RenderObjectPtr to_renderer( nav_map->get( to->name() ) );

    if ( from_renderer.get() && to_renderer.get() )
    {
        // only ONE point is detached - the edge's point. Find out if we need
        // to detach the 'from' or the 'to' point...
        if( from_renderer->type() == "NodeView" &&
            to_renderer->type() == "EdgeView" )
        {
            to_renderer->toPoint()->detach();
        }
        else if( to_renderer->type() == "NodeView" &&
                 from_renderer->type() == "EdgeView" )
        {
            from_renderer->fromPoint()->detach();
        }
    }
}


bool Navigator::mousePress( int x, int y, bool left, bool right )
{
    bool rv = false;

    m_cursorState.x = x;
    m_cursorState.y = y;
    m_cursorState.l = left;
    m_cursorState.r = right;

    int objectsAtPos = findPoints( x, y );

    if( objectsAtPos >= 1 )
    {
        // IF clicked on a ControlPoint
        ControlPoint* topPoint = found( 0 );
        deselectPoint( m_currentPoint );
        selectPoint( topPoint );
    }
    else
    {
        // IF clicked in open space
        deselectPoint( m_currentPoint );
    }
    rv = true; // (de)selectPoint requires refresh

    // update point-clicked state
    if( m_currentPoint != 0 )
    {
        m_rendState.point_selected = true;
        m_rendState.point_attachable = m_currentPoint->isAttachable();
        m_rendState.point_attached = m_currentPoint->isAttached();
        m_rendState.layer_selected = layerFromPoint( m_currentPoint );

        if( m_currentPoint->owner()->type() == "NodeView" )
        {
            m_rendState.layer_is_node = true;
            m_rendState.layer_is_edge = false;
        }
        else if( m_currentPoint->owner()->type() == "EdgeView" )
        {
            m_rendState.layer_is_node = false;
            m_rendState.layer_is_edge = true;
        }
        else
        {
            m_rendState.layer_is_node = false;
            m_rendState.layer_is_edge = false;
        }
    }
    else
    {
        m_rendState.point_selected = false;
    }

    return rv;
}


bool Navigator::mouseMoveTo( int x, int y )
{
    bool rv = false;

    if( m_cursorState.l )
    {
        int xDiff = x - m_cursorState.x;
        int yDiff = m_cursorState.y - y;
        m_cursorState.x = x;
        m_cursorState.y = y;

        if( m_currentPoint )
        {
            movePoint( m_currentPoint, xDiff, yDiff );
        }
        else
        {
            moveView( xDiff, yDiff );
        }

        rv = true;
    }

    return rv;
}


bool Navigator::mouseRelease( int x, int y, bool left, bool right )
{
    bool rv = false;

    m_cursorState.x = x;
    m_cursorState.y = y;
    m_cursorState.l = !left;
    m_cursorState.r = !right;

    int objectsAtPos = findPoints( x, y );

    // if a point is validly dropped on another point, then try to attach them
    //
    // we need to know
    //  - that the point we're dropping is for an Edge layer
    //  - whether the point is the 'from' or the 'to' point
    //
    // we need to find
    //  - a Node layer to attach the point to
    //
    // if Edge 'from' point attachment order is
    //  - Edge-Node
    // if Edge 'to' point attachment order is
    //  - Node-Edge
    //
    if( left &&
        objectsAtPos >= 2 &&
        m_currentPoint->isAttachable() )
    {
        // we want to connect this edge...
        LayerPtr edge( layerFromPoint( m_currentPoint ) );

        if( isFromPoint( m_currentPoint ) )
        {
            int objectIndex = 1;
            LayerPtr node;
            do
            {
                node = layerFromPoint( found( objectIndex ) );
                ++objectIndex;
            }
            while(  !attach( edge, node ) &&
                    objectIndex < objectsAtPos );
        }
        else if( isToPoint( m_currentPoint ) )
        {
            int objectIndex = 1;
            LayerPtr node;
            do
            {
                node = layerFromPoint( found( objectIndex ) );
                ++objectIndex;
            }
            while(  !attach( node, edge ) &&
                    objectIndex < objectsAtPos );
        }
    }

    return rv;
}


bool Navigator::mouseWheel( int x, int y, float rot )
{
    int objectsAtPos = findPoints( x, y );

    if ( objectsAtPos >= 1 )
    {
        ControlPoint* topPoint = found( 0 );
        zoomPoint( topPoint, rot );
    }
    else
    {
        zoomView( rot );
    }

    return true;
}


void Navigator::detachLayer()
{
    LayerPtr edge( layerFromPoint( m_currentPoint ) );

    if( isFromPoint( m_currentPoint ) )
    {
        MapPtr outputList( edge->get( "outputList" ) );
        LayerPtr node( outputList->first() );
        detach( edge, node );
    }
    else if( isToPoint( m_currentPoint ) )
    {
        MapPtr inputList( edge->get( "inputList" ) );
        LayerPtr node( inputList->first() );
        detach( node, edge );
    }
}


Navigator::rend_state const& Navigator::state()
{
    return m_rendState;
}


void Navigator::updateLists()
{
    // update each object in turn
    RenderList::iterator i = m_objects.begin();
    while( i != m_objects.end() )
    {
        RenderObject* ob = ( *i );
        ob->updateList();
        ++i;
    }
}


void Navigator::setupView()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    m_viewPoint.frustum();
    glMatrixMode( GL_MODELVIEW );
}


void Navigator::draw()
{
    glLoadIdentity();
    m_viewPoint.translate();

    // render each object in turn
    RenderList::iterator i = m_objects.begin();
    while( i != m_objects.end() )
    {
        RenderObject* ob = ( *i );

        GLuint renderlist = ob->renderList();
        GLVector pos( ob->position() );

        glPushMatrix();
        glTranslatef( pos.x, pos.y, pos.z );

        glScalef( ob->scale(), ob->scale(), ob->scale() );
        glCallList( renderlist );
        glPopMatrix();

        ++i;
    }

    if( m_busy )
    {
        glCallList( m_busyList );
    }
}


int Navigator::findPoints( int x, int y )
{
    static GLuint hit_buffer[ HIT_BUFFER_SIZE ];

    m_objectsFound = 0;
    GLint m_viewPointportCoords[ 4 ] = { 0 };

    m_selectBuffer[ 0 ] = 0;

    glSelectBuffer( HIT_BUFFER_SIZE, hit_buffer );
    glGetIntegerv( GL_VIEWPORT, m_viewPointportCoords );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glRenderMode( GL_SELECT );
    glLoadIdentity();
    gluPickMatrix(  x,
                    m_viewPointportCoords[ 3 ] - y,
                    2,
                    2,
                    m_viewPointportCoords );
    m_viewPoint.frustum();
    glMatrixMode( GL_MODELVIEW );

    draw();
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glFlush();

    //m_objectsFound
    int hits = glRenderMode( GL_RENDER );

    glMatrixMode( GL_MODELVIEW );

    // process the hit buffer
    m_objectsFound = 0;
    GLuint* ptr = (GLuint *)hit_buffer;

    for( int i = 0; i < hits; i++ )
    {  /* for each hit  */
        GLuint names = *ptr;
        ptr += 3;

        for( unsigned int j = 0; j < names; j++ )
        {
            m_selectBuffer[ m_objectsFound ] = *ptr;
            m_objectsFound++;
            ptr++;
        }
    }

    return m_objectsFound;
}


ControlPoint* Navigator::found( int index )
{
    return ControlPoint::Get( m_selectBuffer[ index ] );
}


void Navigator::moveToFront( RenderObject *object )
{
    // move to front of render list, i.e. it'll get selected first.

    RenderList::iterator i = m_objects.begin();
    while( ( i != m_objects.end() ) &&
           ( *i != object ) )
    {
        ++i;
    }

    if( i != m_objects.end() )
    {
        m_objects.erase( i );
        m_objects.push_front( object );
    }
}


void Navigator::selectPoint( ControlPoint* point )
{

    m_currentPoint = point;
    point->owner()->select( point );
    moveToFront( point->owner() );
}


void Navigator::deselectPoint( ControlPoint* point )
{
    if( point )
    {
        point->owner()->deselect( point );
    }
    m_currentPoint = 0;
}


void Navigator::movePoint( ControlPoint* point, int xDelta, int yDelta )
{
    GLVector delta;
    delta.x = ( float ) xDelta / m_viewPoint.widthMoveFactor();
    delta.y = ( float ) yDelta / m_viewPoint.heightMoveFactor();
    delta.z = 0.0;

    point->move( delta );
}


void Navigator::zoomPoint( ControlPoint* point, float delta )
{
    point->owner()->scale( delta / 3.0 );
}


void Navigator::moveView( int xDelta, int yDelta )
{
    float x = ( float ) xDelta / m_viewPoint.widthMoveFactor();
    float y = ( float ) yDelta / m_viewPoint.heightMoveFactor();

    m_viewPoint.moveRel( x, y );
}


void Navigator::zoomView( float zDelta )
{
    m_viewPoint.zoom( zDelta * 3.0 );
}


LayerPtr Navigator::layerFromPoint( ControlPoint* point )
{
    LayerPtr layer;

    RenderObject* ro = point->owner();
    if( ro->type() == "NodeView" )
    {
        MapPtr node_map( root_map()->get( "Nodes" ) );
        layer = node_map->get( ro->name() );
    }
    else if( ro->type() == "EdgeView" )
    {
        MapPtr edge_map( root_map()->get( "Edges" ) );
        layer = edge_map->get( ro->name() );
    }

    return layer;
}


bool Navigator::isFromPoint( ControlPoint* point )
{
    bool rv = false;

    RenderObject* ro = point->owner();

    ControlPointPtr from = ro->fromPoint();

    if( point == from.get() )
    {
        rv = true;
    }

    return rv;
}


bool Navigator::isToPoint( ControlPoint* point )
{
    bool rv = false;

    RenderObject* ro = point->owner();

    ControlPointPtr to = ro->toPoint();

    if( point == to.get() )
    {
        rv = true;
    }

    return rv;
}



void Navigator::AddRenderer( RenderObject* renderer )
{
    m_objects.push_back( renderer );
}


void Navigator::EraseRenderer( RenderObject* renderer )
{
    RenderList::iterator i = m_objects.begin();
    while( ( i != m_objects.end() ) &&
           ( *i != renderer ) ) {
        ++i;
    }

    if( i != m_objects.end() ) {
        m_objects.erase( i );
    }
}


Navigator::RenderList Navigator::m_objects;


