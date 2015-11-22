#ifndef BRAINLIB_OPERATIONS_H
#define BRAINLIB_OPERATIONS_H


#include "CString.h"
#include "Boolean.h"
#include "Floating.h"
#include "Integer.h"
#include "OptionList.h"
#include "ObjectList.h"
#include "Image.h"
#include "Vector3d.h"
#include "Map.h"
#include "ErrorInterface.h"


namespace nnet {

	/// ( CString ) ///////////////////////////////////////////////////

	inline bool operator==(CStringPtr a, CStringPtr b) { 
        return a->get() == b->get(); 
    }
	
	/// ( Boolean ) ///////////////////////////////////////////////////
	
	inline bool operator==(BooleanPtr a, BooleanPtr b) { 
        return a->get() == b->get(); 
    }

	/// ( Integer<T> ) ////////////////////////////////////////////////

	template<class A, class B>
	inline bool operator==(RefCountPtr<Integer<A> > a, 
						   RefCountPtr<Integer<B> > b) { 
		return a->get() == b->get(); 
	}
	
	template<class A, class T>
	inline bool operator==(RefCountPtr<Integer<A> > a,
						   T b) {
		return a->get() == b;
	}
	
	template<class A, class B>
	inline long operator+(RefCountPtr<Integer<A> > a,
						  RefCountPtr<Integer<B> > b) {
		return a->get() + b->get();
	}
	
	template<class A, class B>
	inline long operator-(RefCountPtr<Integer<A> > a,
						  RefCountPtr<Integer<B> > b) {
		return a->get() - b->get();
	}
	
	/// ( Floating<T> ) ///////////////////////////////////////////////
	
	template<class A, class B>
	inline bool operator==(RefCountPtr<Floating<A> > a, 
						   RefCountPtr<Floating<B> > b) {
		return a->get() == b->get(); 
	}
	
	template<class A, class T>
	inline bool operator==(RefCountPtr<Floating<A> > a,
						   T b) {
		return a->get() == b;
	}
	
	template<class A, class B>
	inline long operator-(RefCountPtr<Floating<A> > a,
						  RefCountPtr<Floating<B> > b) {
		return a->get() - b->get();
	}
	
	template<class A, class B>
	inline double operator+(RefCountPtr<Floating<A> > a,
							RefCountPtr<Floating<B> > b) {
		return a->get() + b->get();
	}
		
	/// ( OptionList ) ////////////////////////////////////////////////
	
	inline bool operator+=(OptionListPtr a, std::string& b) {
		return a->add(b);
	}
	
	inline bool operator-=(OptionListPtr a, std::string& b) {
		return a->remove(b);
	}

	/// ( Vector3d<T> ) ///////////////////////////////////////////////
	
	template<class T>
	inline bool operator==(RefCountPtr<Vector3d<T> > a, 
						   RefCountPtr<Vector3d<T> > b) {
		return ( (a->x() == b->x()) && 
                 (a->y() == b->y()) && 
                 (a->z() == b->z()) );
	}
	
	template<class T>
	inline bool operator!=(RefCountPtr<Vector3d<T> > a, 
						   RefCountPtr<Vector3d<T> > b) {
		return ( (a->x() != b->x()) || 
                 (a->y() != b->y()) || 
                 (a->z() != b->z()) );
	}
	
	template<class A, class B>
	inline void operator+=(RefCountPtr<Vector3d<A> > a,
						   RefCountPtr<Vector3d<B> > b) {
		a->x( a->x() + b->x() );
		a->y( a->y() + b->y() );
		a->z( a->z() + b->z() );
	}

	template<class A, class B>
	inline void operator-=(RefCountPtr<Vector3d<A> > a,
						   RefCountPtr<Vector3d<B> > b) {
		a->x( a->x() - b->x() );
		a->y( a->y() - b->y() );
		a->z( a->z() - b->z() );
	}
	
	template<class A, class B>
	inline void operator/=(RefCountPtr<Vector3d<A> > a,
						   RefCountPtr<Vector3d<B> > b) {
		a->x( a->x() / b->x() );
		a->y( a->y() / b->y() );
		a->z( a->z() / b->z() );
	}	
	
	template<class A, class B>
	inline void operator*=(RefCountPtr<Vector3d<A> > a,
						   RefCountPtr<Vector3d<B> > b) {
		a->x( a->x() * b->x() );
		a->y( a->y() * b->y() );
		a->z( a->z() * b->z() );
	}	
	
	template<class A>
	inline void operator/=(RefCountPtr<Vector3d<A> > a,
						   A b) {
		a->x( a->x() / b );
		a->y( a->y() / b );
		a->z( a->z() / b );
	}
	
	template<class A>
	inline void operator*=(RefCountPtr<Vector3d<A> > a,
						   A b) {
		a->x( a->x() * b );
		a->y( a->y() * b );
		a->z( a->z() * b );
	}
	
	/// ( Vector3d<T>, Image ) ////////////////////////////////////////
	
	template<class T>
	inline bool eqDim(RefCountPtr<Vector3d<T> > a,
					  ImagePtr b) {
		return ( (a->x() == (T)b->x()) && (a->y() == (T)b->y()) );
	}
	
	template<class T>
	inline bool eqDimPix(RefCountPtr<Vector3d<T> > a,
					  ImagePtr b) {
		return ( (a->x() == (T)b->xPixels()) && (a->y() == (T)b->y()) );
	}
	
	template<class T>
	inline bool neqDim(RefCountPtr<Vector3d<T> > a,
					   ImagePtr b) {
		return ( (a->x() != (T)b->x()) || (a->y() != (T)b->y()) );
	}

	template<class T>
	inline bool neqDimPix(RefCountPtr<Vector3d<T> > a,
					   ImagePtr b) {
		return ( (a->x() != (T)b->xPixels()) || (a->y() != (T)b->y()) );
	}
	
	/// ( Generic ) ///////////////////////////////////////////////////
	
	inline bool equivalent(SerializablePtr a, SerializablePtr b) { 
        return a.get() == b.get(); 
    }
	
	
}
	

#endif
