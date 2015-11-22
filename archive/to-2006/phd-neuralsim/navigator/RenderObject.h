#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H


#include <Map.h>
#include <Layer.h>
#include <Vector3d.h>
#include "OpenGL.h"
#include "GLVector.h"
#include "ControlPoint.h"



class ControlPoint;
typedef nnet::RefCountPtr<ControlPoint> ControlPointPtr;

class RenderObject;
typedef nnet::RefCountPtr<RenderObject> RenderObjectPtr;


typedef unsigned char Pixel;


class RenderObject : public nnet::Map {
    /// Object methods
public:
    // Base RenderObject constructor, destructor, object type
    RenderObject( std::string name );
    virtual ~RenderObject();
    
    virtual nnet::MapPtr displaysMap() = 0;
    
    // Serializable functions
    virtual void read( xmlNodePtr from );
    
    // gl render functions
    virtual GLuint makeList() = 0;
    virtual GLuint updateList() = 0;
    virtual GLuint renderList() = 0;
    // object manipulation
    virtual void select( ControlPoint* point );
    virtual void deselect( ControlPoint* point );
    
    virtual nnet::Float3dPtr position();
    virtual void position( GLVector& delta );
    
    virtual GLfloat scale();
    virtual void scale( GLfloat scaleDelta );

    virtual ControlPointPtr fromPoint() = 0;
    virtual ControlPointPtr toPoint() = 0;

    
    // Class methods
public:
    // methods & data for fast scaling of RGBA images for texture mapping
    static void ScaleRGBA( int* in, int inX, int inY,
                           int* out, int outX, int outY );
                           
    // temporary working buffer creation. Reuses existing space.
    static Pixel* MakeBuffer( int ID, int size );
    static Pixel* GetBuffer( int ID );
    
private:
    typedef std::map<int, Pixel*, std::less<int> > BufferMap;
    typedef std::map<int, int, std::less<int> > SizeMap;
    
    static BufferMap    m_buffers;
    static SizeMap      m_sizes;
};


typedef nnet::RefCountPtr<RenderObject> RenderObjectPtr;


#endif
