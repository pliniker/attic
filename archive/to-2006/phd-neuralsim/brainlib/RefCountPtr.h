#ifndef BRAINLIB_REFCOUNTPTR_H
#define BRAINLIB_REFCOUNTPTR_H


// Reference-counting smart pointer.
//
// Cyclic references
// cause memory leaks and may cause strange
// things to happen.
//
// @TODO create "weak pointer" to avoid known cyclics.


#include <assert.h>


namespace nnet {
	
	
    // Pointer container. Manages it's own deleting
    // for virtual destructor purposes.
    template<typename T>
    class PtrContainer {
	public:
        PtrContainer( T* t ) :
        count( 1 ), object( t ) {}
		
        ~PtrContainer() {
			if( object ) {
				delete object;
			}
		}
		
        int count;
        T* object;
    };
	
	
    // Reference-counting pointer, based on some ref-counting
	// pointer code in an old boost version. 
	// This pointer should be type safe. 
	// Pointer copying ( copy constructor and = operator )
    // functions throw std::bad_cast on invalid type casting.
    //
	// See http://www.boost.org/libs/smart_ptr/smart_ptr.htm
	// In fact why not just use latest boost libraries? 
	// boost::shared_ptr<> looks like an almost shoe-in for RefCountPtr
    template<typename T>
	class RefCountPtr {
		// Object methods
	public:
		typedef T element_type;
		
		// Default null initializer
		RefCountPtr() :
		pointer( 0 ) {}
		
		// Single object instantiation
		explicit RefCountPtr( T* t ) :
		pointer( 0 ) {
			if( t ) pointer = new PtrContainer<T>( t );
		}

		// Object copy constructor for same type
		RefCountPtr( const RefCountPtr& p ) throw() {
			acquire( p.give() );
		}
		
		// Object copy constructor for different type
		// type U must be a base of or == the original T
		template<typename U>
		RefCountPtr( const RefCountPtr<U>& u ) throw( std::bad_cast ) {
			if( dynamic_cast<T*>( u.get() ) )
                acquire( ( PtrContainer<T>* )u.give() );
            else
                throw std::bad_cast();
		}
		
		// Decrease count by 1
		~RefCountPtr() {
			release();
		}
		
		// = Op for same type
		RefCountPtr& operator=( const RefCountPtr& p ) {
			if( this != &p ) {
				release();
				acquire( p.give() );
			}
			return *this;
		}
		
		// = Op for different type
		// type U must be a base of or == the original T
		template<typename U>
		RefCountPtr<T> &operator=( const RefCountPtr<U>& u ) throw( std::bad_cast ) {
			if ( ( void* )this != ( void* )&u ) {
                // check that object contained by u is a valid type of T
                if( dynamic_cast<T*>( u.get() ) ) {
                    release();
                    acquire( ( PtrContainer<T>* )u.give() );
                } else {
                    throw std::bad_cast();
                }
			}
			return *this;
		}
		
		// Dereference
		T& operator*() const throw() {
            assert( pointer != 0 );
            assert( pointer->object != 0 );
			return *pointer->object;
		}
		
		// Point to member
		T* operator->() const throw() {
            assert( pointer != 0 );
            assert( pointer->object != 0 );
			return pointer->object;
		}
		
		// Object pointer value
		T* get() const throw() {
			return pointer ? pointer->object : 0;
		}
		
		// Is there only 1 reference to the object and this is it?
		bool unique() const throw() {
			return ( pointer ? pointer->count == 1 : true ) ;
		}
		
		PtrContainer<T>* give() const {
			return pointer;
		}
		
		
	private:        
		void acquire( PtrContainer<T>* p ) throw() {
			pointer = p;
			if ( pointer ) ++pointer->count;
		}
		
		void release() {
			if( pointer ) {
				if( --pointer->count == 0 ) {
					delete pointer;
				}
				pointer = 0;
			}
		}
		
		// Object data
	private:
		PtrContainer<T>* pointer;
	};
	
	
}

#endif
