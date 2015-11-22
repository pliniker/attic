

#include <sstream>
#include "ErrorInterface.h"
#include "XmlFile.h"



long nnet::XmlFile::ReadLong( xmlNodePtr node, std::string name ) {
	long value;
	std::string value_string = GetString( node, name );
	std::istringstream stream_value( value_string );
	stream_value >> value;
	return value;
}


double nnet::XmlFile::ReadDouble( xmlNodePtr node, std::string name ) {
	double value;
	std::string value_string = GetString( node, name );
	std::istringstream stream_value( value_string );
	stream_value >> value;
	return value;
}


bool nnet::XmlFile::ReadBoolean( xmlNodePtr node, std::string name ) {
	std::string value_string = GetString( node, name );
	return ( value_string == "true" );
}


std::string nnet::XmlFile::ReadString( xmlNodePtr node, std::string name ) {
	return GetString( node, name );	
}


void nnet::XmlFile::WriteLong( xmlNodePtr node, std::string name, long value ) {
	std::ostringstream value_stream;
	value_stream << value;
	NewString( node, name, value_stream.str().c_str() );
}


void nnet::XmlFile::WriteDouble( xmlNodePtr node, std::string name, double value ) {
	std::ostringstream value_stream;
	value_stream << value;
	NewString( node, name, value_stream.str().c_str() );
}


void nnet::XmlFile::WriteBoolean( xmlNodePtr node, std::string name, bool value ) {
	std::string value_string = value ? "true" : "false";
	NewString( node, name, value_string );
}


void nnet::XmlFile::WriteString( xmlNodePtr node, std::string name, std::string value ) {
	NewString( node, name, value );
}


void nnet::XmlFile::NewString( xmlNodePtr node, std::string name, std::string value ) {
	xmlNewTextChild( node, NULL, ( xmlChar* )name.c_str(), ( xmlChar* )value.c_str() );
}


std::string nnet::XmlFile::GetString( xmlNodePtr node, std::string name ) {
	std::string value;
	xmlNodePtr child = node->children;
	while( child ) {
		
		if( !xmlStrcmp( child->name, ( xmlChar* )name.c_str() ) ) {
			
			xmlChar* data = xmlNodeListGetString( child->doc, child->children, 1 );
			std::string string_value;
			if( data ) string_value = ( char* )data;
			xmlFree( data );
			
			return string_value;
		}
		
		child = child->next;
	}
	
	return "";	
}

