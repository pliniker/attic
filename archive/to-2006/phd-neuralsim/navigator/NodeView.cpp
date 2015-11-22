
#include <Brainlib.h>
#include <ErrorInterface.h>
#include <Operations.h>
#include "NodeView.h"
#include "Navigator.h"


using namespace nnet;


NodeView::NodeView( std::string name ) :
RenderObject( name ) {

    m_selected = false;
    
    // Generate a new list name
    m_object = glGenLists( 1 );
}


NodeView::NodeView( NodeLayerPtr node ) :
RenderObject ( node->name() ),
m_nodeLayer  ( node ),
m_state      ( node->get( "state" ) ),
m_origRes    ( new Int3d( "m_origRes" ) ),
m_texRes     ( new Int3d( "m_texRes" ) ),
m_scaleFactor( new Float( "m_scaleFactor" ) ) {

    m_scaleFactor->set( 1.0 );
    m_selected = false;
    
    GLuint id = ControlPoint::Allocate( "m_texture", this );
    m_controlPoint = ControlPointPtr( ControlPoint::Get( id ) );
    m_controlPoint->setAttachable( false );
    add( m_controlPoint );
        
    add( m_scaleFactor );
    add( m_nodeLayer );
    add( m_state );
    add( m_origRes );
    add( m_texRes );
        
    // Generate a new list name
    m_object = glGenLists( 1 );
}


NodeView::~NodeView() {
    glDeleteLists( m_object, 1 );
    glDeleteTextures( 1, &m_texture );
}


MapPtr NodeView::displaysMap() {
    return m_nodeLayer;
}


std::string NodeView::type() {
    return "NodeView";
}


void NodeView::read( xmlNodePtr from ) {
    RenderObject::read( from );
    
    m_nodeLayer      = get( name() );
    m_state          = get( "state" );
    m_controlPoint   = get( "m_texture" );
    m_scaleFactor    = get( "m_scaleFactor" );
    m_origRes        = get( "m_origRes" );
    m_texRes         = get( "m_texRes" );
                  
    m_controlPoint->owner( this );
    
    makeList();
}


GLuint NodeView::makeList() {
    /////
    // build the initial m_texture
    int x = 1;
    m_origRes->x( m_state->xPixels() );
    while ( x < m_origRes->x() ) { x *= 2; } 

    int y = 1;
    m_origRes->y( m_state->y() );
    while ( y < m_origRes->y() ) { y *= 2; } 

    m_texRes->x( x * 2 );
    m_texRes->y( y * 2 );
    m_aspectRatio = ( float ) m_origRes->x() / ( float ) m_origRes->y();
    
    if ( m_aspectRatio > 5.0 ) m_aspectRatio = 5.0;
    if ( m_aspectRatio < 0.2 ) m_aspectRatio = 0.2;
        
    unsigned char* buffer1 = RenderObject::MakeBuffer( 
            1, m_origRes->x() * m_origRes->y() * 4 );
    
    unsigned char* buffer2 = RenderObject::MakeBuffer( 
            2, m_texRes->x() * m_texRes->y() * 4 );
    
    if ( m_state->size() ) {
        m_state->convertToRGBA( buffer1 );
        RenderObject::ScaleRGBA( ( int* ) buffer1, 
                                 m_origRes->x(), 
                                 m_origRes->y(),
                                 ( int* ) buffer2, 
                                 m_texRes->x(), 
                                 m_texRes->y() );
    }
    
    // make the gl m_texture
    glGenTextures( 1, &m_texture );
    glBindTexture( GL_TEXTURE_2D, m_texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_texRes->x(), m_texRes->y(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //
    /////
    // build the render list
    glNewList( m_object, GL_COMPILE );
    glPushName( m_controlPoint->glid() );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );
    
    
    glColor3f( 0.2, 0.5, 1.0 );
    GLUquadricObj *pSphere = gluNewQuadric();
    glPushMatrix();
    glTranslatef( -m_aspectRatio, 1.0, 0.0 );
    gluSphere( pSphere, 0.1, 10, 10 );
    glPopMatrix();
    gluDeleteQuadric( pSphere );
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_texture );
    
    // make the square
    glColor3f( 0.9, 0.9, 0.9 );
    glBegin( GL_QUADS );
    glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( -m_aspectRatio, -1, 0 );
    glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( -m_aspectRatio, 1, 0 );
    glTexCoord2f( 1.0f, 0.0f );
    glVertex3f( m_aspectRatio, 1, 0 );
    glTexCoord2f( 1.0f, 1.0f );
    glVertex3f( m_aspectRatio, -1, 0 );
    glEnd();
    
    if ( m_selected ) {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glColor3f( 1.0, 1.0, 1.0 );
        glRectf( -m_aspectRatio, -1.0f, m_aspectRatio, 1.0f );
    }
    
    glDisable( GL_TEXTURE_2D );
    
    glDisable( GL_BLEND );
    
    glPopName();
    glEndList();
    
    //
    /////
    
    return m_object;
}


GLuint NodeView::updateList() {

    if ( neqDimPix( m_origRes, m_state ) ) {
        glDeleteTextures( 1, &m_texture );
        makeList();
        m_origRes->x( m_state->xPixels() );
        m_origRes->y( m_state->y() );
    }
    
    if ( m_state->size() ) {
        unsigned char* buffer1 = RenderObject::GetBuffer( 1 );
        unsigned char* buffer2 = RenderObject::GetBuffer( 2 );
        m_state->convertToRGBA( buffer1 );
        RenderObject::ScaleRGBA( ( int* ) buffer1, 
                                 m_origRes->x(), 
                                 m_origRes->y(),
                                 ( int* ) buffer2, 
                                 m_texRes->x(), 
                                 m_texRes->y() );
                                 
        glBindTexture( GL_TEXTURE_2D, m_texture );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_texRes->x(), m_texRes->y(),
                         GL_RGBA, GL_UNSIGNED_BYTE, buffer2 );
    }
    
    return m_object;
}


GLuint NodeView::renderList() {
    return m_object;
}


void NodeView::select( ControlPoint* ) {
    m_selected = true;
    makeList();
}


void NodeView::deselect( ControlPoint* ) {
    m_selected = false;
    makeList();
}


Float3dPtr NodeView::position() {
    return m_controlPoint->position();
}


void NodeView::position( GLVector& delta ) {
    m_controlPoint->move( delta );
}


void NodeView::scale( GLfloat scaleDelta ) {
    m_scaleFactor->set( m_scaleFactor->get() + scaleDelta );
    if ( m_scaleFactor->get() >= 3.0 )  m_scaleFactor->set( 3.0 );
    if ( m_scaleFactor->get() <= 0.25 ) m_scaleFactor->set( 0.25 );
}


GLfloat NodeView::scale() {
    return m_scaleFactor->get();
}


ControlPointPtr NodeView::fromPoint() {
    return m_controlPoint;
}


ControlPointPtr NodeView::toPoint() {
    return m_controlPoint;
}


SerializablePtr NodeView::Create( std::string name ) {
    return NodeViewPtr( new NodeView( name ) );
}


