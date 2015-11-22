#ifndef BRAINLIB_ARRAY_H
#define BRAINLIB_ARRAY_H


#include <vector>
#include "XmlFile.h"
#include "BinaryFile.h"
#include "Serializable.h"


namespace nnet {
	
	
	/// Serializable Array type
	/// encapsulates std::vector and allows only the bounds-checking
	/// at() function to access the vector
	template<typename T>
	class Array : public Serializable {
		/// Object methods
	public:
		Array( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );	
		virtual void read( xmlNodePtr from );
		
		void        create( int size );
		int         size();
		T&          at( int index );   // bounds-checked access
		
	private:
		void commonWrite( xmlNodePtr to );
		void commonRead( xmlNodePtr from );
		
	private:
		std::string    m_name;
		std::vector<T> m_data;
		int            m_size;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Specialized definitions
	
	typedef Array<char>           CharArray;
	typedef Array<unsigned char>  UcharArray;
	typedef Array<short>          ShortArray;
	typedef Array<unsigned short> UshortArray;
	typedef Array<int>            IntArray;
	typedef Array<unsigned int>   UintArray;
	typedef Array<long>           LongArray;
	typedef Array<unsigned long>  UlongArray;
	typedef Array<float>          FloatArray;
	typedef Array<double>         DoubleArray;
	
	typedef RefCountPtr<CharArray>   CharArrayPtr;
	typedef RefCountPtr<UcharArray>  UcharArrayPtr;
	typedef RefCountPtr<ShortArray>  ShortArrayPtr;
	typedef RefCountPtr<UshortArray> UshortArrayPtr;
	typedef RefCountPtr<IntArray>    IntArrayPtr;
	typedef RefCountPtr<UintArray>   UintArrayPtr;
	typedef RefCountPtr<LongArray>   LongArrayPtr;
	typedef RefCountPtr<UlongArray>  UlongArrayPtr;
	typedef RefCountPtr<FloatArray>  FloatArrayPtr;
	typedef RefCountPtr<DoubleArray> DoubleArrayPtr;
	
	template<> std::string CharArray::type();
	template<> std::string UcharArray::type();
	template<> std::string ShortArray::type();
	template<> std::string UshortArray::type();
	template<> std::string IntArray::type();
	template<> std::string UintArray::type();
	template<> std::string LongArray::type();
	template<> std::string UlongArray::type();
	template<> std::string FloatArray::type();
	template<> std::string DoubleArray::type();
	
	template<> void CharArray::write( xmlNodePtr to );
	template<> void UcharArray::write( xmlNodePtr to );
	template<> void ShortArray::write( xmlNodePtr to );
	template<> void UshortArray::write( xmlNodePtr to );
	template<> void IntArray::write( xmlNodePtr to );
	template<> void UintArray::write( xmlNodePtr to );
	template<> void LongArray::write( xmlNodePtr to );
	template<> void UlongArray::write( xmlNodePtr to );
	template<> void FloatArray::write( xmlNodePtr to );
	template<> void DoubleArray::write( xmlNodePtr to );
	
	template<> void CharArray::read( xmlNodePtr from );
	template<> void UcharArray::read( xmlNodePtr from );
	template<> void ShortArray::read( xmlNodePtr from );
	template<> void UshortArray::read( xmlNodePtr from );
	template<> void IntArray::read( xmlNodePtr from );
	template<> void UintArray::read( xmlNodePtr from );
	template<> void LongArray::read( xmlNodePtr from );
	template<> void UlongArray::read( xmlNodePtr from );
	template<> void FloatArray::read( xmlNodePtr from );
	template<> void DoubleArray::read( xmlNodePtr from );
	
	
	/// Template implementations
	
	template<typename T>
	Array<T>::Array( std::string name ) : 
	m_name( name ) {
		create( 16 );
	}
	
	
	template<typename T>
	std::string Array<T>::name() {
		return m_name;
	}
	
	
	template<typename T>
	void Array<T>::create( int size ) {
		m_data.reserve( size );          
		// chop existing physical allocation (see gotw #54)
		std::vector<T>( m_data ).swap( m_data );
		m_data.resize( size );
		m_size = size;
	}

	
	template<typename T>
	int Array<T>::size() {
		return m_size;
	}
	
	
	template<typename T>
	inline T& Array<T>::at( int index ) {
		return m_data.at( index );       // allow only bounds-checked access
	}
	
	
	// Default implementation for user defined Types
	template<typename T>
	void Array<T>::write( xmlNodePtr to ) {
		commonWrite( to );
		RefCountPtr<char> data( m_data );
		BinaryFile::WriteChar( (char*)&m_data[0], sizeof(T) * m_size );
	}
	
	
	// Default implementation for user defined Types
	template<typename T>
	void Array<T>::read( xmlNodePtr from ) {
		commonRead( from );
		RefCountPtr<char> data( m_data );
		BinaryFile::ReadChar( (char*)&m_data[0], sizeof(T) * m_size );
	}
	
	
	template<typename T>
	void Array<T>::commonWrite( xmlNodePtr to ) {
		XmlFile::WriteLong( to, "size", m_size );
		XmlFile::WriteLong( to, "binpos", BinaryFile::GetPos() );
	}
	
	
	template<typename T>
	void Array<T>::commonRead( xmlNodePtr from ) {
		if( from ) {
            m_size = XmlFile::ReadLong( from, "binpos" );
			BinaryFile::SetPos( m_size );
            m_size = XmlFile::ReadLong( from, "size" );
			create( m_size );
		}
	}
	
	
	template<typename T>
	SerializablePtr Array<T>::Create( std::string name ) {
		return RefCountPtr<Array<T> >( new Array<T>( name ) );
	}
	
	
}


#endif

