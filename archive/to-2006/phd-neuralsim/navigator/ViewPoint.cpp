

#include "ViewPoint.h"
#include <iostream>

ViewPoint::ViewPoint() {
    m_pos.x = 0.0;
    m_pos.y = 0.0;
    m_pos.z = -70.0;
}


void ViewPoint::frustum() {
    glFrustum( -1.0, 1.0, -1.0, 1.0, 10.0, 200.0 );
}


void ViewPoint::translate() {
    glTranslatef( m_pos.x, m_pos.y, m_pos.z );
}


void ViewPoint::resize( int wd, int ht ) {
    m_w = wd;
    m_h = ht;
}


int ViewPoint::width() {
    return m_w;
}


int ViewPoint::height() {
    return m_h;
}


GLVector ViewPoint::position() {
    return m_pos;
}


void ViewPoint::moveRel( float x, float y ) {
    m_pos.x += x;
    m_pos.y += y;
}


void ViewPoint::moveAbs( float x, float y ) {
    m_pos.x = x;
    m_pos.y = y;
}


void ViewPoint::zoom( float z ) {
    m_pos.z += z;
    if ( m_pos.z >= -50.0 )  m_pos.z = -50.0;
    if ( m_pos.z <= -150.0 ) m_pos.z = -150.0;
}


float ViewPoint::widthMoveFactor() {
    return m_w / ( -m_pos.z / 5.0 );
}


float ViewPoint::heightMoveFactor() {
    return m_h / ( -m_pos.z / 5.0 );
}


