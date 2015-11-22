

#include "XmlFile.h"
#include "CString.h"


/// Template implementations

nnet::CString::CString( std::string name ) : 
this_name( name ) {
	this_data = "";
}


std::string nnet::CString::name() {
	return this_name;
}


std::string nnet::CString::type() {
	return "CString";
}


void nnet::CString::write( xmlNodePtr to ) {
	XmlFile::WriteString( to, "value", this_data );
}


void nnet::CString::read( xmlNodePtr from ) {
	if( from ) {
		this_data = XmlFile::ReadString( from, "value" );
	}
}


void nnet::CString::set( std::string new_value ) {
	this_data = new_value;
	setDirty( true );
}


std::string& nnet::CString::get() {
	return this_data;
}


nnet::SerializablePtr nnet::CString::Create( std::string name ) {
	return RefCountPtr<CString>( new CString( name ) );
}

