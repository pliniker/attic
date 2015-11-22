#ifndef BRAINLIB_FLOATING_H
#define BRAINLIB_FLOATING_H


#include "XmlFile.h"
#include "Serializable.h"


namespace nnet {
	
	
	/// Serializable Floating type
	template<typename T>
	class Floating : public Serializable {
		/// Object methods
	public:
		Floating( std::string name );
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
	
	typedef Floating<float>   Float;
	typedef Floating<double>  Double;
	
	typedef RefCountPtr<Float>   FloatPtr;
	typedef RefCountPtr<Double>  DoublePtr;
	
	template<> std::string Float::type();
	template<> std::string Double::type();
	
	
	/// Template implementations
	
	template<typename T>
	Floating<T>::Floating( std::string name ) : 
	this_name( name ) {
		this_data = 0.0;
		this_min = 0.0;
		this_max = 1.0;
		this_step = 0.1;
	}
	
	
	template<typename T>
	std::string Floating<T>::name() {
		return this_name;
	}
	
	
	template<typename T>
	void Floating<T>::write( xmlNodePtr to ) {
		XmlFile::WriteDouble( to, "value", ( double )this_data );
		XmlFile::WriteDouble( to, "min", ( double )this_min );
		XmlFile::WriteDouble( to, "max", ( double )this_max );
		XmlFile::WriteDouble( to, "step", ( double )this_step );
	}
	
	
	template<typename T>
	void Floating<T>::read( xmlNodePtr from ) {
		if ( from ) {
			this_data = XmlFile::ReadDouble( from, "value" );
			this_min =  XmlFile::ReadDouble( from, "min" );
			this_max =  XmlFile::ReadDouble( from, "max" );
			this_step = XmlFile::ReadDouble( from, "step" );
		}
	}
	
	
	template<typename T>
	void Floating<T>::set( T new_value ) {
		this_data = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Floating<T>::setMin( T new_value ) {
		this_min = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Floating<T>::setMax( T new_value ) {
		this_max = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	void Floating<T>::setStep( T new_value ) {
		this_step = new_value;
		setDirty( true );
	}
	
	
	template<typename T>
	T Floating<T>::get() {
		return this_data;
	}
	
	
	template<typename T>
	T Floating<T>::getMin() {
		return this_min;
	}
	
	
	template<typename T>
	T Floating<T>::getMax() {
		return this_max;
	}
	
	
	template<typename T>
	T Floating<T>::getStep() {
		return this_step;
	}
	
	
	template<typename T>
	SerializablePtr Floating<T>::Create( std::string name ) {
		return RefCountPtr<Floating<T> >( new Floating<T>( name ) );
	}
	
	
}


#endif

