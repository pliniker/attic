

#include "Array.h"



template<> std::string nnet::CharArray::type()   { return "CharArray"; }
template<> std::string nnet::UcharArray::type()  { return "UcharArray"; }
template<> std::string nnet::ShortArray::type()  { return "ShortArray"; }
template<> std::string nnet::UshortArray::type() { return "UshortArray"; }
template<> std::string nnet::IntArray::type()    { return "IntArray"; }
template<> std::string nnet::UintArray::type()   { return "UintArray"; }
template<> std::string nnet::LongArray::type()   { return "LongArray"; }
template<> std::string nnet::UlongArray::type()  { return "UlongArray"; }
template<> std::string nnet::FloatArray::type()  { return "FloatArray"; }
template<> std::string nnet::DoubleArray::type() { return "DoubleArray"; }


template<> void nnet::CharArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteChar( &m_data[0], m_size );
}


template<> void nnet::UcharArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteChar( ( char* )&m_data[0], m_size );
}


template<> void nnet::ShortArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteShort( &m_data[0], m_size );
}


template<> void nnet::UshortArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteShort( ( short* )&m_data[0], m_size );
}


template<> void nnet::IntArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteInt( &m_data[0], m_size );
}


template<> void nnet::UintArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteInt( ( int* )&m_data[0], m_size );
}


template<> void nnet::LongArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteLong( &m_data[0], m_size );
}


template<> void nnet::UlongArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteLong( ( long* )&m_data[0], m_size );
}


template<> void nnet::FloatArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteFloat( &m_data[0], m_size );
}


template<> void nnet::DoubleArray::write( xmlNodePtr to ) {
	commonWrite( to );
	BinaryFile::WriteDouble( &m_data[0], m_size );
}


template<> void nnet::CharArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadChar( &m_data[0], m_size );
}


template<> void nnet::UcharArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadChar( ( char* )&m_data[0], m_size );
}


template<> void nnet::ShortArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadShort( &m_data[0], m_size );
}


template<> void nnet::UshortArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadShort( ( short* )&m_data[0], m_size );
}


template<> void nnet::IntArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadInt( &m_data[0], m_size );
}


template<> void nnet::UintArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadInt( ( int* )&m_data[0], m_size );
}


template<> void nnet::LongArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadLong( &m_data[0], m_size );
}


template<> void nnet::UlongArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadLong( ( long* )&m_data[0], m_size );
}


template<> void nnet::FloatArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadFloat( &m_data[0], m_size );
}


template<> void nnet::DoubleArray::read( xmlNodePtr from ) {
	commonRead( from );
	BinaryFile::ReadDouble( &m_data[0], m_size );
}


