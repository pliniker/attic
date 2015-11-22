#ifndef NAVIGATOR_NAVIGATOR_H
#define NAVIGATOR_NAVIGATOR_H


#include <list>
#include <Map.h>
#include "OpenGL.h"
#include "GLVector.h"
#include "ControlPoint.h"
#include "NodeView.h"
#include "EdgeView.h"
#include "ViewPoint.h"
#include "RenderObject.h"


#define HIT_BUFFER_SIZE 4096


class Navigator 
{
public:
    // Constructor & destructor
    Navigator();
    ~Navigator();
    
    void reset();
    
    // GL functions
    void initialize();
    void paint();
    void resize( int w, int h );
    
    // Brainlib interface methods
    void newNode(       nnet::NodeLayerPtr n );
    void newEdge(       nnet::EdgeLayerPtr e );
    void eraseNode(     nnet::NodeLayerPtr n );
    void eraseEdge(     nnet::EdgeLayerPtr e );
    void attachLayers(  nnet::LayerPtr from, nnet::LayerPtr to );
    void detachLayers(  nnet::LayerPtr from, nnet::LayerPtr to );

    // GUI interface methods. 
    // Return 'true' if the display needs refreshing
    bool mousePress(    int x, int y, bool left, bool right );
    bool mouseMoveTo(   int x, int y );
    bool mouseRelease(  int x, int y, bool left, bool right );
    bool mouseWheel(    int x, int y, float rot );
    void detachLayer();

    // internal state variables for public viewing
    struct rend_state {
        bool            point_selected;
        bool            point_attachable;
        bool            point_attached;
        nnet::LayerPtr  layer_selected;
        bool            layer_is_node;
        bool            layer_is_edge;
    };
    // get the internal state
    rend_state const& state();    

   
    // Private implementation
private:
    // Private OpenGL implementation
    void updateLists();
    void setupView();
    void draw(); 

     // Find id's of points at window position x,y
    int findPoints( int x, int y );
    ControlPoint* found( int index );
    
    // Point operations
    void moveToFront(   RenderObject* object );
    void selectPoint(   ControlPoint* point );
    void deselectPoint( ControlPoint* point );    
    void movePoint(     ControlPoint* point, int xDelta, int yDelta );
    void zoomPoint(     ControlPoint* point, float delta );
       
    // ViewPoint operations. Parameters are window sizes and mouse movements
    void moveView( int xDelta, int yDelta );
    void zoomView( float zDelta );

    // Matching Layers to points
    nnet::LayerPtr layerFromPoint( ControlPoint* point );
    bool isFromPoint( ControlPoint* point );
    bool isToPoint( ControlPoint* point );
    

    // Private data
private:
    struct cursor_state {
        unsigned int x;
        unsigned int y;
        bool         l;
        bool         r;
    };

    ViewPoint       m_viewPoint;
   
    // internal selection buffer
    int             m_objectsFound;
    GLuint          m_selectBuffer[ HIT_BUFFER_SIZE ];
    
    // currently selected point
    ControlPoint*   m_currentPoint;

    // mouse positions
    cursor_state    m_cursorState;

    // interaction state
    rend_state      m_rendState;

    // busy indicator
    bool            m_busy;
    GLuint          m_busyList;

    
    /// Class methods
public:
    static void AddRenderer( RenderObject* renderer );
    static void EraseRenderer( RenderObject* renderer );
    
    /// Class data
private:
    // rendering order list of IDs
    typedef std::list<RenderObject*> RenderList;

    static RenderList m_objects;
};


#endif

