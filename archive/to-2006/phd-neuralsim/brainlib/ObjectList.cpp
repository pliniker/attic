

#include "ObjectList.h"



nnet::ObjectList::ObjectList( std::string name ) :
nnet::OptionList( name ) {}


std::string nnet::ObjectList::type() {
	return "ObjectList";
}


void nnet::ObjectList::write( xmlNodePtr to ) {
	OptionList::write( to );
	if( target_map.get() ) Serializable::Serialize( target_map, to );
}


void nnet::ObjectList::read( xmlNodePtr from ) {
	OptionList::read( from );
	
	xmlNodePtr child = from->children;
	while( child ) {
		if( child->type == XML_ELEMENT_NODE ) {
			SerializablePtr object( Serializable::Unserialize( child ) );
			if( object.get() ) {
				target_map = object;
				return;
			}
		}
		child = child->next;
	}
}


void nnet::ObjectList::setTarget( MapPtr target ) {
	target_map = target;
}


bool nnet::ObjectList::add( std::string item ) {
	if( OptionList::add( item ) ) {
		SerializablePtr object( Serializable::Create( item, item ) );
		target_map->add( object );
		return true;
	} else {
		return false;
	}
}


bool nnet::ObjectList::remove( std::string item ) {
	if( OptionList::remove( item ) ) {
		target_map->erase( item );
		return true;
	} else {
		return false;
	}
}


void nnet::ObjectList::clear() {
	OptionList::clear();
	target_map->clear();
}


nnet::SerializablePtr nnet::ObjectList::Create( std::string name ) {
	return RefCountPtr<ObjectList>( new ObjectList( name ) );
}

