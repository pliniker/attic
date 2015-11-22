

#include <Map.h>

#include <CString.h>

#include <Layer.h>
#include <NodeLayer.h>
#include <EdgeLayer.h>
#include <Brainlib.h>
#include <StringList.h>
#include <ErrorInterface.h>

#include "TextIO.h"


/////
// Required by boost::python to get pointer value out of RefCountPtr
//
// const and non-const versions
namespace boost
{

template<class T>
inline T* get_pointer( nnet::RefCountPtr<T>& p )
{
    return p.get();
}

template<class T>
inline T* get_pointer( nnet::RefCountPtr<T> const& p )
{
    return p.get();
}

}
//
/////


#include <boost/python.hpp>
using namespace boost::python;


#include <qimage.h>


/////
// Map traversal wrapping
/*
typedef std::pair<  std::string const, 
                    nnet::SerializablePtr>  NameSerializablePair;

nnet::MapPtr Nodes() { return nnet::root_map()->get( "Nodes" ); }
nnet::MapPtr Edges() { return nnet::root_map()->get( "Edges" ); }
*/
nnet::SerializablePtr ( nnet::Map::*MapGetByName )( std::string ) = &nnet::Map::get;
/*
void Recall( NameSerializablePair& p ) { nnet::recall( p.second ); }
void Learn ( NameSerializablePair& p ) { nnet::learn( p.second ); }
void Reset ( NameSerializablePair& p ) { nnet::reset( p.second ); }

std::string Name( NameSerializablePair& p ) { return p.second->name(); }
std::string Type( NameSerializablePair& p ) { return p.second->type(); }
*/
//
/////

/////
// Helper functions because the alternative is to use the verbose C++
// style of accessing objects
nnet::NodeLayerPtr find_node( std::string name )
{
    nnet::NodeLayerPtr rv;

    nnet::MapPtr root( nnet::root_map() );
    nnet::MapPtr nodes( root->get( "Nodes" ) );

    nnet::Map::iterator i       = nodes->begin();
    nnet::Map::iterator iend    = nodes->end();

    bool found = false;
    while( i != iend && !found )
    {
        rv = nodes->get( i );

        nnet::CStringPtr displayName( rv->get( "displayName" ) );
        
        if( displayName->get() == name )
        {
            found = true;
        }
    }

    return rv;    
}

nnet::NodeLayerPtr find_edge( std::string name )
{
    nnet::EdgeLayerPtr rv;

    nnet::MapPtr root( nnet::root_map() );
    nnet::MapPtr edges( root->get( "Edges" ) );

    nnet::Map::iterator i       = edges->begin();
    nnet::Map::iterator iend    = edges->end();

    bool found = false;
    while( i != iend && !found )
    {
        rv = edges->get( i );
        
        nnet::CStringPtr displayName( rv->get( "displayName" ) );
        
        if( displayName->get() == name )
        {
            found = true;
        }
    }

    return rv;    
}
//
/////
//

void randomize_state( nnet::NodeLayerPtr node, int percent )
{
    nnet::randomizeState( node, percent );
}

void load_state( nnet::NodeLayerPtr node, std::string filename )
{
    nnet::ImagePtr state( node->get( "state" ) );

    QImage imageFile( filename.c_str() );
    QImage image32bpp = imageFile.convertDepth( 32 );
    QImage imageScaled = image32bpp.scale( state->xPixels(), state->y() );

    state->convertFromRGBA( imageScaled.bits() );
}

void set_textio_input( std::string name, nnet::NodeLayerPtr node )
{
    nnet::RefCountPtr<TextIO> textio_layer( find_node( name ) );
    
    textio_layer->setAgentLayer( node );
}

//
/////
// Serializable is an abstract class
struct SerializableWrap : nnet::Serializable, wrapper<nnet::Serializable>
{
    std::string name() { return this->get_override( "name" )(); }
    std::string type() { return this->get_override( "type" )(); }
};
//
/////

/////
//
BOOST_PYTHON_MODULE(nnet) 
{
    class_< nnet::Serializable, SerializableWrap,
            boost::noncopyable, nnet::SerializablePtr >( "Serializable", no_init )
        .def( "name", pure_virtual( &nnet::Serializable::name ) )
        .def( "type", pure_virtual( &nnet::Serializable::type ) );


    class_< nnet::Map, bases<nnet::Serializable>,
            boost::noncopyable, nnet::MapPtr >( "Map", no_init )
        .def( "get",        MapGetByName )
        .def( "exists",     &nnet::Map::exists )
        .def( "empty",      &nnet::Map::empty );


    class_< nnet::CString, bases<nnet::Serializable>,
            boost::noncopyable, nnet::CStringPtr >( "CString", no_init )
        .def( "get",        &nnet::CString::get, return_value_policy<return_by_value>() )
        .def( "set",        &nnet::CString::set );


    class_< nnet::Layer, bases<nnet::Map>,
            boost::noncopyable, nnet::LayerPtr >( "Layer", no_init );


    class_< nnet::NodeLayer, bases<nnet::Layer>,
            boost::noncopyable, nnet::NodeLayerPtr >( "NodeLayer", no_init );


    class_< nnet::EdgeLayer, bases<nnet::Layer>,
            boost::noncopyable, nnet::EdgeLayerPtr >( "EdgeLayer", no_init );


    def( "root", &nnet::root_map );

    def( "find_node", &find_node );
    def( "find_edge", &find_edge );

    def( "blank_state", &nnet::blankState );
    def( "randomize_state", &randomize_state );
    def( "load_state", &load_state );
    def( "textio_setinput", &set_textio_input );

//    def( "recall", &nnet::recall );
//    def( "learn",  &nnet::learn );
//    def( "reset",  &nnet::reset );

    def( "string_list", &nnet::string::get_list, return_value_policy<reference_existing_object>() );

    def( "log",   &nnet::error::log );
    def( "alert", &nnet::error::alert );
    def( "ask",   &nnet::error::ask );
    def( "clear", &nnet::error::clear );

    register_ptr_to_python<nnet::SerializablePtr>();
}
//
/////

