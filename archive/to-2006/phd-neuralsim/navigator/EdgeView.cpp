
#include <Brainlib.h>
#include <ErrorInterface.h>
#include <Operations.h>
#include <NodeLayer.h>
#include "EdgeView.h"
#include "Navigator.h"


using namespace nnet;


EdgeView::EdgeView( std::string name ) :
RenderObject( name )
{
    m_object = glGenLists( 1 );
    m_selected = false;
}


EdgeView::EdgeView( EdgeLayerPtr edge ) :
RenderObject( edge->name() ),
m_edgeLayer ( edge ),
m_startPos  ( new Float3d( "m_startPos" ) ),
m_endPos    ( new Float3d( "m_endPos" ) ),
m_shapePos  ( new Float3d( "m_shapePos" ) )
{

    add( m_edgeLayer );

    GLuint id;

    id = ControlPoint::Allocate( "m_startPoint", this );
    m_startPoint = ControlPointPtr( ControlPoint::Get( id ) );
    m_startPoint->position()->set( -2.0, 2.0, 0.0 );

    id = ControlPoint::Allocate( "m_endPoint", this );
    m_endPoint = ControlPointPtr( ControlPoint::Get( id ) );
    m_endPoint->position()->set( 2.0, -2.0, 0.0 );

    id = ControlPoint::Allocate( "m_shapePoint", this );
    m_shapePoint = ControlPointPtr( ControlPoint::Get( id ) );
    m_shapePoint->position()->set( 0.0, 0.0, 0.0 );

    id = ControlPoint::Allocate( "m_objectPoint", this );
    m_objectPoint = ControlPointPtr( ControlPoint::Get( id ) );
    m_objectPoint->position()->set( 0.0, 0.0, 0.0 );

    m_startPos->set  ( m_startPoint->position() );
    m_endPos->set    ( m_endPoint->position() );
    m_shapePos->set  ( m_shapePoint->position() );

    add( m_startPoint );
    add( m_endPoint );
    add( m_shapePoint );
    add( m_objectPoint );
    add( m_startPos );
    add( m_endPos );
    add( m_shapePos );

    m_startPoint->setAttachable  ( true );
    m_endPoint->setAttachable    ( true );
    m_shapePoint->setAttachable  ( false );
    m_objectPoint->setAttachable ( false );

    m_object = glGenLists( 1 );

    m_selected = false;
}


EdgeView::~EdgeView()
{
    glDeleteLists( m_object, 1 );
}


MapPtr EdgeView::displaysMap()
{
    return m_edgeLayer;
}


std::string EdgeView::type()
{
    return "EdgeView";
}


void EdgeView::read( xmlNodePtr from )
{
    RenderObject::read( from );

    m_edgeLayer      = get( name() );
    m_startPoint     = get( "m_startPoint" );
    m_endPoint       = get( "m_endPoint" );
    m_shapePoint     = get( "m_shapePoint" );
    m_objectPoint    = get( "m_objectPoint" );
    m_startPos       = get( "m_startPos" );
    m_endPos         = get( "m_endPos" );
    m_shapePos       = get( "m_shapePos" );

    m_startPoint->owner  ( this );
    m_endPoint->owner    ( this );
    m_shapePoint->owner  ( this );
    m_objectPoint->owner ( this );

    makeList();
}


