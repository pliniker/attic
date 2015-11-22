#ifndef BRAINLIB_INTEGER_H
#define BRAINLIB_INTEGER_H


#include "Common.h"
#include "XmlFile.h"
#include "Serializable.h"


namespace nnet {
	
	
	/// Serializable Integer type
	template<typename T>
	class Integer : public Serializable {
		/// Object methods
	public:
		Integer( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );	
		virtual void read( xmlNodePtr from );
		
		void set( T new_value );
		void setMin( T new_value );
		void setMax( T new_value );
		void setStep( T new_value );
		
		T get();
		T getMin();
		T getMax();
		T getStep();
		
		/// Object data
	private:
		std::string this_name;
		T this_data;
		T this_min;
		T this_max;
		T this_step;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Specialized definitions
	
	typedef Integer<char>           Char;
	typedef Integer<unsigned char>  Uchar;
	typedef Integer<short>          Short;
	typedef Integer<unsigned short> Ushort;
	typedef Integer<int>            Int;
	typedef Integer<unsigned int>   Uint;
	typedef Integer<long>           Long;
	typedef Integer<unsigned long>  Ulong;
	
	typedef RefCountPtr<Char>   CharPtr;
	typedef RefCountPtr<Uchar>  UcharPtr;
	typedef RefCountPtr<Short>  ShortPtr;
	typedef RefCountPtr<Ushort> UshortPtr;
	typedef RefCountPtr<Int>    IntPtr;
	typedef RefCountPtr<Uint>   UintPtr;
	typedef RefCountPtr<Long>   LongPtr;
	typedef RefCountPtr<Ulong>  UlongPtr;
	
	template<> std::string Char::type();
	template<> std::string Uchar::type();
	template<> std::string Short::type();
	template<> std::string Ushort::type();
	template<> std::string Int::type();
	template<> std::string Uint::type();
	template<> std::string Long::type();
	template<> std::string Ulong::type();
	
	
	/// Template implementations
	
	template<typename T>
	Integer<T>::Integer( std::string name ) : 
	this_name( name ) {
		this_data = 0;
		this_min = 0;
		this_max = Common::IntMax;
		this_step = 1;
	}
	
	
	template<typename T>
	std::string Integer<T>::name() {
		return this_name;
	}
	
	
	template<typename T>
	void Integer<T>::write( xmlNodePtr to ) {
		XmlFile::WriteLong( to, "value", this_data );
		XmlFile::WriteLong( to, "min", this_min );
		XmlFile::WriteLong( to, "max", this_max );
		XmlFile::WriteLong( to, "step", this_step );
	}
	
	
	template<typename T>
	void Integer<T>::read( xmlNodePtr from ) {
		if( from ) {			
			this_data = XmlFile::ReadLong( from, "value" );
			this_min =  XmlFile::ReadLong( from, "min" );
			this_max =  XmlFile::ReadLong( from, "max" );
			this_step = XmlFile::ReadLong( from, "step" );
		}
	}
	
	
	template<typename T>
	void Integer<T>::set( T new_value ) {
		this_data = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Integer<T>::setMin( T new_value ) {
		this_min = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Integer<T>::setMax( T new_value ) {
		this_max = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Integer<T>::setStep( T new_value ) {
		this_step = new_value;
		setDirty( true );
	}

	
	template<typename T>
	T Integer<T>::get() {
		return this_data;
	}
	
	
	template<typename T>
	T Integer<T>::getMin() {
		return this_min;
	}
	
	
	template<typename T>
	T Integer<T>::getMax() {
		return this_max;
	}
	
	
	template<typename T>
	T Integer<T>::getStep() {
		return this_step;
	}
	
	
	template<typename T>
	SerializablePtr Integer<T>::Create( std::string name ) {
		return RefCountPtr<Integer<T> >( new Integer<T>( name ) );
	}
	
	
}


#endif

