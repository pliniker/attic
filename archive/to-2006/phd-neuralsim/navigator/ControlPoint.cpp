#include <iostream>

#include <ErrorInterface.h>
#include "ControlPoint.h"
#include "NavErr.h"


using namespace nnet;


ControlPoint::ControlPoint( std::string name ) :
    Map             ( name ),
    m_renderObject  ( 0 ),
    m_pos           ( new Float3d( "pos" ) ),
    m_attachable    ( new Boolean( "m_attachable" ) )
{
    add( m_pos );
    add( m_attachable );
    m_attachable->set( false );
}


ControlPoint::ControlPoint( std::string name,
                            RenderObject* object,
                            GLuint id ) :
    Map             ( name ),
    m_pos           ( new Float3d( "pos" ) ),
    m_attachable    ( new Boolean( "m_attachable" ) )
{
    m_renderObject = object;
    m_GL_ID = id;

    add( m_pos );
    add( m_attachable );
    m_attachable->set( false );
}


ControlPoint::~ControlPoint()
{
    //    detach();
    Erase( m_GL_ID );
}


std::string ControlPoint::type()
{
    return "ControlPoint";
}


void ControlPoint::read( xmlNodePtr from )
{
    Map::read( from );

    m_pos = get( "pos" );
    m_attachable = get( "m_attachable" );

    Add( this );
}


GLuint ControlPoint::glid()
{
    return m_GL_ID;
}


void ControlPoint::glid( GLuint id )
{
    m_GL_ID = id;
}


RenderObject* ControlPoint::owner()
{
    return m_renderObject;
}


void ControlPoint::owner( RenderObject* o )
{
    m_renderObject = o;
}


Float3dPtr ControlPoint::position()
{
    return m_pos;
}


void ControlPoint::move( GLVector& delta )
{
    m_pos->x( m_pos->x() + delta.x );
    m_pos->y( m_pos->y() + delta.y );
    m_pos->z( m_pos->z() + delta.z );
}


void ControlPoint::moveTo( GLVector& abspos )
{
    m_pos->x( abspos.x );
    m_pos->y( abspos.y );
    m_pos->z( abspos.z );
}


void ControlPoint::setAttachable( bool a )
{
    if( !a )
    {
        detach();
    }
    m_attachable->set( a );
}


bool ControlPoint::isAttachable()
{
    return m_attachable->get();
}


bool ControlPoint::isAttached()
{
    // if somebody has checked out m_pos via position() then this will fail as
    // the reference count will be greater than 1.
    // Ordinarily this should be ok though.
    return !m_pos.unique();
}


void ControlPoint::attachTo( ControlPointPtr point )
{
    if( isAttachable() )
    {
        m_pos = point->position();
        add( m_pos );
    }
    else if( point->isAttachable() )
    {
        point->attachTo( this );
    }
}


void ControlPoint::attachTo( ControlPoint* point )
{
    if( isAttachable() )
    {
        m_pos = point->position();
        add( m_pos );
    }
    else if( point->isAttachable() )
    {
        point->attachTo( this );
    }
}


void ControlPoint::detach()
{
    if ( m_attachable->get() )
    {
        Float3dPtr newPos( new Float3d( "pos" ) );
        newPos->set( m_pos );
        m_pos = newPos;
        add( m_pos );
    }
}


void ControlPoint::select()
{
    m_renderObject->select( this );
}


void ControlPoint::deselect()
{
    m_renderObject->deselect( this );
}


void ControlPoint::Reset()
{
    next_id = 100;
    control_points.clear();
}


GLuint ControlPoint::Allocate( std::string name, RenderObject* object )
{
    next_id++;
    control_points[ next_id ] = new ControlPoint( name, object, next_id );
    return next_id;
}


void ControlPoint::Add( ControlPoint* point )
{
    next_id++;
    point->glid( next_id );
    control_points[ next_id ] = point;
}


void ControlPoint::Erase( GLuint id )
{
    control_points.erase( id );
}


ControlPoint::CMapIt ControlPoint::Begin()
{
    return control_points.begin();
}


ControlPoint::CMapIt ControlPoint::End()
{
    return control_points.end();
}


GLuint ControlPoint::GetId( CMapIt& it )
{
    return ( *it ).first;
}


ControlPoint *ControlPoint::Get( CMapIt& it )
{
    return ( *it ).second;
}


ControlPoint *ControlPoint::Get( GLuint id )
{
    CMapIt i = control_points.find( id );

    if( i != control_points.end() )
    {
        return i->second;
    }
    else
    {

        std::cout << "wanted " << id << std::endl;

        // we shouldn't be called with an id not associated with anything
        THROW_NAVERR( "the GLid has no ControlPoint object" );
    }
}


SerializablePtr ControlPoint::Create( std::string name )
{
    return ControlPointPtr( new ControlPoint( name ) );
}


GLuint ControlPoint::next_id = 100;
ControlPoint::CMap ControlPoint::control_points;


