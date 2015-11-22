
#include "DLLManager.h"
#include "ErrorInterface.h"


nnet::DLLManager::DLLManager( std::string fname ) {
	name = fname;
}


nnet::DLLManager::~DLLManager() {
    if( library ) dlclose( library );
}


bool nnet::DLLManager::makeFactory() {
    library = dlopen( name.c_str(), RTLD_NOW );
    error = dlerror();
	
    if( library == 0 ) {
        std::string err = "DLLManager: ";
        err += error;
		error::log( err );
		return false;
	}
	
    makefact = ( obfact )dlsym( library, "Initialize" );
    error = dlerror();
	
    if( error ) {
        std::string err = "DLLManager: ";
        err += error;
		error::log( err );
		return false;
    }
	
	makefact();
	
	return true;
}
