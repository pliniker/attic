

#include "Operations.h"
#include "ErrorInterface.h"
#include "BadObject.h"
#include "Slot.h"
#include "Layer.h"



nnet::Layer::Layer( std::string name ) :
    Map( name )
{
    CStringPtr    displayName(new CString( "displayName" ) );
    CStringPtr    group(      new CString( "group" ) );

    IntPtr        inputs(     new Int( "inputs" ) );

    MapPtr        inputList(  new Map( "inputList" ) );
    MapPtr        outputList( new Map( "outputList" ) );

    BooleanPtr    ready(      new Boolean( "ready" ) );
    BooleanPtr    enabled(    new Boolean( "enabled" ) );

    ImagePtr      state(      new Image( "state" ) );

	inputs->setReadonly( true );
	inputList->setPrivate( true );
	outputList->setPrivate( true );
	ready->setReadonly( true );
	
	add( displayName );
	add( group );
	add( inputs );
	add( state );
	add( inputList );
	add( outputList );
	add( ready );
	add( enabled );
    add( state );

	displayName->set( name );
	enabled->set( true );
}


nnet::Layer::~Layer() {}


void nnet::Layer::read( xmlNodePtr node ) 
{
	Map::read( node );
}


bool nnet::Layer::check() 
{
    IntPtr inputs( get( "inputs" ) );
    m_inputs = inputs->get();
    
    ImagePtr state( get( "state" ) );
    m_neurons = state->size();
    m_state   = &state->at( 0 );
    
	return true;
}


void nnet::Layer::reset() 
{
    LOG_UNIMPL
}


bool nnet::Layer::attachTo( LayerPtr object ) 
{
    MapPtr outputList( get( "outputList" ) );
	outputList->add( object );
    return true;
}


void nnet::Layer::detachTo( LayerPtr object ) 
{
    MapPtr outputList( get( "outputList" ) );
    
	for( Map::iterator i = outputList->begin(); i != outputList->end(); i++ ) 
    {
		if( equivalent( outputList->get( i ), ( SerializablePtr )object ) ) 
        {
			outputList->erase( i );
			return;
		}
	}
}


bool nnet::Layer::attachFrom( LayerPtr object ) 
{
    MapPtr inputList( get( "inputList" ) );
	inputList->add( object );
    return true;
}


void nnet::Layer::detachFrom( LayerPtr object ) 
{
    MapPtr inputList( get( "inputList" ) );
    
	for( Map::iterator i = inputList->begin(); i != inputList->end(); i++ ) 
    {
		if( equivalent( inputList->get( i ), object ) ) 
        {
			inputList->erase( i );
			return;
		}
	}
}


void nnet::Layer::detachAll() 
{
    MapPtr inputList( get( "inputList" ) );
    MapPtr outputList( get( "outputList" ) );
	inputList->clear();
	outputList->clear();
}


size_t nnet::Layer::numInputs()
{
    return m_inputs;
}


size_t nnet::Layer::numNeurons()
{
    return m_neurons;
}


void nnet::Layer::layerMessage( std::string message ) 
{
    CStringPtr displayName( get( "displayName" ) );
	std::string msg = displayName->get() + ": " + message;
	error::log( msg );
}


bool nnet::check( LayerPtr layer ) 
{
	try 
    {
		if( layer.get() ) 
        {
			bool status = layer->check();
            
            BooleanPtr ready( layer->get( "ready" ) );
			ready->set( status );
            
            nnet::layer::check_signal( layer );
			return status;
		} 
        else 
        {
			return false;
		}
	}
	catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
		throw BadObject( layer, e.what() );
	}
    
	return false;
}


void nnet::reset( LayerPtr layer ) 
{
    try 
    {
        if( layer.get() ) 
        {
            BooleanPtr ready( layer->get( "ready" ) );
            BooleanPtr enabled( layer->get( "enabled" ) );
            
            if( ready->get() && enabled->get() ) 
            {
                layer->reset();
                nnet::layer::reset_signal( layer );
            }
        }
    }
    catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        throw BadObject( layer, e.what() );
    }
}


// send AttachSignal and return true if both attachTo and attachFrom are
// successful
bool nnet::attach( RefCountPtr<Layer> from, RefCountPtr<Layer> to ) 
{
    bool ret = false;
    
	if( from->attachTo( to ) ) 
    {
        if( to->attachFrom( from ) ) 
        {
            nnet::layer::attach_signal( from, to );
            ret = true;
        } 
        else 
        {
            from->detachTo( to );
        }
    }
    
    return ret;
}


bool nnet::detach( RefCountPtr<Layer> from, RefCountPtr<Layer> to ) 
{
	from->detachTo( to );
	to->detachFrom( from );
    nnet::layer::detach_signal( from, to );
    return true;
}



