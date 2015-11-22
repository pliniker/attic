

#include <iostream>
#include "ErrorInterface.h"


static nnet::ErrorInterfaceBase* error_interface;


nnet::ErrorInterfaceBase::~ErrorInterfaceBase() {}


void nnet::ErrorInterface::alert( std::string a ) {
	std::cerr << a << std::endl;
}


void nnet::ErrorInterface::log( std::string l ) {
	std::cerr << l << std::endl;
}


bool nnet::ErrorInterface::ask( std::string q ) {
	std::cerr << q << "... no" << std::endl;
	return false;
}


void nnet::ErrorInterface::clear() {}


void nnet::error::set_error_interface( nnet::ErrorInterfaceBase* iface ) {
	if( ::error_interface ) delete error_interface;
	::error_interface = iface;
}


void nnet::error::alert( std::string a ) {
	::error_interface->alert( a );
}


void nnet::error::log( std::string e ) {
	::error_interface->log( e );
}


bool nnet::error::ask( std::string q ) {
    return ::error_interface->ask( q );
}


void nnet::error::clear() {
	::error_interface->clear();
}


void nnet::error::std_exception( std::string in, const char* what ) {
	std::string msg = in + ": " + what;
	log( msg );
	alert( msg );
}


std::string const nnet::error::NewObjectFailed = "Exception thrown in creating a new object.\nIf this persists with the same object type there is probably a bug in the respective plugin code.";
std::string const nnet::error::DeleteObjectFailed = "Exception thrown in deleting an object.\nThere is probably an error in a destructor somewhere down the line.\n";
std::string const nnet::error::CheckObjectFailed = "Exception thrown in verifying a layer.\nIf this persists there is probably a bug in the respective plugin code.\nYou may want to delete the layer.";
std::string const nnet::error::RecallObjectFailed = "Exception thrown in recalling a layer.\nIf this persists there is probably a bug in the respective plugin code.\nYou may want to delete the layer.";
std::string const nnet::error::LearnObjectFailed = "Exception thrown in training a layer.\nIf this persists there is probably a bug in the respective plugin code.\nYou may want to delete the layer.";
std::string const nnet::error::ResetObjectFailed = "Exception thrown in resetting a layer.\nIf this persists there is probably a bug in the respective plugin code.\nYou may want to delete the layer.";
std::string const nnet::error::UnserializeFailed = "Exception thrown in loading from disk.\nThis may leave the application in an unpredictable state: the application will quit.";


