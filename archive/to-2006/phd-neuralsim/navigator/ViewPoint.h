#ifndef VIEWPOINT_H
#define VIEWPOINT_H


#include "OpenGL.h"
#include "GLVector.h"


class ViewPoint {
public:
    ViewPoint();
    
    void frustum();
    void translate();
    
    void resize( int wd, int ht );
    int width();
    int height();
    
    GLVector position();
    void moveRel( float x, float y );
    void moveAbs( float x, float y );
    void zoom( float z );
    
    float widthMoveFactor();
    float heightMoveFactor();
    
private:
    GLVector    m_pos;
    int         m_w;
    int         m_h;
};


#endif
