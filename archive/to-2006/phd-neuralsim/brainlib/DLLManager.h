#ifndef BRAINLIB_DLLMANAGER_H
#define BRAINLIB_DLLMANAGER_H


#include <string>
#include <dlfcn.h>


namespace nnet {

	
	// plugin C initialiser function
	typedef void ( *obfact )( void );
	

    class DLLManager {
		/// Object methods
    public:
        DLLManager( std::string fname );
        ~DLLManager();

        bool makeFactory();

		/// Object data
    private:
		std::string name;
		obfact      makefact;
        void*       library;
        const char* error;
    };


} // end namespace


#endif
