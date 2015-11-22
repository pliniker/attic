

#include "Map.h"



nnet::Map::Map( std::string name ) :
m_name( name ) {}


std::string nnet::Map::name() {
	return m_name;
}


std::string nnet::Map::type() {
	return "Map";
}


void nnet::Map::write( xmlNodePtr to ) {
	for( iterator it = begin(); it != end(); it++ ) {
		SerializablePtr object( get( it ) );
		if( object.get() ) Serializable::Serialize( object, to );
	}
}


void nnet::Map::read( xmlNodePtr from ) {
	xmlNodePtr child = from->children;
	while( child ) {
		
		if( child->type == XML_ELEMENT_NODE ) {
			SerializablePtr object( Serializable::Unserialize( child ) );
			if( object.get() ) add( object );
		}
		
		child = child->next;
	}
}


nnet::Map::iterator nnet::Map::begin() {
	return m_objects.begin();
}


nnet::Map::iterator nnet::Map::end() {
	return m_objects.end();
}


void nnet::Map::add( SerializablePtr object ) {
	m_objects[ object->name() ] = object;
}


nnet::SerializablePtr nnet::Map::get( iterator it ) {
	return ( *it ).second;
}


nnet::SerializablePtr nnet::Map::get( std::string what ) {
	return m_objects[what];
}


void nnet::Map::erase( iterator it ) {
	m_objects.erase( it );
}


void nnet::Map::erase( std::string what ) {
	m_objects.erase( what );
}


std::string nnet::Map::name( iterator it ) {
	return ( *it ).first;
}


std::string nnet::Map::type( iterator it ) {
	SerializablePtr object( get( it ) );
	return object->type();
}


bool nnet::Map::exists( std::string what ) {
	SerializablePtr object = m_objects[what];
	return ( object.get() != 0 );
}


bool nnet::Map::empty() {
	return m_objects.empty();
}


void nnet::Map::clear() {
	m_objects.clear();
}


nnet::SerializablePtr nnet::Map::first() {
	return get( begin() );
}


int nnet::Map::size() {
	return m_objects.size();
}


nnet::SerializablePtr nnet::Map::Create( std::string name ) {
	return MapPtr( new Map( name ) );
}

