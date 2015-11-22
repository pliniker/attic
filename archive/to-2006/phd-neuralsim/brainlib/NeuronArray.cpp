

#include "ErrorInterface.h"
#include "NeuronArray.h"



nnet::NeuronArray::NeuronArray( std::string name ) : 
    Map( name )
{
    IntPtr inputs( new Int( "inputs" ) );
    IntPtr size( new Int( "size" ) );

    inputs->setReadonly( true );
	size->setReadonly( true );

	add( inputs );
	add( size );
}


void nnet::NeuronArray::read( xmlNodePtr from ) 
{
	Map::read( from );
}


void nnet::NeuronArray::create( size_t array_size, size_t synapses ) 
{
    IntPtr size( get( "size" ) );
	size->set( array_size );

    IntPtr inputs( get( "inputs" ) );
	inputs->set( synapses );
}


bool nnet::NeuronArray::check() 
{
    IntPtr size( get( "size" ) );
    m_size = size->get();

    IntPtr inputs( get( "inputs" ) );
    m_inputs = inputs->get();

	return true;
}


nnet::Unit nnet::NeuronArray::recall( size_t, Unit* ) 
{ 
    LOG_UNIMPL
    return 0; 
}


void nnet::NeuronArray::learn( size_t, Unit*, Unit ) 
{
    LOG_UNIMPL
}


void nnet::NeuronArray::reset() 
{
    LOG_UNIMPL
}

