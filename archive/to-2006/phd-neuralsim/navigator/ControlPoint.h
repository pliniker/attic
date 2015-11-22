#ifndef NAVIGATOR_CONTROLPOINT_H
#define NAVIGATOR_CONTROLPOINT_H


#include <Map.h>
#include <Boolean.h>
#include <Vector3d.h>
#include <Integer.h>
#include "OpenGL.h"
#include "GLVector.h"
#include "RenderObject.h"



class RenderObject;


class ControlPoint : public nnet::Map
{
    /// Object methods
public:
    ControlPoint( std::string name );
    ControlPoint( std::string name, RenderObject* object, GLuint id );
    virtual ~ControlPoint();

    virtual std::string type();
    virtual void read( xmlNodePtr from );

    GLuint glid();
    void glid( GLuint id );

    RenderObject* owner();
    void owner( RenderObject* o );

    nnet::Float3dPtr position();
    void move( GLVector& delta );
    void moveTo( GLVector& abspos );

    void select();
    void deselect();

    void setAttachable( bool a );
    bool isAttachable();
    bool isAttached();
    void attachTo( nnet::RefCountPtr<ControlPoint> point );
    void attachTo( ControlPoint* point );
    void detach();


    /// Object data
private:
    RenderObject*       m_renderObject;
    nnet::Float3dPtr    m_pos;
    nnet::BooleanPtr    m_attachable;

    GLuint              m_GL_ID;


    /// Class methods
public:
    // mapping control points to owner objects
    typedef std::map<GLuint, ControlPoint*, std::less<int> > CMap;
    typedef CMap::iterator CMapIt;

    static void Reset();
    static GLuint Allocate( std::string name, RenderObject* object );
    static void Add( ControlPoint* point );
    static void Erase( GLuint id );
    static CMapIt Begin();
    static CMapIt End();
    static GLuint GetId( CMapIt& it );
    static ControlPoint* Get( CMapIt& it );
    static ControlPoint* Get( GLuint id );

    static nnet::SerializablePtr Create( std::string name );


    /// Class data
private:
    static GLuint next_id;
    static CMap control_points;
};


typedef nnet::RefCountPtr<ControlPoint> ControlPointPtr;


#endif
