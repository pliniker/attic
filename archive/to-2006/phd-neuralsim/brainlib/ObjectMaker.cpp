

#include "Map.h"
#include "ObjectMaker.h"



nnet::ObjectMaker::ObjectMaker( std::string name ) :
nnet::OptionList( name ) {}


std::string nnet::ObjectMaker::type() {
    return "ObjectMaker";
}


void nnet::ObjectMaker::write( xmlNodePtr to ) {
	OptionList::write( to );
	if ( target_map.get() ) Serializable::Serialize( target_map, to );
}


void nnet::ObjectMaker::read( xmlNodePtr from ) {
	OptionList::read( from );
	
	xmlNodePtr child = from->children;
	while ( child ) {
		if ( child->type == XML_ELEMENT_NODE ) {
			SerializablePtr object( Serializable::Unserialize( child ) );
			if ( object.get() ) {
				target_map = object;
				return;
			}
		}
		child = child->next;
	}
}


void nnet::ObjectMaker::setTarget( MapPtr target ) {
    target_map = target;
}
    

bool nnet::ObjectMaker::add( std::string item ) {
	if ( OptionList::add( item ) ) {
		target_map->add( Serializable::Create( choice_name, item ) );
		return true;
	} else {
		return false;
	}
}


bool nnet::ObjectMaker::remove( std::string item ) {
	if ( OptionList::remove( item ) ) {
		target_map->erase( item );
		return true;
	} else {
		return false;
	}
}


void nnet::ObjectMaker::clear() {
	OptionList::clear();
    target_map->clear();
}
    

nnet::SerializablePtr nnet::ObjectMaker::Create( std::string name ) {
    return RefCountPtr<ObjectMaker>( new ObjectMaker( name ) );
}

