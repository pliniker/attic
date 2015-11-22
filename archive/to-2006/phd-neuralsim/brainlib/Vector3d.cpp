

#include "Vector3d.h"



template<> std::string nnet::Char3d::type()   { return "Char3d"; }
template<> std::string nnet::Uchar3d::type()  { return "Uchar3d"; }
template<> std::string nnet::Short3d::type()  { return "Short3d"; }
template<> std::string nnet::Ushort3d::type() { return "Ushort3d"; }
template<> std::string nnet::Int3d::type()    { return "Int3d"; }
template<> std::string nnet::Uint3d::type()   { return "Uint3d"; }
template<> std::string nnet::Long3d::type()   { return "Long3d"; }
template<> std::string nnet::Ulong3d::type()  { return "Ulong3d"; }
template<> std::string nnet::Float3d::type()  { return "Float3d"; }
template<> std::string nnet::Double3d::type() { return "Double3d"; }


template<> void nnet::Float3d::write( xmlNodePtr to ) {
	double temp;
	temp = this_x;  XmlFile::WriteDouble( to, "x", temp );
	temp = this_y;  XmlFile::WriteDouble( to, "y", temp );
	temp = this_z;  XmlFile::WriteDouble( to, "z", temp );
}


template<> void nnet::Float3d::read( xmlNodePtr from ) {
	if( from ) {
		double temp;
		temp = XmlFile::ReadDouble( from, "x" );  this_x = temp;
		temp = XmlFile::ReadDouble( from, "y" );  this_y = temp;
		temp = XmlFile::ReadDouble( from, "z" );  this_z = temp;
	}
}


template<> void nnet::Double3d::write( xmlNodePtr to ) {
	XmlFile::WriteDouble( to, "x", this_x );
	XmlFile::WriteDouble( to, "y", this_y );
	XmlFile::WriteDouble( to, "z", this_z );
}


template<> void nnet::Double3d::read( xmlNodePtr from ) {
	if( from ) {			
		this_x = XmlFile::ReadDouble( from, "x" );
		this_y = XmlFile::ReadDouble( from, "y" );
		this_z = XmlFile::ReadDouble( from, "z" );
	}	
}


