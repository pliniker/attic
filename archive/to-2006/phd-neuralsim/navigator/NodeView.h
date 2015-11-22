#ifndef NAVIGATOR_NODEVIEW_H
#define NAVIGATOR_NODEVIEW_H


#include <NodeLayer.h>
#include <Image.h>
#include <Vector3d.h>
#include <Floating.h>
#include "OpenGL.h"
#include "GLVector.h"
#include "RenderObject.h"



class NodeView : public RenderObject 
{
public:
    // Constructor/destructor
    NodeView( std::string name );
    NodeView( nnet::NodeLayerPtr node );
    ~NodeView();
    
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
    ControlPointPtr     m_controlPoint;
    nnet::NodeLayerPtr  m_nodeLayer;
    nnet::ImagePtr      m_state;
    nnet::Int3dPtr      m_origRes;
    nnet::Int3dPtr      m_texRes;
    nnet::FloatPtr      m_scaleFactor;
    
    GLuint              m_object;
    GLuint              m_texture;          // object and texture names
    GLfloat             m_aspectRatio;      // rectangle aspect ratio
    bool                m_selected;
    
    
public:
    static nnet::SerializablePtr Create( std::string name );
};


typedef nnet::RefCountPtr<NodeView> NodeViewPtr;


#endif
