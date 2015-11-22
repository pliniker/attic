
#include "Serializable.h"
#include "Map.h"
#include "Integer.h"
#include "Floating.h"
#include "CString.h"
#include "Boolean.h"
#include "Array.h"
#include "OptionList.h"
#include "ObjectList.h"
#include "ObjectMaker.h"
#include "Image.h"
#include "Vector3d.h"
#include "BadObject.h"


nnet::Serializable::Serializable() {
	reserved = false;
	viewonly = false;
	dirty    = false;
}


nnet::Serializable::~Serializable() {
}


bool nnet::Serializable::isPrivate() {
	return reserved;
}


bool nnet::Serializable::isReadonly() {
	return viewonly;
}


bool nnet::Serializable::isDirty() {
	return dirty;
}


void nnet::Serializable::setPrivate( bool status ) {
	reserved = status;
}


void nnet::Serializable::setReadonly( bool status ) {
	viewonly = status;
}


void nnet::Serializable::clearDirty() {
	dirty = false;
}


void nnet::Serializable::setDirty( bool status ) {
	dirty = status;
}


void nnet::Serializable::Initialize() {
	
	factory_list = string::new_list( "all_types" );
	base_types = string::new_list( "base_types" );
		
	AddFactory( "Map", Map::Create );
	
	AddFactory( "Char", Char::Create );
	AddFactory( "Uchar", Uchar::Create );
	AddFactory( "Short", Short::Create );
	AddFactory( "Ushort", Ushort::Create );
	AddFactory( "Int", Int::Create );
	AddFactory( "Uint", Uint::Create );
	AddFactory( "Long", Long::Create );
	AddFactory( "Ulong", Ulong::Create );
	base_types->add( "Char" );
	base_types->add( "Uchar" );
	base_types->add( "Short" );
	base_types->add( "Ushort" );
	base_types->add( "Int" );
	base_types->add( "Uint" );
	base_types->add( "Long" );
	base_types->add( "Ulong" );
	
	AddFactory( "Float", Float::Create );
	AddFactory( "Double", Double::Create );
	base_types->add( "Float" );
	base_types->add( "Double" );
	
	AddFactory( "CString", CString::Create );
	base_types->add( "CString" );
	
	AddFactory( "Boolean", Boolean::Create );
	base_types->add( "Boolean" );
	
	AddFactory( "CharArray", CharArray::Create );
	AddFactory( "UcharArray", UcharArray::Create );
	AddFactory( "ShortArray", ShortArray::Create );
	AddFactory( "UshortArray", UshortArray::Create );
	AddFactory( "IntArray", IntArray::Create );
	AddFactory( "UintArray", UintArray::Create );
	AddFactory( "LongArray", LongArray::Create );
	AddFactory( "UlongArray", UlongArray::Create );
	AddFactory( "FloatArray", FloatArray::Create );
	AddFactory( "DoubleArray", DoubleArray::Create );
	base_types->add( "CharArray" );
	base_types->add( "UcharArray" );
	base_types->add( "ShortArray" );
	base_types->add( "UshortArray" );
	base_types->add( "IntArray" );
	base_types->add( "UintArray" );
	base_types->add( "LongArray" );
	base_types->add( "UlongArray" );
	base_types->add( "FloatArray" );
	base_types->add( "DoubleArray" );
	
	AddFactory( "OptionList", OptionList::Create );
	AddFactory( "ObjectList", ObjectList::Create );
	AddFactory( "ObjectMaker", ObjectMaker::Create );
	base_types->add( "OptionList" );
	base_types->add( "ObjectList" );
	base_types->add( "ObjectMaker" );
	
	AddFactory( "Image", Image::Create );
	base_types->add( "Image" );
	
	AddFactory( "Char3d", Char3d::Create );
	AddFactory( "Uchar3d", Uchar3d::Create );
	AddFactory( "Short3d", Short3d::Create );
	AddFactory( "Ushort3d", Ushort3d::Create );
	AddFactory( "Int3d", Int3d::Create );
	AddFactory( "Uint3d", Uint3d::Create );
	AddFactory( "Long3d", Long3d::Create );
	AddFactory( "Ulong3d", Ulong3d::Create );
	AddFactory( "Float3d", Float3d::Create );
	AddFactory( "Double3d", Double3d::Create );
	base_types->add( "Char3d" );
	base_types->add( "Uchar3d" );
	base_types->add( "Short3d" );
	base_types->add( "Ushort3d" );
	base_types->add( "Int3d" );
	base_types->add( "Uint3d" );
	base_types->add( "Long3d" );
	base_types->add( "Ulong3d" );
	base_types->add( "Float3d" );
	base_types->add( "Double3d" );
}


void nnet::Serializable::Cleanup() {}


void nnet::Serializable::AddFactory( std::string type, factory f ) {
	factories[type] = f;
	factory_list->add( type );
	std::string msg = "Serializable::AddFactory added " + type;
	error::log( msg );
}


