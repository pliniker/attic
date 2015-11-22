

#include <Brainlib.h>
#include "RenderObject.h"
#include "Navigator.h"


using namespace nnet;


RenderObject::RenderObject( std::string name ) :
Map( name ) 
{
    Navigator::AddRenderer( this );
}


RenderObject::~RenderObject() 
{
    Navigator::EraseRenderer( this );
}


void RenderObject::read( xmlNodePtr from ) 
{
    Map::read( from );
}


void RenderObject::select( ControlPoint* ) {}


void RenderObject::deselect( ControlPoint* ) {}


Float3dPtr RenderObject::position() 
{
    return Float3dPtr( Float3d::Create( "temp" ) );
}


void RenderObject::position( GLVector& ) {}


GLfloat RenderObject::scale() 
{
    return 1.0;
}


void RenderObject::scale( GLfloat ) {}


void RenderObject::ScaleRGBA( int* in, int xin, int yin,
                              int* out, int xout, int yout ) 
{
    float xScale = ( float ) xin / ( float ) xout;
    float yScale = ( float ) yin / ( float ) yout;
    
    for ( int y = 0; y < yout; y++ ) {
        int inYoffs = ( int ) ( ( float ) y * yScale ) * xin;
        int outYoffs = y * xout;
        for ( int x = 0; x < xout; x++ ) {
            int inX = ( int ) ( ( float ) x * xScale );
            out[ outYoffs + x ] = in[ inYoffs + inX ];
        }
    }
}


Pixel* RenderObject::MakeBuffer( int ID, int size ) 
{
    int old_size = m_sizes[ ID ];
    if ( size > old_size ) {
        Pixel * old_buffer = m_buffers[ ID ];
        if ( old_buffer )
            delete [] old_buffer;
        m_buffers[ ID ] = new Pixel[ size ];
        m_sizes[ ID ] = size;
    }
    return m_buffers[ ID ];
}


Pixel* RenderObject::GetBuffer( int ID ) 
{
    return m_buffers[ ID ];
}


RenderObject::BufferMap RenderObject::m_buffers;
RenderObject::SizeMap   RenderObject::m_sizes;


