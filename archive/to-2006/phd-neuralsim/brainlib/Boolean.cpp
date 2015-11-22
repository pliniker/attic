

#include "XmlFile.h"
#include "Boolean.h"




/// Template implementations

nnet::Boolean::Boolean( std::string name ) : 
m_name( name ) {
	m_data = false;
}


std::string nnet::Boolean::name() {
	return m_name;
}


std::string nnet::Boolean::type() {
	return "Boolean";
}


void nnet::Boolean::write( xmlNodePtr to ) {
	XmlFile::WriteBoolean( to, "value", m_data );
}


void nnet::Boolean::read( xmlNodePtr from ) {
	if( from ) {
		m_data = XmlFile::ReadBoolean( from, "value" );
	}
}


void nnet::Boolean::set( bool new_value ) {
	m_data = new_value;
	setDirty( true );
}


bool nnet::Boolean::get() {
	return m_data;
}


nnet::SerializablePtr nnet::Boolean::Create( std::string name ) {
	return RefCountPtr<Boolean>( new Boolean( name ) );
}