nnet::SerializablePtr nnet::Serializable::Create( std::string type, std::string name ) {
	factory creator = factories[type];
	if( creator ) return creator( name );
	else          return SerializablePtr( 0 );
}


bool nnet::Serializable::Serialize( SerializablePtr root, std::string fname ) {

	xmlDocPtr document = xmlNewDoc( ( xmlChar* )"1.0" );
	xmlNodePtr root_node = xmlNewNode( NULL, ( xmlChar* )"Serializable" );
	xmlDocSetRootElement( document, root_node );
	
	if( root.get() ) Serialize( root, root_node );
	
	xmlSaveFormatFile( fname.c_str(), document, 1 );
	xmlFreeDoc( document );

	unique_objects.clear();
	
	return true;
}


nnet::SerializablePtr nnet::Serializable::Unserialize( std::string fname ) {
	
	xmlDocPtr document;
	xmlNodePtr root_node;
	
	document = xmlParseFile( fname.c_str() );
	if( document == NULL ) return SerializablePtr( 0 );
	
	root_node = xmlDocGetRootElement( document );
	if( root_node == NULL ) {
		xmlFreeDoc( document );
		return SerializablePtr( 0 );
	}
	
	if( xmlStrcmp( root_node->name, ( const xmlChar* )"Serializable" ) ) {
		xmlFreeDoc( document );
		return SerializablePtr( 0 );
	}
	
	xmlNodePtr child_element = root_node->children;
	while( child_element->type != XML_ELEMENT_NODE ) 
		child_element = child_element->next;
	
	SerializablePtr root( Unserialize( child_element ) );

	unique_objects.clear();
	
	return root;
}


void nnet::Serializable::Serialize( SerializablePtr object, xmlNodePtr to ) {
	std::ostringstream id;
	id << object.get();
	
	xmlNodePtr node = xmlNewNode( NULL, ( xmlChar* )object->type().c_str() );
	xmlNewProp( node, ( xmlChar* )"name", ( xmlChar* )object->name().c_str() );
	xmlNewProp( node, ( xmlChar* )"id", ( xmlChar* )id.str().c_str() );
	xmlNewProp( node, ( xmlChar* )"private", ( xmlChar* ) ( object->isPrivate() ? "true" : "false" ) );
	xmlNewProp( node, ( xmlChar* )"readonly", ( xmlChar* ) ( object->isReadonly() ? "true" : "false" ) );
	
	SerializablePtr known_object( unique_objects[ id.str() ] );
	if( !known_object.get() ) {
		unique_objects[ id.str() ] = object;
		object->write( node );
	}
	
	xmlAddChild( to, node );
}


nnet::SerializablePtr nnet::Serializable::Unserialize( xmlNodePtr node ) {
	if( node ) {
		xmlChar* _object_name = xmlGetProp( node, ( xmlChar* )"name" );
		xmlChar* _object_id   = xmlGetProp( node, ( xmlChar* )"id" );
		xmlChar* _object_priv = xmlGetProp( node, ( xmlChar* )"private" );
		xmlChar* _object_ro   = xmlGetProp( node, ( xmlChar* )"readonly" );
		
		if( _object_name == NULL ) return SerializablePtr( 0 );
		
		std::string object_type = ( char* )node->name;
		std::string object_name = ( char* )_object_name;
		std::string object_id   = ( char* )_object_id;
		std::string object_priv = ( char* )_object_priv;
		std::string object_ro   = ( char* )_object_ro;
		
		xmlFree( _object_name );
		xmlFree( _object_id );
		xmlFree( _object_priv );
		xmlFree( _object_ro );
				
		SerializablePtr known_object( unique_objects[object_id] );
		if( known_object.get() ) {
			return known_object;
		} else {

            try {
    			SerializablePtr new_object( Create( object_type, object_name ) );
                unique_objects[object_id] = new_object;
		  	    if ( new_object.get() ) {
				    new_object->read( node );
				    new_object->setReadonly( object_ro == "true" );
				    new_object->setPrivate( object_priv == "true" );
			    } else {
				    std::string msg = "Serializable::Unserialize cannot instantiate a " + object_type;
				    msg += " called " + object_name;
				    error::log( msg );
			    }
                return new_object;

            }
            catch( std::exception& e ) {
                LOG_EXCEPTION_E( e );
                error::std_exception( "Serializable::Unserialize()", e.what() );
                error::alert( error::UnserializeFailed );
                exit( 1 );
            }
		}
	} else {
		return SerializablePtr( 0 );
	}
}


void nnet::Serializable::AddObject( SerializablePtr object ) {
	std::ostringstream id;
	id << object.get();
	unique_objects[ id.str() ] = object;
}


nnet::SerializablePtr nnet::Serializable::GetObject( std::string id ) {
	return unique_objects[id];
}


nnet::Serializable::FactoryMap  nnet::Serializable::factories;
nnet::Serializable::ObjectMap   nnet::Serializable::unique_objects;
nnet::StringList*               nnet::Serializable::factory_list;
nnet::StringList*			    nnet::Serializable::base_types;


