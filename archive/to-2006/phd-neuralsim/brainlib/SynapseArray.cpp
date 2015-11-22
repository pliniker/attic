

#include "ErrorInterface.h"
#include "SynapseArray.h"



nnet::SynapseArray::SynapseArray( std::string name ) : 
    nnet::Map ( name )
{
    IntPtr size( new Int( "size" ) );
	size->setReadonly( true );
	add( size );
}


void nnet::SynapseArray::read( xmlNodePtr from ) 
{
	Map::read( from );
}
        

void nnet::SynapseArray::setDistance( size_t, int )
{
}


void nnet::SynapseArray::create( int array_size ) 
{
    IntPtr size( get( "size" ) );
	size->set( array_size );
}


bool nnet::SynapseArray::check() 
{
    IntPtr size( get( "size" ) );
    m_size = size->get();
	return true;
}


void nnet::SynapseArray::recall( size_t, Unit, Unit*, Unit* ) 
{
    LOG_UNIMPL
}


void nnet::SynapseArray::learn( size_t, Unit, Unit*, Unit ) 
{
    LOG_UNIMPL
}


void nnet::SynapseArray::reset() 
{
    LOG_UNIMPL
}

