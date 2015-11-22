#ifndef NAVIGATOR_GLVECTOR_H
#define NAVIGATOR_GLVECTOR_H


#include <Vector3d.h>
#include "OpenGL.h"


struct GLVector {
    GLVector();
    GLVector( GLfloat i, GLfloat j, GLfloat k );
    GLVector( nnet::Float3dPtr f3d );
    
    void set( GLfloat i, GLfloat j, GLfloat k );
    void set( nnet::Float3dPtr f3d );
        
    void operator+= ( GLVector& l );
    void operator+= ( nnet::Float3dPtr f3d );
    
    void operator-= ( GLVector& l );
    void operator-= ( nnet::Float3dPtr f3d );
    
    GLfloat x, y, z;
};


#endif
