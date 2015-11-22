#ifndef NAVIGATOR_EDGEVIEW_H
#define NAVIGATOR_EDGEVIEW_H


#include <Vector3d.h>
#include <EdgeLayer.h>
#include "OpenGL.h"
#include "GLVector.h"
#include "RenderObject.h"



class EdgeView : public RenderObject 
{
public:
    EdgeView( std::string name );
    EdgeView( nnet::EdgeLayerPtr layer );
    ~EdgeView();
    
    // the EdgeLayer this is attached to
    virtual nnet::MapPtr displaysMap();
    
    // Serializable functions
    virtual std::string type();
    virtual void read( xmlNodePtr from );
    
    // gl render functions
    virtual GLuint makeList();
    virtual GLuint updateList();
    virtual GLuint renderList();
    
    // object manipulation
    virtual void select( ControlPoint* point );
    virtual void deselect( ControlPoint* point );
    
    virtual nnet::Float3dPtr position();
    virtual void position( GLVector& delta );
    
    virtual GLfloat scale();
    virtual void scale( GLfloat scaleDelta );
    
    virtual ControlPointPtr fromPoint();
    virtual ControlPointPtr toPoint();
    

private:
    GLVector pointOnCurve( GLVector p1,
                           GLVector p2,
                           GLVector p3,
                           GLVector p4,
                           float t );
                           
    nnet::EdgeLayerPtr  m_edgeLayer;
    ControlPointPtr     m_startPoint;
    ControlPointPtr     m_endPoint;
    ControlPointPtr     m_shapePoint;
    ControlPointPtr     m_objectPoint;
    
    nnet::Float3dPtr    m_startPos;
    nnet::Float3dPtr    m_endPos;
    nnet::Float3dPtr    m_shapePos;
    
    GLuint              m_object;
    bool                m_selected;
    
    
public:
    static nnet::SerializablePtr Create( std::string name );
};


typedef nnet::RefCountPtr<EdgeView> EdgeViewPtr;


#endif
