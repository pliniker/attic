

#include "GLVector.h"


using namespace nnet;


GLVector::GLVector() {}


GLVector::GLVector( GLfloat i, GLfloat j, GLfloat k ) {
    x = i;
    y = j;
    z = k;
}


GLVector::GLVector( Float3dPtr f3d ) {
    x = f3d->x();
    y = f3d->y();
    z = f3d->z();
}


void GLVector::set
    ( GLfloat i, GLfloat j, GLfloat k ) {
    x = i;
    y = j;
    z = k;
}


void GLVector::set
    ( Float3dPtr f3d ) {
    x = f3d->x();
    y = f3d->y();
    z = f3d->z();
}


void GLVector::operator+= ( GLVector &l ) {
    x += l.x;
    y += l.y;
    z += l.z;
}
                          
                          
void GLVector::operator+= ( Float3dPtr f3d ) {
    x += f3d->x();
    y += f3d->y();
    z += f3d->z();
}
                                                    
                                                    
void GLVector::operator-= ( GLVector &l ) {
    x -= l.x;
    y -= l.y;
    z -= l.z;
}
                                                                              
                                                                              
void GLVector::operator-= ( Float3dPtr f3d ) {
    x -= f3d->x();
    y -= f3d->y();
    z -= f3d->z();
}