GLuint EdgeView::makeList()
{
    /////
    // build the render list
    glNewList( m_object, GL_COMPILE );

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );

    // draw control points
    if( m_selected )
    {
        GLVector pos;
        GLUquadricObj *pSphere;

        // start controlpoint
        glPushName( m_startPoint->glid() );
        pos = m_startPoint->position();

        glColor3f( 1.0, 0.0, 0.0 );
        pSphere = gluNewQuadric();
        glPushMatrix();
        glTranslatef( pos.x, pos.y, pos.z );
        gluSphere( pSphere, 0.2, 10, 10 );
        glPopMatrix();
        gluDeleteQuadric( pSphere );

        glPopName();

        // end controlpoint
        glPushName( m_endPoint->glid() );
        pos = m_endPoint->position();

        glColor3f( 0.0, 1.0, 0.0 );
        pSphere = gluNewQuadric();
        glPushMatrix();
        glTranslatef( pos.x, pos.y, pos.z );
        gluSphere( pSphere, 0.2, 10, 10 );
        glPopMatrix();
        gluDeleteQuadric( pSphere );

        glPopName();

        // shape controlpoint
        glPushName( m_shapePoint->glid() );
        pos = m_shapePoint->position();

        glColor3f( 1.0, 1.0, 1.0 );
        pSphere = gluNewQuadric();
        glPushMatrix();
        glTranslatef( pos.x, pos.y, pos.z );
        gluSphere( pSphere, 0.2, 10, 10 );
        glPopMatrix();
        gluDeleteQuadric( pSphere );

        glPopName();
    }

    glLineWidth( 7.0 );

    // curve
    glPushName( m_objectPoint->glid() );
    glBegin( GL_LINE_STRIP );
    glColor3f( 0.0, 1.0, 0.0 );

    GLVector cPoint1;
    GLVector cPoint2;

    // are we connected back to ourselves?
    if ( m_startPos == m_endPos )
    {
        float diff = m_shapePos->x() - m_startPos->x();
        cPoint1.x = m_startPos->x() + diff;
        cPoint1.y = m_startPos->y() - diff;
        cPoint1.z = m_shapePos->z();

        cPoint2.x = m_startPos->x() + diff;
        cPoint2.y = m_startPos->y() + diff;
        cPoint2.z = m_shapePos->z();
    }
    else
    {
        cPoint1.x = ( m_endPos->x() - m_startPos->x() ) / 2.0 + m_shapePos->x();
        cPoint1.y = ( m_startPos->y() - m_endPos->y() ) / 2.0 + m_shapePos->y();
        cPoint1.z = ( m_startPos->z() - m_endPos->z() ) / 2.0 + m_shapePos->z();

        cPoint2.x = ( m_startPos->x() - m_endPos->x() ) / 2.0 + m_shapePos->x();
        cPoint2.y = ( m_endPos->y() - m_startPos->y() ) / 2.0 + m_shapePos->y();
        cPoint2.z = ( m_startPos->z() - m_endPos->z() ) / 2.0 + m_shapePos->z();
    }

    GLVector vPoint, aPoint;
    float steps = 20.0;
    for ( float t = 0.0; t <= ( 1.0 + ( 1.0f / steps ) ); t += 1.0f / steps )
    {
        vPoint = pointOnCurve(  m_startPoint->position(),
                                cPoint1,
                                cPoint2,
                                m_endPoint->position(),
                                t );
        glColor3f( 1.0 - t, t, 0.0 );
        glVertex3f( vPoint.x, vPoint.y, vPoint.z );
    }

    glEnd();
    glPopName();

    glDisable( GL_BLEND );
    glEndList();
    //
    /////

    return m_object;
}


GLuint EdgeView::updateList()
{
    if ( ( m_startPos != m_startPoint->position() ) ||
         ( m_endPos   != m_endPoint->position()   ) ||
         ( m_shapePos != m_shapePoint->position() ) )
    {
        m_startPos->set  ( m_startPoint->position() );
        m_endPos->set    ( m_endPoint->position() );
        m_shapePos->set  ( m_shapePoint->position() );
        makeList();
    }
    return m_object;
}


GLuint EdgeView::renderList()
{
    return m_object;
}


void EdgeView::select( ControlPoint* )
{
    m_selected = true;
    makeList();
}


void EdgeView::deselect( ControlPoint* )
{
    m_selected = false;
    makeList();
}


Float3dPtr EdgeView::position()
{
    m_objectPoint->position()->set( 0.0, 0.0, 0.0 );
    return m_objectPoint->position();
}


void EdgeView::position( GLVector& delta )
{
    m_startPoint->move   ( delta );
    m_endPoint->move     ( delta );
    m_shapePoint->move   ( delta );
    m_startPos->set  ( m_startPoint->position() );
    m_endPos->set    ( m_endPoint->position() );
    m_shapePos->set  ( m_shapePoint->position() );
}


void EdgeView::scale( GLfloat ) {}


GLfloat EdgeView::scale()
{
    return 1.0;
}


GLVector EdgeView::pointOnCurve( GLVector p1,
                                 GLVector p2,
                                 GLVector p3,
                                 GLVector p4,
                                 float t )
{
    float var1, var2, var3;
    GLVector vPoint( 0.0f, 0.0f, 0.0f );

    var1 = 1 - t;
    var2 = var1 * var1 * var1;
    var3 = t * t * t;

    vPoint.x = ( var2 * p1.x ) +
               ( 3 * t * var1 * var1 * p2.x ) +
               ( 3 * t * t * var1 * p3.x ) +
               ( var3 * p4.x );
    vPoint.y = ( var2 * p1.y ) +
               ( 3 * t * var1 * var1 * p2.y ) +
               ( 3 * t * t * var1 * p3.y ) +
               ( var3 * p4.y );
    vPoint.z = ( var2 * p1.z ) +
               ( 3 * t * var1 * var1 * p2.z ) +
               ( 3 * t * t * var1 * p3.z ) +
               ( var3 * p4.z );

    return vPoint;
}


ControlPointPtr EdgeView::fromPoint()
{
    return m_endPoint;
}


ControlPointPtr EdgeView::toPoint()
{
    return m_startPoint;
}


SerializablePtr EdgeView::Create( std::string name )
{
    return EdgeViewPtr( new EdgeView( name ) );
}


