#ifndef BRAINLIB_VECTOR3D_H
#define BRAINLIB_VECTOR3D_H


#include "XmlFile.h"
#include "Serializable.h"
#include "ErrorInterface.h"


namespace nnet {
	
	
	/// Serializable Vector3d type
	template<typename T>
	class Vector3d : public Serializable {
		/// Object methods
	public:
		Vector3d( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );	
		virtual void read( xmlNodePtr from );
		
		void set( T i, T j );
		void set( T i, T j, T k );
		void set( RefCountPtr<Vector3d<T> > vec );
		
		T&   x();
		T&   y();
		T&   z();
		void x( T n );
		void y( T n );
		void z( T n );
		
		/// Object data
	private:
		std::string this_name;
		T this_x, this_y, this_z;
		
		/// Class methods
	public:			
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Specialized definitions
	
	typedef Vector3d<char>           Char3d;
	typedef Vector3d<unsigned char>  Uchar3d;
	typedef Vector3d<short>          Short3d;
	typedef Vector3d<unsigned short> Ushort3d;
	typedef Vector3d<int>            Int3d;
	typedef Vector3d<unsigned int>   Uint3d;
	typedef Vector3d<long>           Long3d;
	typedef Vector3d<unsigned long>  Ulong3d;
	typedef Vector3d<float>          Float3d;
	typedef Vector3d<double>         Double3d;
	
	typedef RefCountPtr<Char3d>   Char3dPtr;
	typedef RefCountPtr<Uchar3d>  Uchar3dPtr;
	typedef RefCountPtr<Short3d>  Short3dPtr;
	typedef RefCountPtr<Ushort3d> Ushort3dPtr;
	typedef RefCountPtr<Int3d>    Int3dPtr;
	typedef RefCountPtr<Uint3d>   Uint3dPtr;
	typedef RefCountPtr<Long3d>   Long3dPtr;
	typedef RefCountPtr<Ulong3d>  Ulong3dPtr;
	typedef RefCountPtr<Float3d>  Float3dPtr;
	typedef RefCountPtr<Double3d> Double3dPtr;
	
	template<> std::string Char3d::type();
	template<> std::string Uchar3d::type();
	template<> std::string Short3d::type();
	template<> std::string Ushort3d::type();
	template<> std::string Int3d::type();
	template<> std::string Uint3d::type();
	template<> std::string Long3d::type();
	template<> std::string Ulong3d::type();
	template<> std::string Float3d::type();
	template<> std::string Double3d::type();
	
	template<> void Float3d::write( xmlNodePtr to );
	template<> void Double3d::write( xmlNodePtr to );
	
	template<> void Float3d::read( xmlNodePtr from );
	template<> void Double3d::read( xmlNodePtr from );
	
	
	/// Template implementations

	template<typename T>
	Vector3d<T>::Vector3d( std::string name ) : 
	this_name( name ) {
		this_x = this_y = this_z = 0;
	}
	
	
	template<typename T>
	std::string Vector3d<T>::name() {
		return this_name;
	}
	
	
	template<typename T>
	void Vector3d<T>::write( xmlNodePtr to ) {
		XmlFile::WriteLong( to, "x", this_x );
		XmlFile::WriteLong( to, "y", this_y );
		XmlFile::WriteLong( to, "z", this_z );
	}
	
	
	template<typename T>
	void Vector3d<T>::read( xmlNodePtr from ) {
		if( from ) {
			this_x = XmlFile::ReadLong( from, "x" );
			this_y = XmlFile::ReadLong( from, "y" );
			this_z = XmlFile::ReadLong( from, "z" );
		}
	}

	
	template<typename T>
	void Vector3d<T>::set( T i, T j ) {
		this_x = i;
		this_y = j;
		setDirty( true );
	}
	
	
	template<typename T>
	void Vector3d<T>::set( T i, T j, T k ) {
		this_x = i;
		this_y = j;
		this_z = k;
		setDirty( true );
	}
	
	
	template<typename T>
	void Vector3d<T>::set( RefCountPtr<Vector3d<T> > vec ) {
		this_x = vec->x();
		this_y = vec->y();
		this_z = vec->z();
		setDirty( true );
	}
	
	
	template<typename T>
	T& Vector3d<T>::x() { return this_x; }
	
	template<typename T>
	T& Vector3d<T>::y() { return this_y; }

	template<typename T>
	T& Vector3d<T>::z() { return this_z; }

	template<typename T>
	void Vector3d<T>::x( T n ) { this_x = n; setDirty( true ); }

	template<typename T>
	void Vector3d<T>::y( T n ) { this_y = n; setDirty( true ); }

	template<typename T>
	void Vector3d<T>::z( T n ) { this_z = n; setDirty( true ); }

	
	template<typename T>
	SerializablePtr Vector3d<T>::Create( std::string name ) {
		return RefCountPtr<Vector3d<T> >( new Vector3d<T>( name ) );
	}
	
	
}


#endif

